#pragma once

#include <Rendering/Mesh.h>
#include <assimp/Importer.hpp>

class ENGINE_API ResourceLoader
{

public:

    ResourceLoader() = default;

    void init();

    /*!
    * Gets a loaded mesh by name.
    *
    * \param[in] name The name of the mesh to get.
    * \return The mesh or nullptr if it wasn't found.
    */
    const Mesh* getMesh(String const& name) const;

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

    /*!
    *  Tries to load a mesh from the input file.
    *
    *  \param[in] path The path of the file to load.
    *  \return Whether the mesh was loaded.
    */
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

    /*!
    * An array containing every loaded mesh.
    */
    AutoReleaseArray<Mesh*> loadedMeshes = AutoReleaseArray<Mesh*>(100);

    /*!
    * The importer used to import assets.
    */
    Assimp::Importer importer;

    static inline Logger logger = Logger("ResourceLoader");

};
