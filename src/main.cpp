#include <iostream>
// #include <glad/glad.h>
// #include <GLFW/glfw3.h>
// #include <stb/stb_image.h>
// #define GLM_ENABLE_EXPERIMENTAL
// #include <glm/glm.hpp>
// #include <glm/gtc/matrix_transform.hpp>
// #include <glm/gtc/type_ptr.hpp>
// #include <glm/gtx/string_cast.hpp>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "Test.h"
#include "TestMenu.h"
#include "TestLight.h"
#include "TestDynamicLight.h"
#include "TestModel.h"
#include "TestCubeMap.h"
#include "TestInstance.h"
#include "TestShadow.h"

// Vertex vertices[] =
// 	{ //               COORDINATES           /            NORMALS          /           COLORS         /       TEXTURE COORDINATES    //
// 		Vertex{glm::vec3(-1.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
// 		Vertex{glm::vec3(-1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f)},
// 		Vertex{glm::vec3(1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f)},
// 		Vertex{glm::vec3(1.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f)}};

// GLuint indices[] =
// 	{
// 		0, 1, 2,
// 		0, 2, 3};

// Vertex lightVertices[] =
// 	{ //     COORDINATES     //
// 		Vertex{glm::vec3(-0.1f, -0.1f, 0.1f)},
// 		Vertex{glm::vec3(-0.1f, -0.1f, -0.1f)},
// 		Vertex{glm::vec3(0.1f, -0.1f, -0.1f)},
// 		Vertex{glm::vec3(0.1f, -0.1f, 0.1f)},
// 		Vertex{glm::vec3(-0.1f, 0.1f, 0.1f)},
// 		Vertex{glm::vec3(-0.1f, 0.1f, -0.1f)},
// 		Vertex{glm::vec3(0.1f, 0.1f, -0.1f)},
// 		Vertex{glm::vec3(0.1f, 0.1f, 0.1f)}};

// GLuint lightIndices[] =
// 	{
// 		0, 1, 2,
// 		0, 2, 3,
// 		0, 4, 7,
// 		0, 7, 3,
// 		4, 5, 7,
// 		4, 6, 7,
// 		1, 5, 6,
// 		1, 6, 2,
// 		2, 6, 7,
// 		2, 7, 3,
// 		1, 5, 4,
// 		1, 4, 0};

void GLClearError()
{
    while(glGetError());
}

void GLCheckError()
{
    while(GLenum error = glGetError())
    {
        std::cout << "error : " << error << std::endl;
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
    Test** test = static_cast<Test**>(glfwGetWindowUserPointer(window));
    if (test && *test) {
        (*test)->OnWindowResize(window, width, height); 
    }
}


int main()
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	GLFWwindow *window = glfwCreateWindow(800, 800, "JustOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	gladLoadGL();

	int framebufferWidth, framebufferHeight;
	glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);
	glViewport(0, 0, framebufferWidth, framebufferHeight);

	// Texture textures[] = {
	// 	Texture(TEXTURE_DIR "planksSpec.png", "specular", 1, GL_RED, GL_UNSIGNED_BYTE),
	// 	Texture(TEXTURE_DIR "planks.png", "diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE)};

	// Shader shaderProgram(SHADER_DIR "default.vert", SHADER_DIR "default.frag");

	// std::vector<Vertex> verts(vertices, vertices + sizeof(vertices) / sizeof(Vertex));
	// std::vector<GLuint> indi(indices, indices + sizeof(indices) / sizeof(GLuint));
	// std::vector<Texture> tex(textures, textures + sizeof(textures) / sizeof(Texture));

	// Mesh floor(verts, indi, tex);
	// floor.TexSlot(shaderProgram);

	// Shader lightShader(SHADER_DIR "light.vert", SHADER_DIR "light.frag");

	// std::vector<Vertex> lightVerts(lightVertices, lightVertices + sizeof(lightVertices) / sizeof(Vertex));
	// std::vector<GLuint> lightIndi(lightIndices, lightIndices + sizeof(lightIndices) / sizeof(GLuint));

	// Mesh light(lightVerts, lightIndi, tex);
	// light.TexSlot(lightShader);

	// glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	// glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
	// glm::mat4 lightModel = glm::mat4(1.0f);
	// lightModel = glm::translate(lightModel, lightPos);
	// lightShader.Activate();
	// glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
	// glUniform4f(glGetUniformLocation(lightShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);

	// glm::vec3 pyramidPos = glm::vec3(0.0f, -0.4f, 0.0f);
	// glm::mat4 pyramidModel = glm::mat4(1.0f);
	// pyramidModel = glm::translate(pyramidModel, pyramidPos);
	// shaderProgram.Activate();
	// glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(pyramidModel));
	// glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	// glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

	// Camera camera(framebufferWidth, framebufferHeight, glm::vec3(0.0f, 0.0f, 2.0f));


	glEnable(GL_MULTISAMPLE); 
	glEnable(GL_DEPTH_TEST);
	// glEnable(GL_CULL_FACE);
	// glCullFace(GL_FRONT);
	// glFrontFace(GL_CW);

	// glEnable(GL_BLEND);
	// glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGuiIO &io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(window, true); // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
	ImGui_ImplOpenGL3_Init();

	Test *currentTest = nullptr;
	TestMenu *testMenu = new TestMenu(window, currentTest);
	currentTest = testMenu;

	testMenu->Register<TestDynamicLight>("Dynamic Lighting");
	testMenu->Register<TestShadow>("Lighting & Shadow");
	testMenu->Register<TestModel>("Model");
	testMenu->Register<TestCubeMap>("SkyBox");
	testMenu->Register<TestInstance>("Instances");


	glfwSetWindowUserPointer(window, &currentTest);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	

	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.11f, 0.13f, 0.14f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();

		if (currentTest)
		{
			currentTest->OnUpdate(0.0f);
			currentTest->OnRender();

			ImGui::NewFrame();
			ImGui::Begin("Test Options");

			if (currentTest != testMenu && ImGui::Button(" <- "))
			{
				delete currentTest;
				currentTest = testMenu;
			}

			currentTest->OnImguiRender();
			ImGui::End();
		}

		// glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);
		// glViewport(0, 0, framebufferWidth, framebufferHeight);

		// camera.Inputs(window);
		// camera.UpdateMatrix(60.0f, 0.1f, 100.0f, framebufferWidth, framebufferHeight);

		// floor.Draw(shaderProgram, camera);

		// light.Draw(lightShader, camera);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	// shaderProgram.Delete();
	// lightShader.Delete();

	if (currentTest != testMenu)
		delete currentTest;
	delete testMenu;

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
