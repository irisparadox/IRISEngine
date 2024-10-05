#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

const char* vertexShaderSource = "#version 460 core\n"
								 "layout (location = 0) in vec3 aPos;\n"
								 "layout (location = 1) in vec3 colorInput;\n"
								 "out vec3 the_color;\n"
								 "void main() {\n"
								 "	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
								 "	the_color = colorInput;\n"
								 "}\0";

const char* tri_one_frag_source = "#version 460 core\n"
								  "out vec4 FragColor;\n"
								  "in vec3 the_color;\n"
								  "void main() {\n"
								  "	FragColor = vec4(the_color, 1.0);\n"
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

	unsigned int tri_one_frag;
	tri_one_frag = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(tri_one_frag, 1, &tri_one_frag_source, NULL);
	glCompileShader(tri_one_frag);

	glGetShaderiv(tri_one_frag, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(tri_one_frag, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << '\n';
	}

	unsigned int tri_one_shader_prog;
	tri_one_shader_prog = glCreateProgram();

	glAttachShader(tri_one_shader_prog, vertexShader);
	glAttachShader(tri_one_shader_prog, tri_one_frag);
	glLinkProgram(tri_one_shader_prog);

	glGetProgramiv(tri_one_shader_prog, GL_LINK_STATUS, &success);

	if (!success) {
		glGetProgramInfoLog(tri_one_shader_prog, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << '\n';
	}

	glDeleteShader(vertexShader);
	glDeleteShader(tri_one_frag);

	float tri_one_verts[] = {
		 0.0f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f, // A | Acolor
		-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f, // B | Bcolor
		 0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f  // C | Ccolor
	};

	unsigned int vbo_arr, vao_arr;
	glGenVertexArrays(1, &vao_arr);
	glGenBuffers(1, &vbo_arr);

	glBindVertexArray(vao_arr);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_arr);
	glBufferData(GL_ARRAY_BUFFER, sizeof(tri_one_verts), tri_one_verts, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glUseProgram(tri_one_shader_prog);

	// uncomment this call to draw in wireframe polygons.
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		glClearColor(0.16f, 0.21f, 0.22f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		glBindVertexArray(vao_arr);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &vao_arr);
	glDeleteBuffers(1, &vbo_arr);
	glDeleteProgram(tri_one_shader_prog);

	glfwTerminate();

	return 0;
}