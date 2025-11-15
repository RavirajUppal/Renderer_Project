#include "TestInstance.h"

float randf()
{
	return -1.0f + (rand() / (RAND_MAX / 2.0f));
}

TestInstance::TestInstance(GLFWwindow *window) : Test(window), m_Window(window)
{
    glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    glfwGetFramebufferSize(window, &FrameWidth, &FrameHeight);
	glViewport(0, 0, FrameWidth, FrameHeight);

    // The number of asteroids to be created
	const unsigned int amount = 5000;
	// Radius of circle around which asteroids orbit
	float radius = 100.0f;
	// How much ateroids deviate from the radius
	float radiusDeviation = 25.0f;

	// Holds all transformations for the asteroids
	std::vector <glm::mat4> instanceMatrix;

	for (unsigned int i = 0; i < amount; i++)
	{
		// Generates x and y for the function x^2 + y^2 = radius^2 which is a circle
		float x = randf();
		float finalRadius = radius + randf() * radiusDeviation;
		float y = ((rand() % 2) * 2 - 1) * sqrt(1.0f - x * x);

		// Holds transformations before multiplying them
		glm::vec3 tempTranslation;
		glm::quat tempRotation;
		glm::vec3 tempScale;

		// Makes the random distribution more even
		if (randf() > 0.5f)
		{
			// Generates a translation near a circle of radius "radius"
			tempTranslation = glm::vec3(y * finalRadius, randf(), x * finalRadius);
		}
		else
		{
			// Generates a translation near a circle of radius "radius"
			tempTranslation = glm::vec3(x * finalRadius, randf(), y * finalRadius);
		}
		// Generates random rotations
		tempRotation = glm::quat(1.0f, randf(), randf(), randf());
		// Generates random scales
		tempScale = 0.1f * glm::vec3(randf(), randf(), randf());


		// Initialize matrices
		glm::mat4 trans = glm::mat4(1.0f);
		glm::mat4 rot = glm::mat4(1.0f);
		glm::mat4 sca = glm::mat4(1.0f);

		// Transform the matrices to their correct form
		trans = glm::translate(trans, tempTranslation);
		rot = glm::mat4_cast(tempRotation);
		sca = glm::scale(sca, tempScale);

		// Push matrix transformation
		instanceMatrix.push_back(trans * rot * sca);
	}

    // unsigned int amount = 1000;
    // std::vector<glm::mat4> instanceMatrix;
    // srand(glfwGetTime()); // initialize random seed	
    // float radius = 50.0;
    // float offset = 2.5f;
    // for(unsigned int i = 0; i < amount; i++)
    // {
    //     glm::mat4 model = glm::mat4(1.0f);
    //     // 1. translation: displace along circle with 'radius' in range [-offset, offset]
    //     float angle = (float)i / (float)amount * 360.0f;
    //     float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
    //     float x = sin(angle) * radius + displacement;
    //     displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
    //     float y = displacement * 0.4f; // keep height of field smaller compared to width of x and z
    //     displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
    //     float z = cos(angle) * radius + displacement;
    //     model = glm::translate(model, glm::vec3(x, y, z));

    //     // 2. scale: scale between 0.05 and 0.25f
    //     float scale = (rand() % 20) / 100.0f + 0.05;
    //     model = glm::scale(model, glm::vec3(scale));

    //     // 3. rotation: add random rotation around a (semi)randomly picked rotation axis vector
    //     float rotAngle = (rand() % 360);
    //     model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

    //     // 4. now add to list of matrices
    //     instanceMatrix.push_back(model);
    // } 


    glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(0.0f, 0.5f, 0.5f);
    m_Light->SetColor(lightColor);
    m_Light->SetPosition(lightPos);

    m_ShaderProgram = std::make_unique<Shader>(SHADER_DIR "instance.vert", SHADER_DIR "default.frag");

	m_ShaderProgram->Activate();
	glUniform4f(glGetUniformLocation(m_ShaderProgram->ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(m_ShaderProgram->ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
	m_Light->SetMode(m_ShaderProgram.get(), LightMode::Directional);
    m_ShaderProgram->SetInt1("useShadow", 0);
	m_ShaderProgram->SetInt1("shadowMap", 2);
    m_ShaderProgram->SetInt1("shadowCubeMap", 3);

	m_Camera = std::make_unique<Camera>(FrameHeight, FrameHeight, glm::vec3(0.0f, 30.0f, 150.0f), glm::vec3(0.0f, -0.2f, -1.0f));

    // m_Model = std::make_unique<Model>("resources/Models/grindstone/scene.gltf");
    // m_Model = std::make_unique<Model>("resources/Models/sword/scene.gltf", amount, instanceMatrix);
    m_Model = std::make_unique<Model>("resources/Models/asteroid/scene.gltf", amount, instanceMatrix);
    // m_Model = std::make_unique<Model>("resources/Models/airplane/scene.gltf", amount, instanceMatrix);
    // m_Model = std::make_unique<Model>("resources/Models/statue/scene.gltf", amount, instanceMatrix);
    // m_Model = std::make_unique<Model>("resources/Models/crow/scene.gltf", amount, instanceMatrix);
}

TestInstance::~TestInstance()
{
}

void TestInstance::OnUpdate(float deltaTime)
{
    m_Camera->Inputs(m_Window);
	m_Camera->UpdateMatrix(60.0f, 0.1f, 500.0f);
}

void TestInstance::OnRender()
{
    Test::BindPostProcessingFrameBuffer();
    DrawModel();
    Test::DrawPostProcessingOnScreen();
}

void TestInstance::DrawModel()
{
    m_Model->Draw(*m_ShaderProgram, *m_Camera);
}

void TestInstance::OnImguiRender()
{
    Test::OnImguiRender();
}

void TestInstance::OnWindowResize(GLFWwindow *window, int width, int height)
{
    Test::OnWindowResize(window, width, height);
    m_Camera->OnWindowResize(width, height);
}
