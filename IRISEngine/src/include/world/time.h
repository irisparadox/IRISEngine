#ifndef TIME_H
#define TIME_H

class Time {
public:
	static float delta_time;
	static float time;

	static void init();
	static void update();
	static void end();

private:
	static float last_frame;
};

#endif // !TIME_H
