#include "Material.h"

using namespace xgp;

void Material::use() const {
    glUseProgram(_program);
}

GLuint Material::program() const {
    return _program;
}

void Material::setInt(const std::string& name, int value) {
	glUniform1i(glGetUniformLocation(_program, name.c_str()), value);
}

void Material::setFloat(const std::string& name, float value) {
	glUniform1f(glGetUniformLocation(_program, name.c_str()), value);
}

void Material::setVec3(const std::string& name, glm::vec3& vec) {
	glUniform3fv(glGetUniformLocation(_program, name.c_str()), 1, glm::value_ptr(vec));
}

void Material::setVec4(const std::string& name, glm::vec4& vec) {
	glUniform4fv(glGetUniformLocation(_program, name.c_str()), 1, glm::value_ptr(vec));
}

void Material::setMat3(const std::string& name, glm::mat3& mat) {
	glUniformMatrix3fv(glGetUniformLocation(_program, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
}

void Material::setMat4(const std::string& name, glm::mat4& mat) {
	glUniformMatrix4fv(glGetUniformLocation(_program, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
}

void Material::setSampler(const std::string& name, int id) {
	setInt(name, id);
}