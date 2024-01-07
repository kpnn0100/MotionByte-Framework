#include "Vertex.h"

namespace MotionByte
{
    // Implementation of Vertex constructor
    Vertex::Vertex(float x, float y) : x(x), y(y) {}

    // Implementation of VertexList member functions
    void VertexList::addVertex(float x, float y) {
        Vertex vertex(x, y);
        vertices.push_back(vertex);
    }

    size_t VertexList::size() const {
        return vertices.size();
    }

    size_t VertexList::sizeInFloat() const
    {
        return size()*2;
    }

    Vertex& VertexList::getVertex(size_t index) {
            return vertices[index];
    }
    std::vector<Vertex>& VertexList::getVertexList()
    {
        return vertices;
    }
    float* VertexList::toBufferArray() const
    {
        return (float*)vertices.data();
    }
}
