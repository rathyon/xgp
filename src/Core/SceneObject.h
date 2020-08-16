#ifndef __XGP_SCENEOBJECT_H__
#define __XGP_SCENEOBJECT_H__

#include <glm/glm.hpp> // vec2, vec3, mat4, radians
#include <glm/ext.hpp> // perspective, translate, rotate

#include <memory>

namespace xgp {

	class SceneObject {
	public:
		SceneObject();
		SceneObject(const glm::vec3& position);
		SceneObject(const glm::mat4& objToWorld);

		const glm::vec3& position()    const;
		const glm::vec3& scale()       const;
		const glm::quat& rotation() const;
		const glm::mat4& objToWorld()  const;

		std::shared_ptr<SceneObject> parent() const;

		void setPosition(const glm::vec3& position);
		void setScale(float x, float y, float z);
		void setRotation(const glm::quat& quat);
		void setObjToWorld(const glm::mat4& mat);

		virtual void updateMatrix();

	protected:
		glm::vec3 _position;
		glm::vec3 _scale;
		glm::quat _rotation;

		glm::mat4 _objToWorld;

		std::shared_ptr<SceneObject> _parent;
	};

}

#endif