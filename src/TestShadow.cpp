#include "TestShadow.h"

namespace
{
    std::vector<Vertex> GenerateCube(float size = 1.0f)
    {
        std::vector<Vertex> vertices;
        vertices.reserve(24); // 6 faces * 4 vertices

        // Directions for cube faces
        glm::vec3 normals[6] = {
            {0, 0, 1},  // Front
            {0, 0, -1}, // Back
            {-1, 0, 0}, // Left
            {1, 0, 0},  // Right
            {0, 1, 0},  // Top
            {0, -1, 0}  // Bottom
        };

        // Each face defined in CCW order
        glm::vec3 faceCoords[6][4] = {
            {{-1, -1, 1}, {1, -1, 1}, {1, 1, 1}, {-1, 1, 1}},     // Front
            {{-1, -1, -1}, {-1, 1, -1}, {1, 1, -1}, {1, -1, -1}}, // Back
            {{-1, -1, -1}, {-1, -1, 1}, {-1, 1, 1}, {-1, 1, -1}}, // Left
            {{1, -1, -1}, {1, 1, -1}, {1, 1, 1}, {1, -1, 1}},     // Right
            {{-1, 1, -1}, {-1, 1, 1}, {1, 1, 1}, {1, 1, -1}},     // Top
            {{-1, -1, -1}, {1, -1, -1}, {1, -1, 1}, {-1, -1, 1}}  // Bottom
        };

        // Texture coordinates (same for all faces)
        glm::vec2 uvs[4] = {
            {0.0f, 0.0f},
            {1.0f, 0.0f},
            {1.0f, 1.0f},
            {0.0f, 1.0f}};

        for (int f = 0; f < 6; f++)
        { // 6 faces
            for (int v = 0; v < 4; v++)
            { // 4 vertices per face
                glm::vec3 pos = faceCoords[f][v] * size;
                vertices.push_back(Vertex{pos, normals[f], glm::vec3(1.0f), uvs[v]});
            }
        }

        return vertices;
    }

    std::vector<GLuint> GenerateCubeIndices()
    {
        std::vector<GLuint> indices;
        indices.reserve(36);

        for (int f = 0; f < 6; f++)
        {
            GLuint start = f * 4;
            indices.insert(indices.end(), {start, start + 1, start + 2,
                                           start, start + 2, start + 3});
        }

        return indices;
    }

    std::vector<glm::vec3> cubePositions = {
        {0.0f, 2.0f, 0.0f},
        {2.0f, 0.5f, -3.0f},
        {-1.5f, 1.0f, -2.5f},
        {3.0f, 2.5f, -1.0f}};
}

