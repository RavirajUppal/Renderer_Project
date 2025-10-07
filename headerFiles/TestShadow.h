#ifndef TEST_SHADOW_CLASS_H
#define TEST_SHADOW_CLASS_H

#include "Test.h"

class TestShadow : public Test
{
public:
    TestShadow(GLFWwindow *window);
    ~TestShadow();

    void OnUpdate(float deltaTime) override;
    void OnRender() override;
    void OnImguiRender() override;
    void OnWindowResize(GLFWwindow* window, int width, int height) override;
    void RenderShadowMap(Shader* shader) override;

private:
    GLFWwindow *m_Window;
    std::unique_ptr<Shader> m_FloorShader;
    std::unique_ptr<Shader> m_LightShader;
    std::unique_ptr<Camera> m_Camera;
    std::unique_ptr<Mesh> m_Floor;
    std::unique_ptr<Mesh> m_InstancedCube;
    std::unique_ptr<Mesh> m_Cube;
    // std::unique_ptr<Shader> m_CubeInstanceShader;
    float m_LightPos[3] = {0.0f, 4.0f, 0.0f};
    void DrawScene();
};

#endif