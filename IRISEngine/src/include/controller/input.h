#ifndef _INPUT_
#define _INPUT_

#include <unordered_map>
#include <GLFW/glfw3.h>

enum class InputAction {
	MOVE_FORWARD,
	MOVE_BACKWARD,
	MOVE_LEFT,
	MOVE_RIGHT,
	MOVE_UP,
	MOVE_DOWN,
	SPRINT,
	PAUSE
};

class Input {
public:
	static void init(GLFWwindow* _win) {
		window = _win;
		key_map[InputAction::MOVE_FORWARD] = { GLFW_KEY_W, false };
		key_map[InputAction::MOVE_BACKWARD] = { GLFW_KEY_S, false };
		key_map[InputAction::MOVE_LEFT] = { GLFW_KEY_A, false };
		key_map[InputAction::MOVE_RIGHT] = { GLFW_KEY_D, false };
		key_map[InputAction::MOVE_UP] = { GLFW_KEY_SPACE, false };
		key_map[InputAction::MOVE_DOWN] = { GLFW_KEY_LEFT_CONTROL, false };
		key_map[InputAction::SPRINT] = { GLFW_KEY_LEFT_SHIFT, false };
		key_map[InputAction::PAUSE] = { GLFW_KEY_ESCAPE, true };

		for (const auto& [action, _] : key_map) {
			key_state[action] = false;
		}
	}

	static void rebind(InputAction action, int new_key, bool toggle) {
		key_map[action] = { new_key, toggle };
	}

	static bool is_key_pressed(InputAction action) {
		if (key_map.find(action) != key_map.end()) {
			int key = key_map[action].first;
			return glfwGetKey(window, key);
		}
		return false;
	}

	static bool get_key_state(InputAction action) {
		return key_state[action];
	}

	static void key_callback(GLFWwindow* _window, int key, int scancode, int action, int mods) {
		if (key < 0 || key >= GLFW_KEY_LAST) return;
		
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
			key_state[InputAction::PAUSE] = !key_state[InputAction::PAUSE];
		}
	}

private:
	static std::unordered_map<InputAction, std::pair<int, bool>> key_map;
	static std::unordered_map<InputAction, bool> key_state;
	static GLFWwindow* window;
};

#endif // !_INPUT_
