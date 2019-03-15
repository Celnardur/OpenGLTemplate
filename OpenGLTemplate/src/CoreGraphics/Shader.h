#ifndef SHADER_H
#define SHADER_H
// Custom Class to handle shaders based off of code written and copyrighted by Joey de Vries
// https://learnopengl.com/Getting-started/Shaders
// https://learnopengl.com/code_viewer_gh.php?code=includes/learnopengl/shader_s.h
// https://creativecommons.org/licenses/by-nc/4.0/legalcode
// Code is free to modify and use. 
//
// Modified by Aaron Dorrance

#include <GraphicsIncludes.h>
#include <string>

class Shader
{
public:
	// the program ID
	unsigned int id;

	// constructor reads and builds the shader
	Shader() = default;
	Shader(const GLchar * vertexPath, const GLchar * fragmentPath, const GLchar * geoPath);
	Shader(const GLchar * vertexPath, const GLchar * fragmentPath);

	// use/activate the shader
	void use();
	// utility uniform functions
	void setUniform(const std::string &name, float one);
	void setUniform(const std::string &name, float one, float two);
	void setUniform(const std::string &name, float one, float two, float three);
	void setUniform(const std::string &name, float one, float two, float three, float four);

	void setUniform(const std::string &name, glm::vec3 data);
	
	void setUniformI(const std::string &name, int one);

	void setUniform(const std::string &name, glm::mat4 transform);
	
};

inline void Shader::use()
{
	glUseProgram(id);
}

inline void Shader::setUniform(const std::string& name, float one)
{
	glUniform1f(glGetUniformLocation(id, name.c_str()), one);
}

inline void Shader::setUniform(const std::string& name, float one, float two)
{
	glUniform2f(glGetUniformLocation(id, name.c_str()), one, two);
}

inline void Shader::setUniform(const std::string& name, float one, float two, float three)
{
	glUniform3f(glGetUniformLocation(id, name.c_str()), one, two, three);
}

inline void Shader::setUniform(const std::string& name, float one, float two, float three, float four)
{
	glUniform4f(glGetUniformLocation(id, name.c_str()), one, two, three, four);
}

inline void Shader::setUniform(const std::string& name, glm::vec3 data)
{
	glUniform3f(glGetUniformLocation(id, name.c_str()), data.x, data.y, data.z);
}


inline void Shader::setUniformI(const std::string& name, int one)
{
	glUniform1i(glGetUniformLocation(id, name.c_str()), one);
}

inline void Shader::setUniform(const std::string &name, glm::mat4 transform)
{
	glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, glm::value_ptr(transform));
}

#endif
