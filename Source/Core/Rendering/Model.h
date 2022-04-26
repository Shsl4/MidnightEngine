#pragma once

#include <Rendering/ShaderLoader.h>
#include <Memory/Array.h>
#include <Rendering/Mesh.h>

struct Model
{

    explicit Model(String const &file);
    
    AutoReleaseArray<Mesh *> modelMeshes;
    
    static inline Logger logger = Logger("ModelLoader");
    
};
