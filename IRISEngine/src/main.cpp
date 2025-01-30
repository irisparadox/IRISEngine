#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <iostream>
#include <algorithm>
#include <world/time.h>
#include <controller/camera.h>

#define STB_IMAGE_IMPLEMENTATION
#include <graphics/stb_image.h>
#include <graphics/shader.h>
#include <graphics/window.h>

int SCR_WIDTH = 1024;
int SCR_HEIGHT = 576;

bool cursorEnabled = false;
bool firstMouse = true;

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		cursorEnabled = !cursorEnabled;
}

int main() {

	Window window(SCR_WIDTH, SCR_HEIGHT, "IRIS Engine");
	glfwSetKeyCallback(window.get_GLFWwindow(), key_callback);
	Camera cam(cameraPos, cameraUp, cameraFront);
	cam.set_window(window.get_GLFWwindow());

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << '\n';
		return -1;
	}

	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	
	Shader rectShader("shaders/testShader.vert", "shaders/testShader.frag");

	float cubeVerts[] = {
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	unsigned int vbo_arr, vao_arr;
	glGenVertexArrays(1, &vao_arr);
	glGenBuffers(1, &vbo_arr);

	glBindVertexArray(vao_arr);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_arr);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVerts), cubeVerts, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	int	width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load("resources/textures/container.jpg", &width, &height, &nrChannels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load texture" << '\n';
	}
	stbi_image_free(data);

	rectShader.use();

	rectShader.set_int("_Texture1", 0);

	// uncomment this call to draw in wireframe polygons.
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window.get_GLFWwindow(), true);
	ImGui_ImplOpenGL3_Init("#version 460");

	Time::init();

	while (!window.should_close()) {
		window.update();
		Time::update();
		if (firstMouse || cursorEnabled) {
			cam.update_last_mouse_pos();
			firstMouse = false;
		}
		else if (!cursorEnabled) cam.update();
		
		if (cursorEnabled)
			glfwSetInputMode(window.get_GLFWwindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		else
			glfwSetInputMode(window.get_GLFWwindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ImGui_ImplGlfw_NewFrame();
		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();
		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);

		glm::mat4 model(1.0f);

		rectShader.set_mat4("_model", model);
		rectShader.set_mat4("_view", cam.get_view_matrix());
		rectShader.set_mat4("_projection", cam.get_projection_matrix((float)window.get_width() / (float)window.get_height()));

		glBindVertexArray(vao_arr);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		/*ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
		ImGui::SetNextWindowSize(ImVec2(70, 50), ImGuiCond_Always);
		ImGui::Begin("FPS", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize);
		ImGui::Text("%.1f", io.Framerate);
		ImGui::End();

		ImGui::SetNextWindowPos(ImVec2(70, 0), ImGuiCond_Always);
		ImGui::SetNextWindowSize(ImVec2(60, 50), ImGuiCond_Always);
		ImGui::Begin("Time", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize);
		ImGui::Text("%.1f", Time::time);
		ImGui::End();*/

		static bool window_bool = true;
		static bool init_window = true;
		if (init_window) {
			ImGui::SetNextWindowSize(ImVec2(269, 163), ImGuiCond_Always);
			ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
			init_window = false;
		}

		if (ImGui::Begin("Debug", &window_bool, ImGuiWindowFlags_None))
		{

			ImGui::SetCursorPos(ImVec2(15, 30));
			ImGui::Text("FPS: %.1f", io.Framerate);

			ImGui::SetCursorPos(ImVec2(15, 50));
			ImGui::Text("Time: %.1f", Time::time);

			ImGui::SetCursorPos(ImVec2(15, 70));
			ImGui::Text("Delta Time: %.4f", Time::delta_time);

			ImGui::SetCursorPos(ImVec2(15, 110));
			static bool c19 = false;
			ImGui::Checkbox("Wireframe", &c19);

			if(c19) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

			/*ImGui::SetCursorPos(ImVec2(15, 70));
			ImGui::Text("CPU Usage: %.2%%", );

			ImGui::SetCursorPos(ImVec2(15, 90));
			ImGui::Text("GPU Usage: %.2%%");*/

		}
		ImGui::End();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
	Time::end();

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glDeleteVertexArrays(1, &vao_arr);
	glDeleteBuffers(1, &vbo_arr);

	return 0;
}