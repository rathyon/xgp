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

	const GLenum PixelFormats[] = {
	0,
	GL_RED,
	GL_RG,
	GL_RGB,
	GL_RGBA
	};

	enum class CubemapType {
		SKYBOX,
		IRRADIANCE,
		GGX
	};

	class Image {
	public:
		Image() { }

		const int width() const;
		const int height() const;
		const int channels() const;
		const GLuint id() const;

	protected:
		int _width;
		int _height;
		int _channels;
		GLuint _id;
	};

	class Texture : public Image {
	public:
		Texture(const std::string& filepath);
		~Texture();
	};

	class Cubemap : public Image {
	public:
		Cubemap(std::string directory, CubemapType type);
		~Cubemap();
	};
}



#endif