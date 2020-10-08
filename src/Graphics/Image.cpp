#include "Image.h"
#include <vector>
#include <filesystem>

#define STB_IMAGE_IMPLEMENTATION
#define STBI_FAILURE_USERMSG
#include <stb_image.h>

#include <gli/gli.hpp>

using namespace xgp;

const int Image::width() const {
	return _width;
}

const int Image::height() const {
	return _height;
}

const int Image::channels() const {
	return _channels;
}

const GLuint Image::id() const {
	return _id;
}

Texture::Texture(const std::string& filepath) {
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(filepath.c_str(), &_width, &_height, &_channels, 0);

	if (!data) {
		std::cout << "ERROR loading texture: " << stbi_failure_reason() << std::endl;
		exit(EXIT_FAILURE);
	}

	glGenTextures(1, &_id);
	glBindTexture(GL_TEXTURE_2D, _id);

	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		PixelFormats[_channels],
		_width,
		_height,
		0,
		PixelFormats[_channels],
		GL_UNSIGNED_BYTE,
		data);

	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(data);
}

Texture::Texture(const std::string& filepath, TexSampler& sampler) {
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(filepath.c_str(), &_width, &_height, &_channels, 0);

	if (!data) {
		std::cout << "ERROR loading texture: " << stbi_failure_reason() << std::endl;
		exit(EXIT_FAILURE);
	}

	glGenTextures(1, &_id);
	glBindTexture(GL_TEXTURE_2D, _id);

	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		sampler.floatData ? FloatPixelFormats[_channels] : PixelFormats[_channels],
		_width,
		_height,
		0,
		PixelFormats[_channels],
		GL_UNSIGNED_BYTE,
		data);

	if (sampler.genMipmap)
		glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, sampler.wrapS);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, sampler.wrapT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, sampler.min);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, sampler.mag);


	glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture() {
}

// restricted to .dds files only!
Cubemap::Cubemap(std::string directory, CubemapType type) {
	gli::gl GL(gli::gl::PROFILE_GL33);
	gli::texture texture = gli::load(directory.c_str());

	gli::gl::format const format = GL.translate(texture.format(), texture.swizzles());
	GLenum target = GL.translate(texture.target());
	glm::tvec3<GLsizei> const extent(texture.extent());

	glGenTextures(1, &_id);
	glBindTexture(GL_TEXTURE_CUBE_MAP, _id);

	_width = extent.x;
	_height = extent.y;
	glTexStorage2D(
		GL_TEXTURE_CUBE_MAP,
		static_cast<GLint>(texture.levels()),
		format.Internal,
		_width,
		_height);

	for (int face = 0; face < 6; face++) {
		for (std::size_t level = 0; level < texture.levels(); ++level) {
			glm::tvec3<GLsizei> levelExtent(texture.extent(level));
			glTexSubImage2D(
				GL_TEXTURE_CUBE_MAP_POSITIVE_X + face,
				static_cast<GLint>(level),
				0,
				0,
				levelExtent.x,
				levelExtent.y,
				format.External,
				format.Type,
				texture.data(0, face, level));
		}
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if (type == CubemapType::GGX) {
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	}
	else {
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	//*********************************************************************************************************
	/** /
	glGenTextures(1, &_id);
	glBindTexture(GL_TEXTURE_CUBE_MAP, _id);

	const std::string faces[] = { "posx", "negx", "posy", "negy", "posz", "negz" };

	std::string prefix;
	std::string extension;
	if (type == CubemapType::SKYBOX) {
		prefix = "";
		extension = ".hdr";
	}
	else if (type == CubemapType::IRRADIANCE) {
		prefix = "irradiance_";
		extension = ".dds";
	}
	else {
		prefix = "ggx_";
		extension = ".dds";
	}

	if (type == CubemapType::SKYBOX) {
		stbi_set_flip_vertically_on_load(false);

		for (unsigned int face = 0; face < 6; face++) {
			std::string filepath = directory + prefix + faces[face] + extension;
			float* data = stbi_loadf(filepath.c_str(), &_width, &_height, &_channels, 0);

			if (!data) {
				std::cout << "ERROR loading cubemap: " << stbi_failure_reason() << std::endl;
				exit(EXIT_FAILURE);
			}

			glTexImage2D(
				GL_TEXTURE_CUBE_MAP_POSITIVE_X + face,
				0,
				_channels == 4 ? GL_RGBA16F : GL_RGB16F,
				_width,
				_height,
				0,
				_channels == 4 ? GL_RGBA : GL_RGB,
				GL_FLOAT,
				data);

			stbi_image_free(data);
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}
	else {
		gli::gl GL(gli::gl::PROFILE_GL33);
		// Load order: xpos xneg ypos yneg zpos zneg
		for (unsigned int face = 0; face < 6; face++) {
			std::string filepath = directory + prefix + faces[face] + extension;
			gli::texture texture = gli::load(filepath.c_str());

			gli::gl::format const format = GL.translate(texture.format(), texture.swizzles());
			GLenum target = GL.translate(texture.target());
			glm::tvec3<GLsizei> const extent(texture.extent());

			// only done once
			if (face == 0) {
				_width = extent.x;
				_height = extent.y;
				glTexStorage2D(
					GL_TEXTURE_CUBE_MAP,
					static_cast<GLint>(texture.levels()),
					format.Internal,
					_width,
					_height);
			}

			for (std::size_t level = 0; level < texture.levels(); ++level) {
				glm::tvec3<GLsizei> levelExtent(texture.extent(level));
				glTexSubImage2D(
					GL_TEXTURE_CUBE_MAP_POSITIVE_X + face,
					static_cast<GLint>(level),
					0,
					0,
					levelExtent.x,
					levelExtent.y,
					format.External,
					format.Type,
					texture.data(0, 0, level));
			}
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	}

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	/**/
}

Cubemap::~Cubemap() {
}