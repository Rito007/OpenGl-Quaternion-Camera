#ifndef SHADER_CLASS_HEADER
#define SHADER_CLASS_HEADER
#include<glad/glad.h>
#include<iostream>
#include<string>
#include<fstream>
#include<sstream>

class Shader
{
private:
	std::string getFileContents(const char* nomeShader);
public:
	GLuint shaderID;
	Shader(const char* vertexShader, const char* fragmentShader);
	void Ativar();
	void Deletar();
	void compileErrors(unsigned int shader, const char* type);
};



#endif


