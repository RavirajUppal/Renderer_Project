#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

#include<glad/glad.h>
#include<string>
#include<fstream>
#include<sstream>
#include<iostream>
#include<cerrno>
#include<glm/glm.hpp>

std::string get_file_contents(const char* filename);

class Shader 
{
public:
	GLuint ID;
	Shader(const char* vertexFile, const char* fragmentFile, const char* geometryFile = nullptr);

	GLuint CompileShader(GLuint type, const char* source);
	void Activate();
	void Delete();
	void SetVec3(const std::string& name, const glm::vec3& value) const;
	void SetFloat4(const std::string& name, float x, float y, float z, float w) const;
	void SetFloat3(const std::string& name, float x, float y, float z) const;
	void SetFloat1(const std::string& name, float x) const;
	void SetInt1(const std::string& name, int x) const;
	void SetMat4(const std::string &name, const float *matrix) const;
	void SetVecOfMat4(const std::string &name, const std::vector<glm::mat4>& matrix) const;
	void PrintActiveUniforms() const;

private:
	void compileErrors(unsigned int shader, const char* type);
};

#endif
