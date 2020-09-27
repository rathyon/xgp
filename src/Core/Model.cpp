#include <Model.h>
#include <Resources.h>
#include <Utils.h>
#include <BlinnPhongMaterial.h>

#include <Geometry.h>

#include <iostream>
#include <filesystem>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

using namespace xgp;

namespace std {
    template<> struct hash<ObjVertex> {
        size_t operator()(ObjVertex const& vertex) const {
            return ((hash<glm::vec3>()(vertex.pos) ^
                (hash<glm::vec3>()(vertex.normal) << 1)) >> 1) ^
                (hash<glm::vec2>()(vertex.texCoord) << 1);
        }
    };
}

Model::Model(const std::shared_ptr<Geometry>& geo) {
    _geometry = geo;
    prepare();
}

Model::Model(const std::shared_ptr<Geometry>& geo, const glm::vec3& position)
    : Shape(position) {
    _geometry = geo;
    prepare();
}

Model::Model(const std::shared_ptr<Geometry>& geo, const glm::mat4& objToWorld)
    : Shape(objToWorld) {
    _geometry = geo;
    prepare();
}

void Model::prepare() {
    //potentially other things...

    _geometry->upload();
}

void Model::draw() {
    updateMatrix();

    glUseProgram(_material->program());

    glUniformMatrix4fv(glGetUniformLocation(_material->program(), "ModelMatrix"), 1, GL_FALSE, glm::value_ptr(objToWorld()));
    glUniformMatrix3fv(glGetUniformLocation(_material->program(), "NormalMatrix"), 1, GL_FALSE, glm::value_ptr(normalMatrix()));

    if (_material)
        _material->uploadData();

    glBindVertexArray(_geometry->VAO());
    //glDrawArrays(GL_TRIANGLES, 0, _geometry->vertices().size());
    glDrawElements(GL_TRIANGLES, _geometry->indices().size(), GL_UNSIGNED_INT, 0); // has a specific "problem", check Geometry class for details
    glBindVertexArray(0);

    glUseProgram(0);
}

