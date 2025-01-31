#include <graphics/window.h>
#include <iostream>

Window::Window(int _w, int _h, const char* _title) : _m_width(_w), _m_height(_h), window(nullptr) {
	if (!glfwInit()) {
		std::cerr << "FATAL ERROR: Could not initialize GLFW\n";
		return;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif // __APPLE__

	window = glfwCreateWindow(_m_width, _m_height, _title, NULL, NULL);
	if (!window) {
		std::cerr << "FATAL ERROR: Could not create the window\n";
		glfwTerminate();
		return;
	}

	glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
}

Window::~Window() {
	glfwDestroyWindow(window);
	glfwTerminate();
}

bool Window::should_close() {
	return glfwWindowShouldClose(window);
}

void Window::update() {
	glfwGetWindowSize(window, &_m_width, &_m_height);
	glfwSwapBuffers(window);
	glfwPollEvents();
}

void Window::framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

int Window::get_width() const {
	return _m_width;
}

int Window::get_height() const {
	return _m_height;
}

GLFWwindow* Window::get_GLFWwindow() const {
	return window;
}