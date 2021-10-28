#include <Rendering/ShaderProgram.h>
#include <Rendering/Vertex.h>
#include <fstream>
#include <filesystem>
#include <Utilities/FileHelper.h>
#include <exception>

ShaderProgram::ShaderProgram(std::string programName)
{

	int linkSuccessful = 0;
	int valid = 0;
	char log[1024];

	this->programId = glCreateProgram();

	if (!programId) {
		logger->fatal("Failed to create shader program!");
		throw new std::runtime_error("Failed to create shader program!");
	}

	// Load our vertex and fragment shader codes
	std::string vertexCode = FileHelper::getFileContents("Resources/Shaders/Vertex/" + programName + ".vert");
	std::string fragmentCode = FileHelper::getFileContents("Resources/Shaders/Fragment/" + programName + ".frag");

	if (vertexCode.empty() || fragmentCode.empty()) {
		throw new std::runtime_error("Failed to load shader code! Is the path valid?");
	}

	// Create vertex and fragment shaders
	this->vertexId = makeShader(vertexCode, GL_VERTEX_SHADER);
	this->fragmentId = makeShader(fragmentCode, GL_FRAGMENT_SHADER);

	if(!vertexId || !fragmentId) {
		throw new std::runtime_error("Failed to create shader program!");
	}

	glLinkProgram(programId);
	glGetProgramiv(programId, GL_LINK_STATUS, &linkSuccessful);
	
	if (!linkSuccessful) {
		glGetProgramInfoLog(programId, 1024, NULL, log);
		logger->fatal("Failed to link shader! Error: {}", log);
		throw new std::runtime_error("Failed to create shader program!");
	}

	glValidateProgram(programId);
	glGetProgramiv(programId, GL_VALIDATE_STATUS, &valid);

	if (!valid) {
		glGetProgramInfoLog(programId, 1024, NULL, log);
		logger->fatal("Shader validation failed! Error: {}", log);
		throw new std::runtime_error("Failed to create shader program!");
	}

	glDetachShader(programId, vertexId);
	glDetachShader(programId, fragmentId);

    int pos = glGetAttribLocation(programId, "position");
    int col = glGetAttribLocation(programId, "inColor");

	glEnableVertexAttribArray(pos);
	glVertexAttribPointer(pos, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);

	glEnableVertexAttribArray(col);
	glVertexAttribPointer(col, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));

}

ShaderProgram::~ShaderProgram()
{
	unbind();
	if (programId) {
		glDeleteProgram(programId);
	}
}

int ShaderProgram::makeShader(std::string source, int type)
{

	int shaderId = glCreateShader(type);

	if (!shaderId) {
		logger->fatal("Failed to create shader! Type: {}", type == GL_VERTEX_SHADER ? "Vertex shader" : "Fragment shader");
		return 0;
	}

	const char* src = source.c_str();
	int compileSuccessful = 0;
	char log[1024];

	glShaderSource(shaderId, 1, &src, nullptr);
	glCompileShader(shaderId);
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &compileSuccessful);

	if (!compileSuccessful) {
		glGetShaderInfoLog(shaderId, 1024, NULL, log);
		logger->fatal("Failed to compile shader! Type: {}. Error: {}", type == GL_VERTEX_SHADER ? "Vertex shader" : "Fragment shader", log);
		return 0;
	}

	glAttachShader(programId, shaderId);

	return shaderId;

}

void ShaderProgram::bind() 
{
	glUseProgram(this->programId);
}

void ShaderProgram::unbind()
{
	glUseProgram(0);
}
