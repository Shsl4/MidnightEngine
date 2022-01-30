#pragma once

#include <Core/EngineTypes.h>
#include <Core/Logging/Logger.h>
#include <Memory/Array.h>
#include <bgfx/bgfx.h>
#include <bx/bx.h>
#include <bx/readerwriter.h>
#include <bx/file.h>
#include <filesystem>

/*!
 * A utility class used to load and compile shader files.
 */
class ShaderLoader {

public:

    /*!
     * Loads a ShaderProgram.
     *
     * \param[in] name The name of the program to load.
     */
    static bgfx::ProgramHandle loadProgram(std::string const& name);

    /*!
     * Loads a Shader file.
     *
     * \param[in] name The name of the shader file to load.
     */
    static bgfx::ShaderHandle loadShader(std::string const& name);

    /*!
     * Opens a file in binary mode and stores all the data in an UInt8 Array.
     *
     *  \param[in] path The path of the file to load.
     *  \return An array containing the binary data.
     */
    static Array<UInt8> loadFile(std::string const& path);
    
    /*!
     * Gets the shader resource path for the current renderer.
     */
    static std::string getShaderResourcePath();
    
private:

    /*!
     * The ShaderLoader Logger.
     */
    inline static const Logger logger = Logger("ShaderLoader");

};
