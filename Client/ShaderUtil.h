#pragma once

#include <string>

// Utitlity class to load, compile and attach a vertex- and fragment shader to a program
class ShaderUtil
{

private: 
	unsigned int mProgramId;

	unsigned int GetCompiledShader(unsigned int shader_type, const std::string& shader_source);

public:
	ShaderUtil() :mProgramId(0){}
	~ShaderUtil() {}

	// Load a vertex and a fragment shader from file
	bool Load(const std::string& vertexShaderFile, const std::string& fragmentShaderFile);

	// Load a vertex and a geometry and a fragment shader from file
	bool Load(const std::string & vertexShaderFile, const std::string & geometryShaderFile, const std::string & fragmentShaderFile);

	// Use the program
	void Use();

	// Delete the program
	void Delete();

	//sets the uniform location
	void setUniformLocationF(string name, float var);
};