TestShadow::TestShadow(GLFWwindow *window) : Test(window), m_Window(window)
{
    // m_Shadow = true;
    Vertex floorVertices[] =
        {//               COORDINATES           /            NORMALS          /           COLORS         /       TEXTURE COORDINATES    //
         Vertex{glm::vec3(-5.0f, 0.0f, 5.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
         Vertex{glm::vec3(-5.0f, 0.0f, -5.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f)},
         Vertex{glm::vec3(5.0f, 0.0f, -5.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f)},
         Vertex{glm::vec3(5.0f, 0.0f, 5.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f)}};

    GLuint floorIndices[] =
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

    std::vector<Vertex> floorVerts(floorVertices, floorVertices + sizeof(floorVertices) / sizeof(Vertex));
    std::vector<GLuint> floorIndi(floorIndices, floorIndices + sizeof(floorIndices) / sizeof(GLuint));
    std::vector<Texture> floorTex(textures, textures + sizeof(textures) / sizeof(Texture));

    // std::vector<Vertex> lightVerts(lightVertices, lightVertices + sizeof(lightVertices) / sizeof(Vertex));
    // std::vector<GLuint> lightIndi(lightIndices, lightIndices + sizeof(lightIndices) / sizeof(GLuint));

    std::vector<Vertex> lightVerts = GenerateCube(0.1f);
    std::vector<GLuint> lightIndi = GenerateCubeIndices();

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
    m_FloorShader->SetFloat1("farPlane", 100.0f);

    m_Light->SetMode(m_FloorShader.get(), LightMode::Point);
    m_Floor = std::make_unique<Mesh>(floorVerts, floorIndi, floorTex);

    //Camera
    m_Camera = std::make_unique<Camera>(FrameHeight, FrameHeight, glm::vec3(0.0f, 4.0f, 10.0f), glm::vec3(0.0f, -0.5f, -1.0f));

    //Cubes
    std::vector<Vertex> CubeVerts = GenerateCube(0.2f);
    std::vector<GLuint> CubeIndi = GenerateCubeIndices();

    // m_CubeInstanceShader = std::make_unique<Shader>(SHADER_DIR "instance.vert", SHADER_DIR "default.frag");
    // m_CubeInstanceShader->Activate();
    // m_CubeInstanceShader->SetFloat4("lightColor", lightColor.x, lightColor.y, lightColor.z, lightColor.w);
    // m_CubeInstanceShader->SetFloat3("lightPos", lightPos.x, lightPos.y, lightPos.z);
    // m_CubeInstanceShader->SetInt1("lightMode", 0);

    // Create instance transforms
    std::vector<glm::mat4> instanceMatrices;
    for (int i = 0; i < 5; i++)
    {
        // Random X, Y, Z in range [-5, 5]
        float x = (std::rand() % 1000 / 1000.0f) * 8.0f - 4.0f;
        float y = (std::rand() % 1000 / 1000.0f) * 2.0f; // a bit smaller range for height
        float z = (std::rand() % 1000 / 1000.0f) * 8.0f - 4.0f;

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(x, y, z));

        // Optional: random rotation
        float angle = (std::rand() % 360);
        model = glm::rotate(model, glm::radians(angle), glm::vec3(0.3f, 1.0f, 0.5f));

        // Optional: random scale
        float scale = 0.2f + (std::rand() % 100 / 100.0f); // [0.5, 1.5]
        model = glm::scale(model, glm::vec3(scale));

        instanceMatrices.push_back(model);
    }

    // Create instanced Mesh (uses the constructor with instanceMatrices)
    m_InstancedCube = std::make_unique<Mesh>(CubeVerts, CubeIndi, std::vector<Texture>{}, instanceMatrices.size(), instanceMatrices);

    m_Cube = std::make_unique<Mesh>(CubeVerts, CubeIndi, floorTex);

}

TestShadow::~TestShadow()
{
}

void TestShadow::OnUpdate(float deltaTime)
{
    m_Camera->Inputs(m_Window);
    m_Camera->UpdateMatrix(60.0f, 0.1f, 100.0f);
}

void TestShadow::OnRender()
{
    Test::BindPostProcessingFrameBuffer();
    DrawScene();
    Test::DrawPostProcessingOnScreen();
}

void TestShadow::DrawScene()
{
    glm::vec3 lightPos = m_Light->GetPosition();
    glm::mat4 lightModel = glm::mat4(1.0f);
    lightModel = glm::translate(lightModel, lightPos);
    m_Light->m_Mesh->Draw(*m_LightShader, *m_Camera, lightModel);

    m_FloorShader->Activate();
    m_FloorShader->SetMat4("lightSpaceMatrix", glm::value_ptr(m_Light->GetLightSpaceMatrix()));
    m_FloorShader->SetFloat3("lightPos", lightPos.x, lightPos.y, lightPos.z);
    m_FloorShader->SetInt1("useShadow", m_Shadow);
    if (m_Shadow){
        if (m_Light->GetMode() == LightMode::Point){
            glActiveTexture(GL_TEXTURE0 + 3);
            glBindTexture(GL_TEXTURE_CUBE_MAP, m_ShadowCubeMap);
        }
        else{
            glActiveTexture(GL_TEXTURE0 + 2);
            glBindTexture(GL_TEXTURE_2D, m_ShadowMap);
        }
    }
    m_Floor->Draw(*m_FloorShader, *m_Camera);

    // m_CubeInstanceShader->Activate();
    // m_CubeInstanceShader->SetFloat3("lightPos", lightPos.x, lightPos.y, lightPos.z);
    // m_InstancedCube->Draw(*m_CubeInstanceShader, *m_Camera);

    for (const auto& pos : cubePositions) {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, pos);
        m_Cube->Draw(*m_FloorShader, *m_Camera, model);
    }
}

void TestShadow::RenderShadowMap(Shader* shader)
{
    m_Floor->Draw(*shader, *m_Camera);
    for (const auto& pos : cubePositions) 
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, pos);
        m_Cube->Draw(*shader, *m_Camera, model);
    }
}

void TestShadow::OnImguiRender()
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

void TestShadow::OnWindowResize(GLFWwindow *window, int width, int height)
{
    Test::OnWindowResize(window, width, height);
    m_Camera->OnWindowResize(width, height);
}
