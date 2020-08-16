#ifndef __XGP_TEXTURE_H__
#define __XGP_TEXTURE_H__

#include <GL/glew.h>
#include "Image.h"

namespace xgp {

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

	class Texture {
	public:
		Texture(Image img);
		~Texture();

		ImageType type() const;
		GLuint id() const;

	private:
		ImageType _type;
		GLuint _id;
	};

}

#endif