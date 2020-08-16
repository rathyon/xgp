#include "Image.h"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_FAILURE_USERMSG
#include <stb_image.h>

using namespace xgp;

Image::Image() { }

Image::~Image() {
}

void Image::loadImage(const std::string& filepath, ImageType type) {
	stbi_set_flip_vertically_on_load(true);
	_data = stbi_load(filepath.c_str(), &_width, &_height, &_channels, 0);

	// allocation failure or image is corrupt or invalid
	if (!_data) {
		std::cout << "ERROR loading image: " << stbi_failure_reason() << std::endl;
		exit(EXIT_FAILURE);
	}
	_type = type;
}

// Remember to delete the image after generating the textures!
void Image::free() {
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

const ImageType Image::type() const {
	return _type;
}

unsigned char* Image::data() const {
	return _data;
}