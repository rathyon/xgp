#ifndef __XGP_IMAGE_H__
#define __XGP_IMAGE_H__

#include <GL/glew.h>
#include <iostream>

namespace xgp {

	enum ImageType {
		IMG_1D = 0,
		IMG_2D = 1,
		IMG_3D = 2,
		IMG_CUBE = 3
	};

	const GLenum TextureTargets[] = {
		GL_TEXTURE_1D,
		GL_TEXTURE_2D,
		GL_TEXTURE_3D,
		GL_TEXTURE_CUBE_MAP
	};

	const GLenum PixelFormats[] = {
	0,
	GL_RED,
	GL_RG,
	GL_RGB,
	GL_RGBA
	};

	/*
		Images are 2D by default, might be extended in the future to accomodate other types
	*/
	class Image {
	public:
		Image();
		~Image();

		void loadImage(const std::string& filepath);

		const int width() const;
		const int height() const;
		const int channels() const;
		unsigned char* data() const;
		const GLuint id() const;

	private:
		int _width;
		int _height;
		int _channels;
		unsigned char* _data;
		GLuint _id;
	};
}



#endif