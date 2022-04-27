#pragma once

#include <Rendering/Mesh.h>

class ResourceLoader
{

public:
    
    ResourceLoader();

    Mesh* getMesh(String const& name) const;

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
    
private:
    
    bool loadMesh(String const& file);

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

    AutoReleaseArray<Mesh*> loadedMeshes = AutoReleaseArray<Mesh*>(100);
    
    static inline Logger logger = Logger("ResourceLoader");

};
