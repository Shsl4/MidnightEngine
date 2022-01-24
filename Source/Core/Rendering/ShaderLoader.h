#pragma once

#include <Core/EngineTypes.h>
#include <Core/Logging/Logger.h>
#include <bgfx/bgfx.h>
#include <bx/bx.h>
#include <bx/readerwriter.h>
#include <bx/file.h>
#include <filesystem>

/*!
 * A utility class used to load BGFX shaders.
 */
class ShaderLoader {

public:

    /*!
     * Loads a ShaderProgram.
     *
     * @param[in] programName The name of the program to load.
     */
    static bgfx::ProgramHandle loadProgram(const char *programName);

    /*!
     * Loads a Shader file.
     *
     * @param[in] _name The name of the shader file to load.
     * @deprecated This function was copy/pasted from the bgfx source and needs to be written correctly for the engine.
     * @todo Rewrite the function.
     */
    static bgfx::ShaderHandle loadShader(const char *_name);

private:

    /// @deprecated This function was copy/pasted from the bgfx source and needs to be written correctly for the engine.
    static const bgfx::Memory *loadMem(bx::FileReaderI *_reader, const char *_filePath);

    /// @deprecated This function was copy/pasted from the bgfx source and needs to be written correctly for the engine.
    static bx::AllocatorI *getDefaultAllocator() {
        static bx::DefaultAllocator __allocator;
        return &__allocator;
    }

    /// @deprecated This member was copy/pasted from the bgfx source and will be removed.
    inline static bx::AllocatorI *defaultAllocator = getDefaultAllocator();
    
    /// @deprecated This member was copy/pasted from the bgfx source and will be removed.
    inline static bx::FileReaderI *fileReader = BX_NEW(defaultAllocator, bx::FileReader);

    inline static const Logger logger = Logger("ShaderLoader");

};

