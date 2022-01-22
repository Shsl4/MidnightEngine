#pragma once

#include <Core/EngineTypes.h>
#include <Core/Logging/Logger.h>
#include <bgfx/bgfx.h>
#include <bx/bx.h>
#include <bx/readerwriter.h>
#include <bx/file.h>
#include <filesystem>

class ShaderLoader {

public:

    static bgfx::ProgramHandle loadProgram(const char *programeName);

    static bgfx::ShaderHandle loadShader(const char *_name);

private:

    static const bgfx::Memory *loadMem(bx::FileReaderI *_reader, const char *_filePath);

    inline static const Logger logger = Logger("ShaderLoader");

    static bx::AllocatorI *getDefaultAllocator() {
        static bx::DefaultAllocator __allocator;
        return &__allocator;
    }

    inline static bx::AllocatorI *defaultAllocator = getDefaultAllocator();
    inline static bx::FileReaderI *fileReader = BX_NEW(defaultAllocator, bx::FileReader);

};

