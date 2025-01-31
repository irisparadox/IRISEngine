#include <controller/input.h>

std::unordered_map<InputAction, std::pair<int, bool>> Input::key_map;
std::unordered_map<InputAction, bool> Input::key_state;
GLFWwindow* Input::window = nullptr;