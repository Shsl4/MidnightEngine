#pragma once

#include <Core/EngineMacros.h>
#include <bgfx/bgfx.h>

#include <Memory/String.h>

class ENGINE_INTERNAL ShaderManager
{
    
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
     * Gets the shader resource path for the current renderer.
    */
    static String getShaderResourcePath();

    
};
