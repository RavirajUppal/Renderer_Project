#ifndef TEST_INSTANCE_H
#define TEST_INSTANCE_H

#include "Test.h"
#include "Model.h"

class TestInstance : public Test
{
public:
    TestInstance(GLFWwindow *window);
    ~TestInstance();

    void OnUpdate(float deltaTime) override;
    void OnRender() override;
    void OnImguiRender() override;
    void OnWindowResize(GLFWwindow* window, int width, int height) override;

private:
    GLFWwindow *m_Window;
    std::unique_ptr<Model> m_Model;
    std::unique_ptr<Shader> m_ShaderProgram;
    std::unique_ptr<Camera> m_Camera;
    void DrawModel();
};

#endif
