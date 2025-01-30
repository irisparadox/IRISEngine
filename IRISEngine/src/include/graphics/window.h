#ifndef _WINDOW_
#define _WINDOW_

#include <GLFW/glfw3.h>

class Window {
public:
	Window(int _w, int _h, const char* _title);
	~Window();

	void update();
	bool should_close();

	int get_width() const;
	int get_height() const;
	GLFWwindow* get_GLFWwindow() const;

private:
	GLFWwindow* window;
	int _m_width, _m_height;

	static void framebuffer_size_callback(GLFWwindow* _win, int _w, int _h);
};

#endif // !_WINDOW_
