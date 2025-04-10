#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

#include<glad/glad.h>
#include<string>
#include<fstream>
#include<sstream>
#include<iostream>
#include<cerrno>
#include <unordered_map>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

std::string get_file_contents(const char* filename);

class Shader
{
public:
	GLuint ID;
	Shader(const char* vertexFile, const char* fragmentFile);

	void Activate() const;
	void Delete() const;

	GLint GetUniformLocation(const std::string& name) const;

	void SetMat4(const std::string& name, const glm::mat4& mat) const;
	void SetVec3(const std::string& name, const glm::vec3& vec) const;
	void SetFloat(const std::string& name, float value) const;

private:
	mutable std::unordered_map<std::string, GLint> uniformCache;

	void compileErrors(unsigned int shader, const char* type);
	static std::string get_file_contents(const char* filename);
};


#endif