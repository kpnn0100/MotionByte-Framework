#pragma once
#include <iostream>
#include <vector>

namespace MotionByte
{
    class Vertex {
    public:
        float x, y;

        Vertex(float x, float y);

        // You can add more functionality or member functions here if needed
    };

    class VertexList {
    private:
        std::vector<Vertex> vertices;

    public:
        // Add a vertex to the list
        void addVertex(float x, float y);

        // Get the number of vertices in the list
        size_t size() const;
        size_t sizeInFloat() const;
        // Access a vertex by index
        Vertex& getVertex(size_t index);
        std::vector<Vertex>& getVertexList();
        float* toBufferArray() const;

    };
}
