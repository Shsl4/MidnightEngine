#pragma once

#include <Core/EngineTypes.h>
#include <Core/Logging/Logger.h>
#include <bgfx/bgfx.h>
#include <bx/bx.h>
#include <bx/readerwriter.h>
#include <bx/file.h>
#include <filesystem>

class ShaderLoader
{

public:

	static bgfx::ProgramHandle loadProgram(const char* programeName);

	static bgfx::ShaderHandle loadShader(const char* _name);

private:

	static const bgfx::Memory* loadMem(bx::FileReaderI* _reader, const char* _filePath);

	inline static const Logger logger = Logger("ShaderLoader");

	class FileReader : public bx::FileReader
	{
		typedef bx::FileReader super;

	public:
		virtual bool open(const bx::FilePath& _filePath, bx::Error* _err) override
		{
			std::string filePath = fmt::format("{}/{}", std::filesystem::current_path().string(), _filePath.getCPtr());
			return super::open(filePath.c_str(), _err);
		}
	};

	static bx::AllocatorI* getDefaultAllocator()
	{
		static bx::DefaultAllocator __allocator;
		return &__allocator;
	}

	inline static bx::AllocatorI* defaultAllocator = getDefaultAllocator();
	inline static bx::FileReaderI* fileReader = BX_NEW(defaultAllocator, FileReader);

};