std::vector<std::shared_ptr<Model>> xgp::loadOBJ(const std::string& filePath) {
    tinyobj::ObjReader objReader = tinyobj::ObjReader();
    tinyobj::ObjReaderConfig config = tinyobj::ObjReaderConfig();
    config.vertex_color = false;

    if (!objReader.ParseFromFile(filePath, config)) {
        std::cerr << objReader.Error() << std::endl;
        std::cerr << "ERROR: Failed to load OBJ file: " << filePath << std::endl;
    }
    std::cout << objReader.Warning() << std::endl;

    std::filesystem::path p = std::filesystem::path(filePath);
    if (!std::filesystem::exists(p))
        std::cerr << "ERROR: Failed to load OBJ file: " << std::endl << "Path does not exist!" << std::endl;

    //Load Materials
    const std::vector<tinyobj::material_t> objMaterials = objReader.GetMaterials();
    std::vector<std::shared_ptr<Material>> materials;

    // Currently loading as BP material, will switch to PBR in the future
    for (tinyobj::material_t objMaterial : objMaterials) {
        std::shared_ptr<BlinnPhongMaterial> material = std::make_shared<BlinnPhongMaterial>();

        material->setAmbient(glm::vec3(objMaterial.ambient[0], objMaterial.ambient[1], objMaterial.ambient[2]));
        material->setShininess(objMaterial.shininess);

        if (objMaterial.diffuse_texname.size() > 0) {
            if(Resource.getTexture(objMaterial.diffuse_texname))
                material->setDiffuseTex(Resource.getTexture(objMaterial.diffuse_texname)->id());
            else {
                std::shared_ptr<Texture> diffuseMap = std::make_shared<Texture>();
                diffuseMap->loadTexture(p.parent_path().string() + "/" + objMaterial.diffuse_texname);
                Resource.addTexture(objMaterial.diffuse_texname, diffuseMap);
                material->setDiffuseTex(diffuseMap->id());
            }
        }
        else {
            material->setDiffuse(glm::vec3(objMaterial.diffuse[0], objMaterial.diffuse[1], objMaterial.diffuse[2]));
        }

        if (objMaterial.specular_texname.size() > 0) {
            if (Resource.getTexture(objMaterial.specular_texname))
                material->setSpecularTex(Resource.getTexture(objMaterial.specular_texname)->id());
            else {
                std::shared_ptr<Texture> specularMap = std::make_shared<Texture>();
                specularMap->loadTexture(p.parent_path().string() + "/" + objMaterial.specular_texname);
                Resource.addTexture(objMaterial.specular_texname, specularMap);
                material->setSpecularTex(specularMap->id());
            }
        }
        else {
            material->setSpecular(glm::vec3(objMaterial.specular[0], objMaterial.specular[1], objMaterial.specular[2]));
        }

        if (objMaterial.normal_texname.size() > 0) {
            if (Resource.getTexture(objMaterial.normal_texname))
                material->setNormalMap(Resource.getTexture(objMaterial.normal_texname)->id());
            else {
                std::shared_ptr<Texture> normalMap = std::make_shared<Texture>();
                normalMap->loadTexture(p.parent_path().string() + "/" + objMaterial.normal_texname);
                Resource.addTexture(objMaterial.normal_texname, normalMap);
                material->setNormalMap(normalMap->id());
            }
        }

        // also known as height map (I think?)
        if (objMaterial.bump_texname.size() > 0) {
            if (Resource.getTexture(objMaterial.bump_texname))
                material->setHeightMap(Resource.getTexture(objMaterial.bump_texname)->id());
            else {
                std::shared_ptr<Texture> heightMap = std::make_shared<Texture>();
                heightMap->loadTexture(p.parent_path().string() + "/" + objMaterial.bump_texname);
                Resource.addTexture(objMaterial.bump_texname, heightMap);
                material->setHeightMap(heightMap->id());
            }
        }
        materials.push_back(material);
    }

    std::vector<std::shared_ptr<Model>> models;

    tinyobj::attrib_t attrib = objReader.GetAttrib();
    for (const auto& shape : objReader.GetShapes()) {
        std::string name = shape.name;

        // Load Geometry
        std::shared_ptr<Geometry> geometry = std::make_shared<Geometry>();
        std::vector<Vertex> vertices;
        std::vector<GLuint> indices;
        std::unordered_map<ObjVertex, unsigned int> uniqueVertices{};

        for (const auto& index : shape.mesh.indices) {
            ObjVertex objVertex{};

            objVertex.pos = {
                attrib.vertices[3 * index.vertex_index + 0],
                attrib.vertices[3 * index.vertex_index + 1],
                attrib.vertices[3 * index.vertex_index + 2]
            };

            objVertex.normal = {
                attrib.normals[3 * index.normal_index + 0],
                attrib.normals[3 * index.normal_index + 1],
                attrib.normals[3 * index.normal_index + 2]
            };

            objVertex.texCoord = {
                attrib.texcoords[2 * index.texcoord_index + 0],
                attrib.texcoords[2 * index.texcoord_index + 1]
                //1.0f - attrib.texcoords[2 * index.texcoord_index + 1] in case it needs to be flipped
            };

            if (uniqueVertices.count(objVertex) == 0) {
                uniqueVertices[objVertex] = static_cast<GLuint>(vertices.size());

                // includes (empty) tangent attribute
                Vertex vertex{};
                vertex.pos = objVertex.pos;
                vertex.normal = objVertex.normal;
                vertex.texCoord = objVertex.texCoord;
                vertex.tangent = glm::vec3(0);

                vertices.push_back(vertex);
            }
            indices.push_back(uniqueVertices[objVertex]);
        }
        geometry->setVertices(vertices);
        geometry->setIndices(indices);
        geometry->computeTangents();
        //TODO: prevent overwrites
        Resource.addGeometry(name, geometry);

        //Create model
        std::shared_ptr<Model> model = std::make_shared<Model>(geometry);

        // OBJ format specifies material PER face, in this case I'm loading the first one only
        // this should be a rare occurence, I will also be preparing models just in case weird stuff happens
        if(materials.size() > 0)
            model->setMaterial(materials[shape.mesh.material_ids[0]]);
        else {
            std::shared_ptr<BlinnPhongMaterial> default_mat = std::make_shared<BlinnPhongMaterial>();
            model->setMaterial(default_mat);
        }
        model->updateMatrix();
        models.push_back(model);
        Resource.addShape(name, model);
    }

    return models;
}