#ifndef __XGP_IMAGE_H__
#define __XGP_IMAGE_H__

#include <GL/glew.h>
#include <iostream>
#include <vector>

namespace xgp {

	enum ImageType {
		IMG_1D = 0,
		IMG_2D = 1,
		IMG_3D = 2,
		IMG_CUBE = 3
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
		const GLuint id() const;

	private:
		int _width;
		int _height;
		int _channels;
		GLuint _id;
	};

	class Cubemap {
	public:
		Cubemap();
		~Cubemap();

		//void loadCubemap(const std::string& filepath);
		void loadCubemap(std::vector<std::string> filepaths);

		const int width() const;
		const int height() const;
		const int channels() const;
		const GLuint id() const;

	private:
		int _width;
		int _height;
		int _channels;
		GLuint _id;
	};
}



#endif