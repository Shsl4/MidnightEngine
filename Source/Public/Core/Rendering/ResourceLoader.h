#pragma once

#include <Rendering/Mesh.h>
#include <Logging/Logger.h>

class ENGINE_API ResourceLoader
{

public:

    ResourceLoader() = default;

    void init();

    /*!
     *  \brief Gets a loaded mesh by name.
     *
     *  \param[in] name The name of the mesh to get.
     *  \return The mesh or nullptr if it wasn't found.
     */
    NODISCARD const Mesh* getMesh(String const& name) const;

    /*!
     *  \brief Opens a file in binary mode and stores all the data in an UInt8 Array.
     *
     *  \param[in] path The path of the file to load.
     *  \return An array containing the binary data.
     */
    static Array<UInt8> loadFile(String const& path);
    
private:

    /*!
     *  \brief Tries to load a mesh from the input file.
     *
     *  \param[in] file The path of the file to load.
     *  \return Whether the mesh was loaded.
     */
    bool loadMesh(String const& file);
    
    /*!
     *  \brief An array containing every loaded mesh.
     */
    AutoReleaseArray<Mesh*> loadedMeshes = AutoReleaseArray<Mesh*>(100);

    static inline Logger logger = Logger("ResourceLoader");

};
