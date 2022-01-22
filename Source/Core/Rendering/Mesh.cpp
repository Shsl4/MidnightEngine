#include <Rendering/Mesh.h>
#include <sstream>
#include <fstream>

Mesh::Mesh(Array<Vertex> const &vertices, size_t numIndices, MeshType type, std::string const &name) :
        vertexCount(vertices.getSize()), indexCount(numIndices), meshType(type), meshName(name) {

    data = allocator.allocate<Vertex>(vertexCount);
    indices = allocator.allocate<uint16_t>(indexCount);

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
    allocator.deallocate(data);
    allocator.deallocate(indices);

}

struct String {

public:

    static Array<std::string> split(std::string str, const char separator) {

        Array<std::string> arr;

        size_t from = 0;
        size_t to = 0;

        const size_t size = str.size();

        for (size_t i = 0; i < size; ++i) {

            if (str[i] == separator) {
                to = i;
                arr += std::string(&str[from], &str[to]);
                from = to + 1;
            }


        }

        if (from == to + 1) {
            to = size;
            arr += std::string(&str[from], &str[to]);
        }

        return arr;

    }

};

enum class OBJDataType {

    Comment,
    Object,
    Vertex,
    Normal,
    TexCoordinate,
    Face,
    Invalid

};

OBJDataType getDataType(std::string_view string) {

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

    Array<Vector3> vertexPositions;
    Array<Vector3> vertexNormals;
    Array<Vector2> vertexTexCoords;

    Array<UInt16> vPosIndices;
    Array<UInt16> vTexCoordIndices;
    Array<UInt16> vNormalsIndices;

    Array<Vertex> vertices;

    auto fstream = std::fstream("Resources/Models/" + file);

    std::string currentLine = "";
    std::string objectName = "";

    Logger::check(fstream.is_open(), "The file {} does not exist.", file);

    while (fstream.good()) {

        std::getline(fstream, currentLine);

        if (currentLine.empty()) {continue;}

        Array<std::string> elements = String::split(currentLine, ' ');
        OBJDataType dataType = getDataType(elements[0]);

        switch (dataType) {

            case OBJDataType::Vertex: {

                vertexPositions += Vector3(std::stof(elements[1]), std::stof(elements[2]), std::stof(elements[3]));
                break;

            }

            case OBJDataType::Normal: {

                vertexNormals += Vector3(std::stof(elements[1]), std::stof(elements[2]), std::stof(elements[3]));
                break;

            }

            case OBJDataType::TexCoordinate: {

                vertexTexCoords += Vector2(std::stof(elements[1]), std::stof(elements[2]));
                break;

            }

            case OBJDataType::Face: {

                size_t size = elements.getSize();

                for (size_t i = 0; i < size - 1; ++i) {

                    auto data = String::split(elements[i + 1], '/');
                    vPosIndices += std::stoi(data[0]) - 1;
                    vTexCoordIndices += data[1].empty() ? 0 : std::stoi(data[1]) - 1;
                    vNormalsIndices += std::stoi(data[2]) - 1;

                }

                break;

            }

            default:
                break;

        }

    }

    vertices.resize(vPosIndices.getSize());

    size_t capacity = vertices.getCapacity();

    for (size_t i = 0; i < capacity; ++i) {

        auto vtx = Vertex(vertexPositions[vPosIndices[i]],
                vertexNormals[vNormalsIndices[i]],
                vertexTexCoords[vTexCoordIndices[i]],
                LinearColors::white);

        vertices += vtx;

    }

    Mesh *mesh = instance->allocator.instantiate<Mesh>(vertices, vPosIndices.getSize(), MeshType::OBJ, objectName);

    instance->loadedMeshes += mesh;

    return mesh;

}
