#pragma once
#include "util/geometry/Vertex.h"  // Include the VertexList class
#include "util/functional/GraphicMath.h"
namespace MotionByte
{
    class Transformer {
    public:
        virtual ~Transformer() {}

        // Transform the provided VertexList
        virtual void transform(VertexList& vertexList) = 0;
    };
}
