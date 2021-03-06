#pragma once

#include <Rendering/Texture.h>

#include <Rendering/Model.h>
#include <Memory/SharedPointer.h>

class ENGINE_API ResourceLoader : public Object {

public:
    ResourceLoader() = default;

    ~ResourceLoader() override;

    void init();

    /*!
     *  \brief Gets a loaded model by name.
     *
     *  \param[in] name The name of the mesh to get.
     *  \return The mesh or nullptr if it wasn't found.
     */
    NODISCARD WeakPointer<Model> getModel(String const& name) const;

    /*!
     *  \brief Gets a loaded texture by name.
     *
     *  \param[in] name The name of the texture to get.
     *  \return The texture or nullptr if it wasn't found.
     */
    NODISCARD WeakPointer<Texture> getTexture(String const& name) const;

    NODISCARD String getRandomTexture() const;
 
    /*!
     *  \brief Opens a file in binary mode and stores all the data in an UInt8 Array.
     *
     *  \param[in] path The path of the file to load.
     *  \return An array containing the binary data.
     */
    static Array<UInt8> loadFile(String const& path);

private:
    /*!
     *  \brief Tries to load a texture from the input file.
     *
     *  \param[in] file The path of the file to load.
     */
    void loadTexture(String const& file);

    /*!
    *  \brief Tries to load a model from the input file.
    *
    *  \param[in] file The path of the file to load.
    */
    void loadModel(String const& file);

    /*!
     *  \brief An array containing every loaded mesh.
     */
    Array<SharedPointer<Model>> loadedModels = Array<SharedPointer<Model>>(100);

    /*!
    *  \brief An array containing every loaded texture.
    */
    Array<SharedPointer<Texture>> loadedTextures = Array<SharedPointer<Texture>>(100);

    WeakPointer<Texture> missingTexture;

};
