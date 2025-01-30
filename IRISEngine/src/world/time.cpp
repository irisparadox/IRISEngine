#include <world/time.h>
#include <GLFW/glfw3.h>

float Time::delta_time = 0;
float Time::time = 0;
float Time::last_frame = 0;

void Time::init() {
	delta_time = 0;
	time = 0;
	last_frame = 0;
}

void Time::update() {
	float current_frame = static_cast<float>(glfwGetTime());
	delta_time = current_frame - last_frame;
	last_frame = current_frame;

	time += delta_time;
}

void Time::end() {
	delta_time = 0;
	last_frame = 0;
	time = 0;
}