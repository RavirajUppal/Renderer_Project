#ifndef TEST_CLASS_H
#define TEST_CLASS_H

#include "Mesh.h"
#include "Light.h"
#include "imgui/imgui.h"

class Test
{
public:
    Test(GLFWwindow *window);
    virtual ~Test();

    virtual void OnUpdate(float deltaTime);
    virtual void OnRender();
    virtual void OnImguiRender();
    virtual void OnWindowResize(GLFWwindow* window, int width, int height);
    virtual void RenderShadowMap(Shader* shader) {};

    void SetFrameBufferData();
    void BindPostProcessingFrameBuffer();
    void DrawPostProcessingOnScreen();
    
protected:
    int FrameWidth, FrameHeight;
    bool m_Shadow = false;
    unsigned int m_ShadowMap;
    unsigned int m_ShadowCubeMap;
    std::unique_ptr<Light> m_Light;
    
private:
    int m_ShadowWidth = 1024, m_ShadowHeight = 1024;
    bool m_MSAA = false, m_PostProcessing = false;
    VAO m_FrameBufferVAO;
    int m_CurrentPostProcess = 0;
    unsigned int m_MultisamplingFBO, m_PostProcessingFBO, m_FrameBufferTex;
    unsigned int m_ShadowMapFBO;
    unsigned int m_ShadowCubeMapFBO;
    std::unique_ptr<Shader> m_PostProcessShader;
    std::unique_ptr<Shader> m_ShadowMapShader;
    std::unique_ptr<Shader> m_ShadowCubeMapShader;
    std::unique_ptr<Shader> m_DebugOutputShader;
    void RenderShadowPass();
    void ConfigureShadowShader();
};

#endif
