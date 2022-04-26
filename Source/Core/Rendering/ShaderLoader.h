#pragma once

#include <filesystem>

#include <Core/EngineTypes.h>
#include <Logging/Logger.h>
#include <Memory/Array.h>
#include <Memory/String.h>

#include <bgfx/bgfx.h>

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
    static bgfx::ProgramHandle loadProgram(String const& name);

    /*!
     * Loads a Shader file.
     *
     * \param[in] name The name of the shader file to load.
     */
    static bgfx::ShaderHandle loadShader(String const& name);

    /*!
     * Opens a file in binary mode and stores all the data in an UInt8 Array.
     *
     *  \param[in] path The path of the file to load.
     *  \return An array containing the binary data.
     */
    static Array<UInt8> loadFile(String const& path);
    
    /*!
     * Gets the shader resource path for the current renderer.
     */
    static String getShaderResourcePath();
    
private:

    /*!
     * The ShaderLoader Logger.
     */
    inline static const Logger logger = Logger("ShaderLoader");

};

