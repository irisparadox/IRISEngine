#ifndef _CAMERA_
#define _CAMERA_

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <world/time.h>
#include "input.h"

class Camera {
	using vec3 = glm::vec3;

public:
	Camera(vec3 _start_pos, vec3 _upvec, vec3 _start_front, GLFWwindow* _win) :
		_Mypos(_start_pos), _Worldup(_upvec), _Myspeed(2.5f), _Mysens(0.2f), _Myfov(75.0f),
		_Clipnear(0.01f), _Clipfar(100.0f), _Smoothness(0.6f), cursor_enabled(false), window(_win) {
		_quat = glm::quatLookAt(glm::normalize(_start_front), _Worldup);
		update_last_mouse_pos();
		_vec_update();
	};

	glm::mat4 get_view_matrix() const {
		return glm::lookAt(_Mypos, _Mypos + _Vecfront, _Vecup);
	}

	glm::mat4 get_projection_matrix(float aspect_ratio) const {
		return glm::perspective(glm::radians(_Myfov), aspect_ratio, _Clipnear, _Clipfar);
	}

	void set_window(GLFWwindow* _window) {
		window = _window;
	}

public:
	void update_last_mouse_pos() {
		double x, y;
		glfwGetCursorPos(window, &x, &y);
		_last_mouse_pos = glm::vec2(static_cast<float>(x), -static_cast<float>(y));
	}

	void update() {
		double x, y;
		glfwGetCursorPos(window, &x, &y);
		_mouse_pos = glm::vec2(static_cast<float>(x), -static_cast<float>(y));
		glm::vec2 delta = (_mouse_pos - _last_mouse_pos);
		_last_mouse_pos = _mouse_pos;

		if (cursor_enabled) {
			update_last_mouse_pos();
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
		else {
			process_mouse(delta.x, delta.y);
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}

		process_keyboard();
	}

	void process_keyboard() {
		float velocity = _Myspeed * Time::delta_time;
		if (Input::is_key_pressed(InputAction::SPRINT)) velocity *= 2;
		if (Input::is_key_pressed(InputAction::MOVE_FORWARD)) _Mypos += glm::normalize(vec3(_Vecfront.x, 0.0f, _Vecfront.z)) * velocity;
		if (Input::is_key_pressed(InputAction::MOVE_BACKWARD)) _Mypos -= glm::normalize(vec3(_Vecfront.x, 0.0f, _Vecfront.z)) * velocity;
		if (Input::is_key_pressed(InputAction::MOVE_LEFT)) _Mypos -= glm::normalize(vec3(_Vecright.x, 0.0f, _Vecright.z)) * velocity;
		if (Input::is_key_pressed(InputAction::MOVE_RIGHT)) _Mypos += glm::normalize(vec3(_Vecright.x, 0.0f, _Vecright.z)) * velocity;
		if (Input::is_key_pressed(InputAction::MOVE_UP)) _Mypos += vec3(0.0f, 1.0f, 0.0f) * velocity;
		if (Input::is_key_pressed(InputAction::MOVE_DOWN)) _Mypos -= vec3(0.0f, 1.0f, 0.0f) * velocity;

		cursor_enabled = Input::get_key_state(InputAction::PAUSE);
	}

private:
	void _vec_update() {
		_Vecfront = glm::normalize(_quat * glm::vec3(0.0f, 0.0f, -1.0f));
		_Vecright = glm::normalize(glm::cross(_Vecfront, _Worldup));
		_Vecup    = glm::normalize(glm::cross(_Vecright, _Vecfront));
	}

	void process_mouse(float xOffset, float yOffset) {
		xOffset *= _Mysens;
		yOffset *= _Mysens;

		glm::quat yaw_quat = glm::angleAxis(glm::radians(-static_cast<float>(xOffset)), _Worldup);

		vec3 forward = _quat * vec3(0.0f, 0.0f, -1.0f);
		float current_pitch = glm::degrees(asin(forward.y));

		float new_pitch = glm::clamp(current_pitch + static_cast<float>(yOffset), -84.0f, 84.0f);
		float delta_pitch = new_pitch - current_pitch;

		vec3 local_right = glm::normalize(glm::cross(forward, _Worldup));
		glm::quat pitch_quat = glm::angleAxis(glm::radians(delta_pitch), local_right);

		glm::quat target_quat = glm::normalize(pitch_quat * yaw_quat * _quat);

		_quat = glm::slerp(_quat, target_quat, _Smoothness);

		_vec_update();
	}

private:
	vec3 _Mypos;
	vec3 _Vecfront;
	vec3 _Vecup;
	vec3 _Vecright;
	vec3 _Worldup;

	glm::quat _quat;
	glm::vec2 _mouse_pos;
	glm::vec2 _last_mouse_pos{ 0.0f, 0.0f };

	GLFWwindow* window;

	float _Myspeed, _Mysens, _Myfov;
	float _Clipnear, _Clipfar;
	float _Smoothness;

	bool cursor_enabled;
};

#endif // !_CAMERA_
