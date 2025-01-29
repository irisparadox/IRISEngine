#ifndef TIME_H
#define TIME_H

#include <GLFW/glfw3.h>

class Time {
public:
	static float delta_time;
	static float time;

public:
	static void init() {
		delta_time = 0;
		time = 0;
		last_frame = 0;
	}

	static void update() {
		float current_frame = static_cast<float>(glfwGetTime());
		delta_time = current_frame - last_frame;
		last_frame = current_frame;

		time += delta_time;
	}

	static void end() {
		delta_time = 0;
		last_frame = 0;
		time = 0;
	}

private:
	static float last_frame;
};

#endif // !TIME_H
