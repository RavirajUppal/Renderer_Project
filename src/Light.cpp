#include "Light.h"


void Light::SetPosition(const glm::vec3& pos) {m_Position = pos; }
const glm::vec3& Light::GetPosition() const {return m_Position; }
void Light::SetColor(const glm::vec4& color) {m_Color = color; }
const glm::vec4& Light::GetColor() const {return m_Color; }
void Light::setEnabled(bool e) { m_Enabled = e; }
bool Light::isEnabled() const { return m_Enabled; }

void Light::SetMode(Shader* shader, LightMode mode)
{
    m_Mode = mode;
    shader->Activate();
    shader->SetInt1("lightMode", static_cast<int>(mode));
}
LightMode Light::GetMode() const {return m_Mode;}

const glm::mat4& Light::GetLightSpaceMatrix()
{
    glm::mat4 ProjectionMatrix = glm::mat4(1.0f);
    glm::mat4 viewMatrix = glm::mat4(1.0f);
    if (m_Mode == LightMode::Directional)
    {
        ProjectionMatrix= glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 100.0f);
        viewMatrix = glm::lookAt(10.0f * m_Position, glm::vec3(0.0), glm::vec3(0.0f, 0.0f, -1.0f));
    }
    else if (m_Mode ==LightMode::Spot)
    {
        ProjectionMatrix = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 100.0f);
        viewMatrix = glm::lookAt(m_Position, m_Position + glm::vec3(0.0, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f));
    }
    else
    {
        ProjectionMatrix = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 100.0f);
        viewMatrix = glm::lookAt(m_Position, m_Position + glm::vec3(0.0, -10.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f));
    }
    glm::mat4 lightSpaceMatrix = ProjectionMatrix * viewMatrix;
    return lightSpaceMatrix;
}

const std::vector<glm::mat4>& Light::GetLightSpaceMatrices()
{
    glm::mat4 ProjectionMatrix = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 100.0f);
    std::vector<glm::mat4> shadowTransforms;
    shadowTransforms.push_back(ProjectionMatrix * 
                 glm::lookAt(m_Position, m_Position + glm::vec3( 1.0, 0.0, 0.0), glm::vec3(0.0,-1.0, 0.0)));
    shadowTransforms.push_back(ProjectionMatrix * 
                 glm::lookAt(m_Position, m_Position + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0,-1.0, 0.0)));
    shadowTransforms.push_back(ProjectionMatrix * 
                 glm::lookAt(m_Position, m_Position + glm::vec3( 0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
    shadowTransforms.push_back(ProjectionMatrix * 
                 glm::lookAt(m_Position, m_Position + glm::vec3( 0.0,-1.0, 0.0), glm::vec3(0.0, 0.0,-1.0)));
    shadowTransforms.push_back(ProjectionMatrix * 
                 glm::lookAt(m_Position, m_Position + glm::vec3( 0.0, 0.0, 1.0), glm::vec3(0.0,-1.0, 0.0)));
    shadowTransforms.push_back(ProjectionMatrix * 
                 glm::lookAt(m_Position, m_Position + glm::vec3( 0.0, 0.0,-1.0), glm::vec3(0.0,-1.0, 0.0)));
    return shadowTransforms;
}

// ---------------- Directional Light ----------------
DirectionalLight::DirectionalLight(const glm::vec3 &pos, const glm::vec4 &color, const glm::vec3 &dir,  float intensity)
    : Light(pos, color, intensity), m_Direction(glm::normalize(dir)) {}

void DirectionalLight::upload(Shader &shader, const std::string &name) const
{
    shader.SetVec3(name + ".color", m_Color * m_Intensity);
    shader.SetVec3(name + ".direction", m_Direction);
    shader.SetInt1(name + ".enabled", m_Enabled ? 1 : 0);
}

// ---------------- Point Light ----------------
PointLight::PointLight(const glm::vec3 &pos, const glm::vec4 &color, float intensity,
                       float constant = 1.0f, float linear = 0.09f, float quadratic = 0.032f)
    : Light(pos, color, intensity),
      m_Constant(constant), m_Linear(linear), m_Quadratic(quadratic) {}

void PointLight::upload(Shader &shader, const std::string &name) const
{
    shader.SetVec3(name + ".color", m_Color * m_Intensity);
    shader.SetVec3(name + ".position", m_Position);
    shader.SetFloat1(name + ".constant", m_Constant);
    shader.SetFloat1(name + ".linear", m_Linear);
    shader.SetFloat1(name + ".quadratic", m_Quadratic);
    shader.SetInt1(name + ".enabled", m_Enabled ? 1 : 0);
}

// ---------------- Spot Light ----------------
SpotLight::SpotLight(const glm::vec3 &pos, const glm::vec4 &color, const glm::vec3 &dir, 
                     float intensity, float cutOff, float outerCutOff)
    : Light(pos, color, intensity), m_Direction(glm::normalize(dir)),
      m_CutOff(cutOff), m_OuterCutOff(outerCutOff) {}

void SpotLight::upload(Shader &shader, const std::string &name) const
{
    shader.SetVec3(name + ".color", m_Color * m_Intensity);
    shader.SetVec3(name + ".position", m_Position);
    shader.SetVec3(name + ".direction", m_Direction);
    shader.SetFloat1(name + ".cutOff", m_CutOff);
    shader.SetFloat1(name + ".outerCutOff", m_OuterCutOff);
    shader.SetInt1(name + ".enabled", m_Enabled ? 1 : 0);
}