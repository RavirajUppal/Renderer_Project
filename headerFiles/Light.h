#ifndef LIGHT_H
#define LIGHT_H

#include"Mesh.h"

enum class LightMode
{
    Directional = 0, Point = 1, Spot = 2
};


class Light
{
public:
    Light(const glm::vec3& pos = glm::vec3(0.0f, 10.0f, 1.0f), const glm::vec4& color = glm::vec4(1.0f), float intensity = 1.0f)
        : m_Position(pos), m_Color(color), m_Intensity(intensity), m_Enabled(true) {};

    virtual ~Light() = default;

    virtual void upload(Shader& shader, const std::string& uniformName) const {};

    LightMode m_Mode;

    void SetPosition(const glm::vec3& pos);
    const glm::vec3& GetPosition() const;

    void SetColor(const glm::vec4& color);
    const glm::vec4& GetColor() const;

    void setEnabled(bool e);
    bool isEnabled() const;

    void SetMode(Shader* shader, LightMode mode);
    LightMode GetMode() const;

    const glm::mat4 GetLightSpaceMatrix();
    const std::vector<glm::mat4> GetLightMatricesForCubeMap();

    std::unique_ptr<Mesh> m_Mesh;

protected:
    glm::vec3 m_Position;
    glm::vec4 m_Color;
    float m_Intensity;
    bool m_Enabled;
    std::unique_ptr<Shader> m_Shader;
};


// ---------------- Directional Light ----------------
class DirectionalLight : public Light {
public:
    DirectionalLight(const glm::vec3& pos, const glm::vec4& color, const glm::vec3& dir, float intensity);
    void upload(Shader& shader, const std::string& name) const override;

private:
    glm::vec3 m_Direction;
};


// ---------------- Point Light ----------------
class PointLight : public Light {
public:
    PointLight(const glm::vec3& pos, const glm::vec4& color, float intensity,
               float constant, float linear, float quadratic);

    void upload(Shader& shader, const std::string& name) const override;

private:
    float m_Constant, m_Linear, m_Quadratic;
};


// ---------------- Spot Light ----------------
class SpotLight : public Light {
public:
    SpotLight(const glm::vec3& pos, const glm::vec4& color, const glm::vec3& dir, 
              float intensity, float cutOff, float outerCutOff);

    void upload(Shader& shader, const std::string& name) const override;

private:
    glm::vec3 m_Direction;
    float m_CutOff, m_OuterCutOff;
};

#endif