#ifndef __XGP_PERSPECTIVE_H__
#define __XGP_PERSPECTIVE_H__

#include "Camera.h"

namespace xgp {

	class Perspective : public Camera {
	public:
		Perspective(int width, int height, const glm::vec3& eye,
			const glm::vec3& at, const glm::vec3& up, float near, float far, float fov);

		float fov() const;
		void updateProjMatrix(int width, int height) override;

	private:
		float _fov;
	};

}

#endif