#include "Shader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cassert>
#include <bitset>
#include "BufferMap.h"

#define FNV_OFFSET_BASIS (0xcbf29ce484222325ULL)
#define FNV_PRIME (0x100000001b3ULL)

uint64_t hashString(const std::string & strToHash)
{
	uint64_t hash = FNV_OFFSET_BASIS;
	for (char e : strToHash)
	{
		hash ^= e;
		hash *= FNV_PRIME;
	}

	return hash;
}

Shader::Shader(const GLchar * vertexPath, const GLchar * fragmentPath, const GLchar * geoPath)
{
	std::string vertCode = readShader(vertexPath);
	std::string fragCode = readShader(fragmentPath);
	std::string geoCode = readShader(fragmentPath);

	uint64_t ui64ShaderHash = hashString(vertCode + fragCode + geoCode);

	static BufferMap bmShaders;

	id = bmShaders[ui64ShaderHash];
	if (id)
		return;

	std::vector<unsigned int> programs;
	programs.push_back(compileShader(vertCode.c_str(), GL_VERTEX_SHADER));
	programs.push_back(compileShader(fragCode.c_str(), GL_FRAGMENT_SHADER));
	programs.push_back(compileShader(geoCode.c_str(), GL_GEOMETRY_SHADER));

	id = linkShader(programs);
	bmShaders.insert(ui64ShaderHash, id);
}

Shader::Shader(const GLchar * vertexPath, const GLchar * fragmentPath)
{
	std::string vertCode = readShader(vertexPath);
	std::string fragCode = readShader(fragmentPath);

	uint64_t ui64ShaderHash = hashString(vertCode + fragCode);

	static BufferMap bmShaders;

	id = bmShaders[ui64ShaderHash];
	if (id)
		return;

	std::vector<unsigned int> programs;
	programs.push_back(compileShader(vertCode.c_str(), GL_VERTEX_SHADER));
	programs.push_back(compileShader(fragCode.c_str(), GL_FRAGMENT_SHADER));

	id = linkShader(programs);
	bmShaders.insert(ui64ShaderHash, id);
}

std::string attributeToStr(Attribute type)
{
	switch (type)
	{
	case POSITION:
		return "Position";

	case COLOR:
		return "Color";

	case TEXTURE:
		return "Texture";

	case NORMAL:
		return "Normal";

	case SPECULAR:
		return "Specular";

	default:
		return "InvalidAttribute" + std::to_string(type);

	}
}

std::string genVertexShader(const std::array<unsigned int, LAST> & aDimensions, bool bHasLighting)
{
	std::stringstream strStream;

	strStream << "#version 330 core" << std::endl;

	for(unsigned int i = 0; i < aDimensions.size(); ++i)
	{
		if (aDimensions[i] > 0)
		{
			switch (i)
			{
			case POSITION:
				strStream << "layout (location = 0) in vec3 inPos;" << std::endl;
				break;

			case COLOR:
				strStream << "layout (location = 1) in vec4 inColor;" << std::endl;
				strStream << "out vec4 color;" << std::endl;
				break;

			case TEXTURE:
				strStream << "layout (location = 2) in vec2 inTextureCord;" << std::endl;
				strStream << "out vec2 TextureCord;" << std::endl;
				break;
			}

			strStream << std::endl;
		}
	}

	strStream << "void main()" << std::endl << "{" << std::endl;
	for (unsigned int i = 0; i < aDimensions.size(); ++i)
	{
		if (aDimensions[i] > 0)
		{
			switch (i)
			{
			case POSITION:
				strStream << "\tgl_Position = vec4(inPos, 1.0);" << std::endl;
				break;

			case COLOR:
				strStream << "\tcolor = inColor;" << std::endl;
				break;

			case TEXTURE:
				strStream << "\tTextureCord = inTextureCord;" << std::endl;
				break;
			}
		}
	}
	strStream << "}";

	return strStream.str();
}

