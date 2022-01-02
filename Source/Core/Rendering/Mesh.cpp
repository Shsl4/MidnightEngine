#include <Rendering/Mesh.h>
#include <vector>
#include <sstream>
#include <fstream>

class String{
    
public:
    
    static Array<std::string> split(std::string str, const char separator){
        
        Array<std::string> arr;
        int from = 0;
        int to = 0;
        const unsigned int size = str.size();
        for (int i = 0; i < size; ++i) {
            
            if(str[i] == separator){
                
                to = i;
                arr += std::string(&str[from], &str[to]);
                from = to + 1;
                
            }
            
            
        }
        
        if(from == to + 1){
            to = size;
            arr += std::string(&str[from], &str[to]);
        }

        return arr;
        
    }
    
};

enum class OBJDataType{
    
    Comment,
    Object,
    Vertex,
    Normal,
    TexCoordinate,
    Face,
    Invalid
    
};

OBJDataType getDataType(std::string string) {
    
    if(string == "#"){
        return OBJDataType::Comment;
    }
    if(string == "o"){
        return OBJDataType::Object;
    }
    if(string == "v"){
        return OBJDataType::Vertex;
    }
    if(string == "vt"){
        return OBJDataType::TexCoordinate;
    }
    if(string == "vn"){
        return OBJDataType::Normal;
    }
    if(string == "f"){
        return OBJDataType::Face;
    }
    
    return OBJDataType::Invalid;
    
}

Mesh* MeshLoader::loadOBJ(std::string const& file){
    
    Array<Vector3> vertexPositions;
    Array<Vector3> vertexNormals;
    Array<Vector2> vertexTexCoords;

    Array<unsigned short> vPosIndices;
    Array<unsigned short> vTexCoordIndices;
    Array<unsigned short> vNormalsIndices;
    
    Array<Vertex> vertices;
    
    std::fstream fstream = std::fstream("Resources/Models/" + file);
    std::string currentLine = "";
    
    std::string objectName = "";
    
    if(fstream.is_open()){
        
        while (fstream.good()) {
            
            std::getline(fstream, currentLine);
            
            if(currentLine.empty()) { continue; }
            
            Array<std::string> elements = String::split(currentLine, ' ');
            
            OBJDataType dataType = getDataType(elements[0]);
            
            switch (dataType) {
                    
                case OBJDataType::Invalid:
                case OBJDataType::Comment:
                case OBJDataType::Object:
                    break;
                    
                case OBJDataType::Vertex: {
                    
                    vertexPositions += Vector3(std::stof(elements[1]), std::stof(elements[2]), std::stof(elements[3]));
                    break;
                    
                }
                    
                case OBJDataType::Normal:{
                    
                    vertexNormals += Vector3(std::stof(elements[1]), std::stof(elements[2]), std::stof(elements[3]));
                    break;
                    
                }
                    
                case OBJDataType::TexCoordinate: {
                    
                    vertexTexCoords += Vector2(std::stof(elements[1]), std::stof(elements[2]));
                    break;
                    
                }
                    
                case OBJDataType::Face: {
                    
                    for(int i = 0; i < elements.getSize() - 1; ++i){
                        
                        Array<std::string> data = String::split(elements[i + 1], '/');
                        vPosIndices += std::stoi(data[0]) - 1;
                        vTexCoordIndices += data[1].empty() ? 0 : std::stoi(data[1]) - 1;
                        vNormalsIndices += std::stoi(data[2]) - 1;

                    }

                    break;
                    
                }
                    
            }
            
        }
        
    }
    else{
        
        Logger::check(false, "The file {} does not exist.", file);

    }
    
    vertices.resize(vPosIndices.getSize());
    
    for (int i = 0; i < vertices.getCapacity(); ++i){
        
        Vertex vtx = Vertex(vertexPositions[vPosIndices[i]],
                            vertexNormals[vNormalsIndices[i]],
                            vertexTexCoords[vTexCoordIndices[i]],
                            LinearColors::white);
 
        vertices += vtx;
        
    }
    
    Mesh* mesh = Allocator().instantiate<Mesh>(vertices, vPosIndices.getSize(), MeshType::OBJ, objectName);
    
    loadedMeshes += mesh;
    
    return mesh;
        
}
