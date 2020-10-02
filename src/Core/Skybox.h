#ifndef __XGP_SKYBOX_H__
#define __XGP_SKYBOX_H__

#include <string>
#include <Image.h>

namespace xgp {

	class Skybox {
	public:
		Skybox(std::string directory);
		~Skybox();

		void draw() const;

		GLuint envMap() const;
		GLuint irradianceMap() const;
		GLuint ggxPrefilterMap() const;

	private:
		void genCubeGeometry();

		GLuint _vao;
		GLuint _prog;

		GLuint _envMap;
		GLuint _irradianceMap;
		GLuint _ggxPrefilterMap;
	};

}

#endif