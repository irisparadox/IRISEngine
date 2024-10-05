#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

const char* vertexShaderSource = "#version 460 core\n"
								 "layout (location = 0) in vec3 aPos;\n"
								 "void main() {\n"
								 "	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
								 "}\0";

const char* tri_one_frag_source = "#version 460 core\n"
								  "out vec4 FragColor;\n"
								  "void main() {\n"
								  "	FragColor = vec4(0.85, 1.00, 0.62, 1.0);\n"
								  "}\n";

const char* tri_two_frag_source = "#version 460 core\n"
								  "out vec4 FragColor;\n"
								  "void main() {\n"
								  "	FragColor = vec4(0.26, 0.41, 1.00, 1.0);\n"
								  "}\n";

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

int main() {

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif // __APPLE__


	GLFWwindow* window = glfwCreateWindow(800, 600, "IRIS Engine", NULL, NULL);

	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << '\n';
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << '\n';
		return -1;
	}

	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << '\n';
	}

	unsigned int tri_one_frag, tri_two_frag;
	tri_one_frag = glCreateShader(GL_FRAGMENT_SHADER);
	tri_two_frag = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(tri_one_frag, 1, &tri_one_frag_source, NULL);
	glCompileShader(tri_one_frag);

	glShaderSource(tri_two_frag, 1, &tri_two_frag_source, NULL);
	glCompileShader(tri_two_frag);

	glGetShaderiv(tri_one_frag, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(tri_one_frag, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << '\n';
	}

	glGetShaderiv(tri_two_frag, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(tri_two_frag, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << '\n';
	}

	unsigned int tri_one_shader_prog, tri_two_shader_prog;
	tri_one_shader_prog = glCreateProgram();
	tri_two_shader_prog = glCreateProgram();

	glAttachShader(tri_one_shader_prog, vertexShader);
	glAttachShader(tri_one_shader_prog, tri_one_frag);
	glLinkProgram(tri_one_shader_prog);

	glAttachShader(tri_two_shader_prog, vertexShader);
	glAttachShader(tri_two_shader_prog, tri_two_frag);
	glLinkProgram(tri_two_shader_prog);

	glGetProgramiv(tri_one_shader_prog, GL_LINK_STATUS, &success);

	if (!success) {
		glGetProgramInfoLog(tri_one_shader_prog, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << '\n';
	}

	glGetProgramiv(tri_two_shader_prog, GL_LINK_STATUS, &success);

	if (!success) {
		glGetProgramInfoLog(tri_two_shader_prog, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << '\n';
	}

	glDeleteShader(vertexShader);
	glDeleteShader(tri_one_frag);
	glDeleteShader(tri_two_frag);

	float tri_one_verts[] = {
		-0.5f,  0.5f, 0.0f, // A
		-1.0f, -0.5f, 0.0f, // B
		-0.1f,  0.0f, 0.0f  // C
	};

	float tri_two_verts[] = {
		0.5f,  0.5f, 0.0f, // A
		0.1f,  0.0f, 0.0f, // B
		1.0f, -0.5f, 0.0f  // C
	};

	unsigned int vbo_arr[2], vao_arr[2];
	glGenVertexArrays(2, vao_arr);
	glGenBuffers(2, vbo_arr);

	glBindVertexArray(vao_arr[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_arr[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(tri_one_verts), tri_one_verts, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(vao_arr[1]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_arr[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(tri_two_verts), tri_two_verts, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// uncomment this call to draw in wireframe polygons.
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		glClearColor(0.16f, 0.21f, 0.22f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(tri_one_shader_prog);
		
		glBindVertexArray(vao_arr[0]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glUseProgram(tri_two_shader_prog);

		glBindVertexArray(vao_arr[1]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(2, vao_arr);
	glDeleteBuffers(2, vbo_arr);
	glDeleteProgram(tri_one_shader_prog);
	glDeleteProgram(tri_two_shader_prog);

	glfwTerminate();

	return 0;
}