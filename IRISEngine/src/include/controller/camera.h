#ifndef _CAMERA_
#define _CAMERA_

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <world/time.h>

#define FORWARD glm::vec3(1.0f, 0.0f, 0.0f);
#define BACKWARDS glm::vec3(-1.0f, 0.0f, 0.0f);
#define RIGHT glm::vec3(0.0f, 0.0f,  1.0f);
#define LEFT glm::vec3(0.0f, 0.0f, -1.0f);
#define UP glm::vec3(0.0f, 1.0f, 0.0f);
#define DOWN glm::vec3(0.0f, -1.0f, 0.0f)

class Camera {
	using vec3 = glm::vec3;

public:
	Camera(vec3 _start_pos, vec3 _upvec, vec3 _start_front) :
		_Mypos(_start_pos), _Worldup(_upvec), _Myspeed(2.5f), _Mysens(0.2f), _Myfov(75.0f), _Clipnear(0.01f), _Clipfar(100.0f), _Smoothness(0.6f) {
		_quat = glm::quatLookAt(glm::normalize(_start_front), _Worldup);
		_vec_update();
	};

	glm::mat4 get_view_matrix() const {
		return glm::lookAt(_Mypos, _Mypos + _Vecfront, _Vecup);
	}

	glm::mat4 get_projection_matrix(float _aspect_ratio) const {
		return glm::perspective(glm::radians(_Myfov), _aspect_ratio, _Clipnear, _Clipfar);
	}

public:
	void process_keyboard(GLFWwindow* window) {
		float velocity = _Myspeed * Time::delta_time;
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) _Mypos += glm::normalize(vec3(_Vecfront.x, 0.0f, _Vecfront.z)) * velocity;
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) _Mypos -= glm::normalize(vec3(_Vecfront.x, 0.0f, _Vecfront.z)) * velocity;
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) _Mypos -= glm::normalize(vec3(_Vecright.x, 0.0f, _Vecright.z)) * velocity;
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) _Mypos += glm::normalize(vec3(_Vecright.x, 0.0f, _Vecright.z)) * velocity;
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) _Mypos += vec3(0.0f, 1.0f, 0.0f) * velocity;
		if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) _Mypos -= vec3(0.0f, 1.0f, 0.0f) * velocity;
	}

	void process_mouse(double xOffset, double yOffset) {
		xOffset *= static_cast<double>(_Mysens);
		yOffset *= static_cast<double>(_Mysens);

		glm::quat yaw_quat = glm::angleAxis(glm::radians(-static_cast<float>(xOffset)), _Worldup);

		vec3 forward = _quat * vec3(0.0f, 0.0f, -1.0f);
		float current_pitch = glm::degrees(asin(forward.y));

		float new_pitch = glm::clamp(current_pitch + static_cast<float>(yOffset), -85.0f, 85.0f);
		float delta_pitch = new_pitch - current_pitch;

		vec3 local_right	 = glm::normalize(glm::cross(forward, _Worldup));
		glm::quat pitch_quat = glm::angleAxis(glm::radians(delta_pitch), local_right);

		glm::quat target_quat = glm::normalize(pitch_quat * yaw_quat * _quat);
		
		_quat = glm::slerp(_quat, target_quat, _Smoothness);

		_vec_update();
	}

private:
	void _vec_update() {
		_Vecfront = glm::normalize(_quat * glm::vec3(0.0f, 0.0f, -1.0f));
		_Vecright = glm::normalize(glm::cross(_Vecfront, _Worldup));
		_Vecup    = glm::normalize(glm::cross(_Vecright, _Vecfront));
	}

private:
	vec3 _Mypos;
	vec3 _Vecfront;
	vec3 _Vecup;
	vec3 _Vecright;
	vec3 _Worldup;

	glm::quat _quat;

	float _Myspeed, _Mysens, _Myfov;
	float _Clipnear, _Clipfar;
	float _Smoothness;
};

#endif // !_CAMERA_
