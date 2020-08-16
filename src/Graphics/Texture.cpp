#include "Texture.h"

#include <Utils.h>

using namespace xgp;

Texture::Texture(Image img) {
	_type = img.type();
	glGenTextures(1, &_id);
	glBindTexture(TextureTargets[_type], _id);

	if (_type == IMG_1D) {
	}
	else if (_type == IMG_2D) {
		glTexImage2D(TextureTargets[_type],
			0, 
			PixelFormats[img.channels()],
			img.width(), 
			img.height(),
			0, 
			PixelFormats[img.channels()],
			GL_UNSIGNED_BYTE,  // data is stored as unsigned char in Image
			img.data());

		Utils::checkOpenGLError("Error in glTexImage2D");

		glGenerateMipmap(TextureTargets[_type]);
		Utils::checkOpenGLError("Error in generating mipmaps");

	}
	else if (_type == IMG_3D) {
	}
	else if (_type == IMG_CUBE) {
	}

	glTexParameteri(TextureTargets[_type], GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(TextureTargets[_type], GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(TextureTargets[_type], GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(TextureTargets[_type], GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(TextureTargets[_type], 0);

	img.free();
}

Texture::~Texture() { }

ImageType Texture::type() const {
	return _type;
}

GLuint Texture::id() const {
	return _id;
}