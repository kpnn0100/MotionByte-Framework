#pragma once

#include <vector>
#include <array>
#include <cstddef>
#include "util/geometry/Vertex.h"

namespace mb {

struct Node {
    double i;                // vertex index (may be fractional per Python adaptation)
    double x;
    double y;
    Node* prev = nullptr;
    Node* next = nullptr;
    double z = 0.0;
    Node* prevZ = nullptr;
    Node* nextZ = nullptr;
    bool steiner = false;
};

struct FlattenResult {
    std::vector<double> vertices;
    std::vector<size_t> holes;
    int dimensions = 2;
};

/**
 * Earcut polygon triangulation
 * @param coords flat array [x0,y0,x1,y1,...]
 * @param holeIndices indices (in vertex units) where each hole starts
 * @param dim coordinate dimension (2 or 3; only x,y used)
 * @return list of vertex indices (as int, converted from internal double representation)
 */
std::vector<int> earcut(const std::vector<double>& data,
                        const std::vector<size_t>* holeIndices = nullptr,
                        int dim = 2);

/**
 * Calculate deviation between original polygon and triangulated result
 */
double deviation(const std::vector<double>& data,
                 const std::vector<size_t>* holeIndices,
                 int dim,
                 const std::vector<double>& triangles);

/**
 * Flatten multi-ring polygon data structure
 */
FlattenResult flatten(const std::vector<std::vector<std::array<double,2>>>& multiRing);

} // namespace mb

namespace MotionByte {

/**
 * Wrapper function for earcut triangulation using VertexList
 * @param outerRings Vector of outer boundary vertices (multiple polygons)
 * @param holes Vector of hole vertices (can be empty)
 * @return VertexList containing triangulated vertices, each 3 consecutive vertices form a triangle
 */
VertexList triangulate(const std::vector<VertexList>& outerRings, 
                       const std::vector<VertexList>& holes = {});

} // namespace MotionByte
