#include <Rendering/Mesh.h>
#include <Memory/String.h>
#include <sstream>
#include <fstream>

Mesh::Mesh(Array<Vertex> const &vertices, size_t numIndices, MeshType type, std::string const &name) :
        vertexCount(vertices.getSize()), indexCount(numIndices), meshType(type), meshName(name) {

    data = vertexAllocator.alloc(vertexCount);
    indices = indexAllocator.alloc(indexCount);

    for (uint16_t i = 0; i < indexCount; ++i) {
        indices[i] = (uint16_t) indexCount - 1 - i;
    }

    memcpy(data, vertices.begin(), vertexCount * sizeof(Vertex));

    vertexBuffer = createVertexBuffer(bgfx::makeRef(data, (uint32_t) vertexCount * sizeof(Vertex)), Mesh::getVertexLayout());
    indexBuffer = bgfx::createIndexBuffer(bgfx::makeRef(indices, (uint32_t) indexCount * sizeof(uint16_t)));
    programHandle = ShaderLoader::loadProgram("Advanced");

}

Mesh::~Mesh() {

    bgfx::destroy(vertexBuffer);
    bgfx::destroy(indexBuffer);
    bgfx::destroy(programHandle);
    vertexAllocator.release(data);
    indexAllocator.release(indices);

}

enum class OBJDataType {

    Comment,
    Object,
    Vertex,
    Normal,
    TexCoordinate,
    Face,
    Invalid

};

OBJDataType getDataType(String const& string) {

    if (string == "#") {
        return OBJDataType::Comment;
    }
    if (string == "o") {
        return OBJDataType::Object;
    }
    if (string == "v") {
        return OBJDataType::Vertex;
    }
    if (string == "vt") {
        return OBJDataType::TexCoordinate;
    }
    if (string == "vn") {
        return OBJDataType::Normal;
    }
    if (string == "f") {
        return OBJDataType::Face;
    }

    return OBJDataType::Invalid;

}

Mesh *MeshLoader::loadOBJ(std::string const &file) {

    // Preallocate the arrays with large sizes to avoid many reallocations later.
    auto vertexPositions = Array<Vector3>(10000);
    auto vertexNormals = Array<Vector3>(10000);
    auto vertexTexCoords = Array<Vector2>(5000);

    auto vPosIndices = Array<UInt16>(5000);
    auto vTexCoordIndices = Array<UInt16>(5000);
    auto vNormalsIndices = Array<UInt16>(5000);

    auto fstream = std::fstream("Resources/Models/" + file);

    std::string currentLine = "";
    std::string objectName = "";

    Logger::check(fstream.is_open(), "The file {} does not exist.", file);

    while (fstream.good()) {

        std::getline(fstream, currentLine);

        if (currentLine.empty()) { continue; }

        String line = currentLine;
        Array<String> elements = line.split(' ');
        OBJDataType dataType = getDataType(elements[0]);
    
        double x = elements.getSize() > 1 ? elements[1].toDouble() : 0.0;
        double y = elements.getSize() > 2 ? elements[2].toDouble() : 0.0;
        double z = elements.getSize() > 3 ? elements[3].toDouble() : 0.0;
        
        switch (dataType) {

            case OBJDataType::Vertex: {

                vertexPositions += Vector3(x, y, z);
                break;

            }

            case OBJDataType::Normal: {

                vertexNormals += Vector3(x, y, z);
                break;

            }

            case OBJDataType::TexCoordinate: {

                vertexTexCoords += Vector2(x, y);
                break;

            }

            case OBJDataType::Face: {

                size_t size = elements.getSize();

                for (size_t i = 0; i < size - 1; ++i) {

                    auto data = elements[i + 1].split('/');
                    
                    UInt16 v0 = data[0].toInteger();
                    UInt16 v1 = data[1].toInteger();
                    UInt16 v2 = data[2].toInteger();

                    vPosIndices += v0 - 1;
                    vTexCoordIndices += data[1].isEmpty() ? 0 : v1 - 1;
                    vNormalsIndices += v2 - 1;
                }

                break;

            }

            default:
                break;

        }

    }

    auto vertices = Array<Vertex>(vPosIndices.getSize());
    size_t capacity = vertices.getCapacity();

    for (size_t i = 0; i < capacity; ++i) {

        auto vtx = Vertex(vertexPositions[vPosIndices[i]],
                vertexNormals[vNormalsIndices[i]],
                vertexTexCoords[vTexCoordIndices[i]],
                LinearColors::white);

        vertices += vtx;

    }

    Mesh *mesh = instance->allocator.construct(vertices, vPosIndices.getSize(), MeshType::OBJ, objectName);

    instance->loadedMeshes += mesh;

    return mesh;

}
