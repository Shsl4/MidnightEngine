#pragma once

#include <Math/Vector3.h>
#include <Math/Vector2.h>
#include <Rendering/LinearColor.h>

/*!
 * A vertex contains data about a point in space. Vertices are used to represent meshes.
 */
struct ENGINE_API Vertex {

public:

    Vertex() = default;
    
    Vertex(float x, float y, float z, float r, float g, float b, float a)
            : position(Vector3(x, y, z)), color(LinearColor::fromLinearRGBA(r, g, b, a)) {
    }

    Vertex(Vector3 const &pos, Vector3 const &norm, Vector2 const &texCoords, LinearColor const &col, Vector3 const& bcc)
            : position(pos), textureCoordinates(texCoords), normal(norm), color(col), barycentric(bcc) {

    }

    Vertex(float x, float y, float z, LinearColor color) : position(Vector3(x, y, z)), color(color) {

    }

    Vertex(float x, float y, float z) : position(Vector3(x, y, z)), color(LinearColors::white) {

    }
    
    bool operator==(Vertex const& other) const{
        
        return (position == other.position) && (normal == other.normal) &&
            (textureCoordinates == other.textureCoordinates) && (color == other.color);
        
    }
    
    bool operator< (Vertex const& other) const{
        
        return (position < other.position);
        
    }

    /*!
     * The position of the vertex in space.
     */
    Vector3 position = Vector3::zero;
 
    /*!
     * The vertex texture coordinates.
     */
    Vector2 textureCoordinates = Vector2::zero;
 
    /*!
     * The vertex normal.
     */
    Vector3 normal = Vector3::zero;
    
    /*!
     * The vertex color.
     */
    LinearColor color = LinearColors::white;
    
    /*!
     * The barycentric coordinates.
     */
    Vector3 barycentric = Vector3::zero;

};
