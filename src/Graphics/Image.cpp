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
	stbi_set_flip_vertically_on_load(true);
	_data = stbi_load(filepath.c_str(), &_width, &_height, &_channels, 0);

	// allocation failure or image is corrupt or invalid
	if (!_data) {
		std::cout << "ERROR loading image: " << stbi_failure_reason() << std::endl;
		exit(EXIT_FAILURE);
	}

	glGenTextures(1, &_id);
	glBindTexture(TextureTargets[IMG_2D], _id);

	glTexImage2D(TextureTargets[IMG_2D],
		0,
		PixelFormats[_channels],
		_width,
		_height,
		0,
		PixelFormats[_channels],
		GL_UNSIGNED_BYTE,  // data is stored as unsigned char in Image
		_data);

	Utils::checkOpenGLError("Error in glTexImage2D");

	glGenerateMipmap(TextureTargets[IMG_2D]);
	Utils::checkOpenGLError("Error in generating mipmaps");

	glTexParameteri(TextureTargets[IMG_2D], GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(TextureTargets[IMG_2D], GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(TextureTargets[IMG_2D], GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(TextureTargets[IMG_2D], GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(TextureTargets[IMG_2D], 0);

	// Remember to delete the image after generating the textures!
	stbi_image_free(_data);
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

unsigned char* Image::data() const {
	return _data;
}

const GLuint Image::id() const {
	return _id;
}