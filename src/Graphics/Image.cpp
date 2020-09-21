#include "Image.h"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_FAILURE_USERMSG
#include <stb_image.h>

#include <Utils.h>

using namespace xgp;

Image::Image() { }

Image::~Image() {
}

void Image::loadImage(const std::string& filepath) {
	glGenTextures(1, &_id);
	glBindTexture(GL_TEXTURE_2D, _id);

	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(filepath.c_str(), &_width, &_height, &_channels, 0);

	// allocation failure or image is corrupt or invalid
	if (!data) {
		std::cout << "ERROR loading image: " << stbi_failure_reason() << std::endl;
		exit(EXIT_FAILURE);
	}

	glTexImage2D(GL_TEXTURE_2D,
		0,
		PixelFormats[_channels],
		_width,
		_height,
		0,
		PixelFormats[_channels],
		GL_UNSIGNED_BYTE,  // data is stored as unsigned char in Image
		data);

	Utils::checkOpenGLError("Error in glTexImage2D");

	glGenerateMipmap(GL_TEXTURE_2D);
	Utils::checkOpenGLError("Error in generating mipmaps");

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);

	// Remember to delete the image after generating the textures!
	stbi_image_free(data);
}

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



Cubemap::Cubemap() { }

Cubemap::~Cubemap() {
}

void Cubemap::loadCubemap(std::vector<std::string> filepaths) {
	glGenTextures(1, &_id);
	glBindTexture(GL_TEXTURE_CUBE_MAP, _id);

	stbi_set_flip_vertically_on_load(false);

	for (unsigned int side = 0; side < 6; side++) {
		unsigned char* data = stbi_load(filepaths[side].c_str(), &_width, &_height, &_channels, 0);
		// allocation failure or image is corrupt or invalid
		if (!data) {
			std::cout << "ERROR loading cubemap: " << stbi_failure_reason() << std::endl;
			exit(EXIT_FAILURE);
		}
		glTexImage2D(
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + side,
			0,
			PixelFormats[_channels],
			_width,
			_height,
			0,
			PixelFormats[_channels],
			GL_UNSIGNED_BYTE,  // data is stored as unsigned char in Image
			data);

		// Remember to delete the image after generating the textures!
		stbi_image_free(data);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

const int Cubemap::width() const {
	return _width;
}

const int Cubemap::height() const {
	return _height;
}

const int Cubemap::channels() const {
	return _channels;
}

const GLuint Cubemap::id() const {
	return _id;
}