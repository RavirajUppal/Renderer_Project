#include "TestDynamicLight.h"

TestDynamicLight::TestDynamicLight(GLFWwindow *window) : Test(window), m_Window(window)
{
    Vertex vertices[] =
        {//               COORDINATES           /            NORMALS          /           COLORS         /       TEXTURE COORDINATES    //
         Vertex{glm::vec3(-5.0f, 0.0f, 5.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
         Vertex{glm::vec3(-5.0f, 0.0f, -5.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f)},
         Vertex{glm::vec3(5.0f, 0.0f, -5.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f)},
         Vertex{glm::vec3(5.0f, 0.0f, 5.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f)}};

    GLuint indices[] =
        {
            0, 1, 2,
            0, 2, 3};

    Vertex lightVertices[] =
        {//     COORDINATES     //
         Vertex{glm::vec3(-0.1f, -0.1f, 0.1f)},
         Vertex{glm::vec3(-0.1f, -0.1f, -0.1f)},
         Vertex{glm::vec3(0.1f, -0.1f, -0.1f)},
         Vertex{glm::vec3(0.1f, -0.1f, 0.1f)},
         Vertex{glm::vec3(-0.1f, 0.1f, 0.1f)},
         Vertex{glm::vec3(-0.1f, 0.1f, -0.1f)},
         Vertex{glm::vec3(0.1f, 0.1f, -0.1f)},
         Vertex{glm::vec3(0.1f, 0.1f, 0.1f)}};

    GLuint lightIndices[] =
        {
            0, 1, 2,
            0, 2, 3,
            0, 4, 7,
            0, 7, 3,
            4, 5, 7,
            4, 6, 7,
            1, 5, 6,
            1, 6, 2,
            2, 6, 7,
            2, 7, 3,
            1, 5, 4,
            1, 4, 0};

    Texture textures[] = {
        Texture(TEXTURE_DIR "planks.png", "diffuse", 0),
        Texture(TEXTURE_DIR "planksSpec.png", "specular", 1)};

    glfwGetFramebufferSize(window, &FrameWidth, &FrameHeight);
    glViewport(0, 0, FrameWidth, FrameHeight);

    std::vector<Vertex> verts(vertices, vertices + sizeof(vertices) / sizeof(Vertex));
    std::vector<GLuint> indi(indices, indices + sizeof(indices) / sizeof(GLuint));
    std::vector<Texture> tex(textures, textures + sizeof(textures) / sizeof(Texture));

    std::vector<Vertex> lightVerts(lightVertices, lightVertices + sizeof(lightVertices) / sizeof(Vertex));
    std::vector<GLuint> lightIndi(lightIndices, lightIndices + sizeof(lightIndices) / sizeof(GLuint));


    //Light
    m_LightShader = std::make_unique<Shader>(SHADER_DIR "light.vert", SHADER_DIR "light.frag");
    glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    glm::vec3 lightPos = glm::vec3(m_LightPos[0], m_LightPos[1], m_LightPos[2]);
    m_Light = std::make_unique<Light>(lightPos, lightColor);
    m_Light->m_Mesh = std::make_unique<Mesh>(lightVerts, lightIndi);
    m_LightShader->Activate();
    m_LightShader->SetFloat4("lightColor", lightColor.x, lightColor.y, lightColor.z, lightColor.w);

    //Floor
    m_FloorShader = std::make_unique<Shader>(SHADER_DIR "default.vert", SHADER_DIR "default.frag", SHADER_DIR "default.geom");
    m_FloorShader->Activate();
    m_FloorShader->SetFloat4("lightColor", lightColor.x, lightColor.y, lightColor.z, lightColor.w);
    m_FloorShader->SetFloat3("lightPos", lightPos.x, lightPos.y, lightPos.z);
    m_FloorShader->SetInt1("shadowMap", 2);
    m_FloorShader->SetInt1("shadowCubeMap", 3);
    m_FloorShader->SetInt1("useShadow", 0);
    m_FloorShader->SetFloat1("farPlane", 100.0f);
    m_Light->SetMode(m_FloorShader.get(), LightMode::Point);
    m_Floor = std::make_unique<Mesh>(verts, indi, tex);

    //Camera
    m_Camera = std::make_unique<Camera>(FrameHeight, FrameHeight, glm::vec3(0.0f, 2.0f, 3.0f), glm::vec3(0.0f, -0.5f, -1.0f));
}

TestDynamicLight::~TestDynamicLight()
{
}

void TestDynamicLight::OnUpdate(float deltaTime)
{
    // float time = glfwGetTime();
    // glm::vec3 lightPos = glm::vec3(1.0f * sin(time), 1.0f, 1.0f * cos(time));

    m_Camera->Inputs(m_Window);
    m_Camera->UpdateMatrix(60.0f, 0.1f, 100.0f);
}

void TestDynamicLight::OnRender()
{
    Test::BindPostProcessingFrameBuffer();
    DrawScene();
    Test::DrawPostProcessingOnScreen();
}

void TestDynamicLight::DrawScene()
{
    glm::vec3 lightPos = m_Light->GetPosition();
    glm::mat4 lightModel = glm::mat4(1.0f);
    lightModel = glm::translate(lightModel, lightPos);
    m_Light->m_Mesh->Draw(*m_LightShader, *m_Camera, lightModel);

    m_FloorShader->Activate();
    m_FloorShader->SetMat4("lightSpaceMatrix", glm::value_ptr(m_Light->GetLightSpaceMatrix()));
    m_FloorShader->SetFloat3("lightPos", lightPos.x, lightPos.y, lightPos.z);

    m_Floor->Draw(*m_FloorShader, *m_Camera);
}

void TestDynamicLight::OnImguiRender()
{
    if (ImGui::Button("Directional Light"))
    {
        m_Light->SetMode(m_FloorShader.get(), LightMode::Directional);
    }
    if (ImGui::Button("Point Light"))
    {
        m_Light->SetMode(m_FloorShader.get(), LightMode::Point);
    }
    if (ImGui::Button("Spot Light"))
    {
        m_Light->SetMode(m_FloorShader.get(), LightMode::Spot);
    }
    if (ImGui::DragFloat3("Position", m_LightPos, 0.1f, -5.0f, 5.0f))
    {
        m_Light->SetPosition(glm::vec3(m_LightPos[0], m_LightPos[1], m_LightPos[2]));
    }
    Test::OnImguiRender();
}

void TestDynamicLight::OnWindowResize(GLFWwindow *window, int width, int height)
{
    Test::OnWindowResize(window, width, height);
    m_Camera->OnWindowResize(width, height);
}
