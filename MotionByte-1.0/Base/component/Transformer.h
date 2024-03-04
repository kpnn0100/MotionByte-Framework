#pragma once
#include "../Graphic/Vertex.h"  // Include the VertexList class
#include "../../Functional/GraphicMath.h"
namespace MotionByte
{
    class Transformer {
    public:
        virtual ~Transformer() {}

        // Transform the provided VertexList
        virtual void transform(VertexList& vertexList) = 0;
    };
}
