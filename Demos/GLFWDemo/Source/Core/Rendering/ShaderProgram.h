#pragma once

#include <Core/EngineTypes.h>
#include <Core/Logging/Logger.h>
#include <memory>

class ShaderProgram
{

public:

	ShaderProgram(std::string programName);
	~ShaderProgram();

	void bind();
	void unbind();
	FORCEINLINE int getProgram() { return this->programId; }

private:


	int programId;
	int vertexId;
	int fragmentId;

	int makeShader(std::string source, int type);

	inline static const UniquePtr<Logger> logger = std::make_unique<Logger>("ShaderBuilder");

};

