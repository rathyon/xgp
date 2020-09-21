#ifndef __XGP_SKYBOX_H__
#define __XGP_SKYBOX_H__

#include <string>
#include <vector>
#include <Image.h>

namespace xgp {

	class Skybox {
	public:
		Skybox(std::vector<std::string> filepaths);
		~Skybox();

		void draw() const;

	private:
		void genCubeGeometry();

		GLuint _vao;
		GLuint _prog;

		Cubemap* _cubemap;
	};

}

#endif