#include <Rendering/Model.h>
#include <Memory/String.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

Model::Model(String const& file)
{
    String path = "Resources/Models/";
    path.append(file);
    
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path.toCString(), aiProcessPreset_TargetRealtime_Fast);

    if (scene == nullptr)
    {
        logger.error("Failed to load model. The path {} is invalid.", path);
        return;
    }
    
    for (size_t i = 0; i < scene->mNumMeshes; ++i)
    {
        modelMeshes += Mesh::from(scene->mMeshes[i]);
    }

    logger.success("Successfully loaded model {} with {} meshes.", file, scene->mNumMeshes);
    
}