std::string genFragmentShader(const std::array<unsigned int, LAST> & aDimensions, bool bHasLighting)
{
	std::stringstream fragStream;

	fragStream << "#version 330 core" << std::endl;

	if (aDimensions[0] && !aDimensions[1] && !aDimensions[2])
	{
		fragStream << "uniform vec4 color;" << std::endl;
	}

	for (unsigned int i = 0; i < aDimensions.size(); ++i)
	{
		if (aDimensions[i] > 0)
		{
			switch (i)
			{
			case COLOR:
				fragStream << "in vec4 color;" << std::endl;
				break;

			case TEXTURE:
				fragStream << "in vec2 TextureCord;" << std::endl;
				fragStream << "uniform sampler2D Texture;" << std::endl;
				break;
			}

			fragStream << std::endl;
		}
	}

	fragStream << std::endl << "out vec4 FragColor;" << std::endl << std::endl;

	fragStream << "void main()" << std::endl << "{" << std::endl;
	if (aDimensions[COLOR] && aDimensions[TEXTURE])
	{
		fragStream << "\tFragColor = texture(Texture, TextureCord) * color;" << std::endl;
	}
	else if (aDimensions[TEXTURE])
	{
		fragStream << "\tFragColor = texture(Texture, TextureCord);" << std::endl;
	}
	else
	{
		fragStream << "\tFragColor = color;" << std::endl;
	}
	fragStream << "}";

	return fragStream.str();
}

Shader::Shader(const std::array<unsigned int, LAST> & aDimensions, bool bHasLighting)
{
	// calculate a hash of the shader using an using passed data
	uint64_t shaderHash = 0;

	for (auto e : aDimensions)
	{
		assert(e <= 4 && "Dimension too large.");
		shaderHash <<= 3;
		shaderHash += e;
	}

	shaderHash <<= 1;
	if (bHasLighting)
		shaderHash += 1;

	//std::cout << "Shader Hash:" << std::bitset<32>(shaderHash) << std::endl;

	// use hash map to check if previously used shader
	static BufferMap generatedShaders;

	id = generatedShaders[shaderHash];
	if (id)
	{
		//std::cout << "Using ID: " << id << std::endl;
		return;
	}


	std::string strVertexShader = genVertexShader(aDimensions, bHasLighting);

	//std::cout << strVertexShader << std::endl;

	std::string strFragShader = genFragmentShader(aDimensions, bHasLighting);

	//std::cout << strFragShader << std::endl;

	std::vector<unsigned int> programs;
	programs.push_back(compileShader(strVertexShader.c_str(), GL_VERTEX_SHADER));
	programs.push_back(compileShader(strFragShader.c_str(), GL_FRAGMENT_SHADER));

	id = linkShader(programs);

	// add id to the shader map
	generatedShaders.insert(shaderHash, id);
	//std::cout << "Created ID: " << id << std::endl;
}

std::string Shader::readShader(const std::string & path)
{
	std::string code;
	std::ifstream file;

	file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		file.open(path);

		std::stringstream stream, fragStream;
		stream << file.rdbuf();

		file.close();

		code = stream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << path << " not successfully read." << std::endl;
	}

	return code;
}

unsigned int Shader::compileShader(const char * code, GLenum type)
{
	unsigned int shaderID = glCreateShader(type);
	int success;
	char infoLog[512];

	glShaderSource(shaderID, 1, &code, NULL);
	glCompileShader(shaderID);
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shaderID, 512, NULL, infoLog);
		switch (type)
		{
		case GL_VERTEX_SHADER:
			std::cout << "Vertex Shader Compilation Failed\n" << infoLog << std::endl;
			break;
		case GL_FRAGMENT_SHADER:
			std::cout << "Fragment Shader Compilation Failed\n" << infoLog << std::endl;
			break;
		case GL_GEOMETRY_SHADER:
			std::cout << "Geometry Shader Compilation Failed\n" << infoLog << std::endl;
			break;
		}

	}

	return shaderID;
}

unsigned int Shader::linkShader(const std::vector<unsigned int> & programs)
{
	int success;
	char infoLog[512];
	unsigned int uid;
	uid = glCreateProgram();
	for (unsigned int e : programs)
		glAttachShader(uid, e);


	glLinkProgram(uid);
	glGetProgramiv(uid, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(uid, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	for (unsigned int e : programs)
		glDeleteShader(e);

	return uid;
}
