/**
 * using implementation of earcut algorithm from https://github.com/mapbox/earcut
 * The implementation is adapted for C++ and integrated into the FontManager class.
 */

#include "Triangulation.h"
#include <cmath>
#include <algorithm>
#include <limits>
#include <functional>
#include <iostream>

namespace mb {

namespace detail {

inline Node* createNode(double i, double x, double y, std::vector<Node*>& pool) {
    pool.push_back(new Node{ i, x, y });
    return pool.back();
}

inline double signedArea(const std::vector<double>& data, int start, int end, int dim) {
    double s = 0.0;
    int j = end - dim;
    for (int i = start; i < end; i += dim) {
        s += (data[j] - data[i]) * (data[i + 1] + data[j + 1]);
        j = i;
    }
    return s;
}

inline double area(Node* p, Node* q, Node* r) {
    return (q->y - p->y) * (r->x - q->x) - (q->x - p->x) * (r->y - q->y);
}

inline bool equals(Node* a, Node* b) {
    return a->x == b->x && a->y == b->y;
}

inline void removeNode(Node* p) {
    p->next->prev = p->prev;
    p->prev->next = p->next;
    if (p->prevZ) p->prevZ->nextZ = p->nextZ;
    if (p->nextZ) p->nextZ->prevZ = p->prevZ;
}

inline Node* insertNode(double i, double x, double y, Node* last, std::vector<Node*>& pool) {
    Node* p = createNode(i, x, y, pool);
    if (!last) {
        p->prev = p;
        p->next = p;
    } else {
        p->next = last->next;
        p->prev = last;
        last->next->prev = p;
        last->next = p;
    }
    return p;
}

inline Node* linkedList(const std::vector<double>& data, int start, int end, int dim, bool clockwise, std::vector<Node*>& pool) {
    Node* last = nullptr;
    bool isClockwise = signedArea(data, start, end, dim) > 0;
    if (clockwise == isClockwise) {
        for (int i = start; i < end; i += dim) {
            int baseIndex = i / dim;
            double x = data[i];
            double y = data[i + 1];
            double fractional_index = baseIndex + std::fmod(x, 1.0) * 0.1 + std::fmod(y, 1.0) * 0.01;
            last = insertNode(fractional_index, x, y, last, pool);
        }
    } else {
        for (int i = end - dim; i >= start; i -= dim) {
            int baseIndex = i / dim;
            double x = data[i];
            double y = data[i + 1];
            double fractional_index = baseIndex + std::fmod(x, 1.0) * 0.1 + std::fmod(y, 1.0) * 0.01;
            last = insertNode(fractional_index, x, y, last, pool);
        }
    }
    if (last && equals(last, last->next)) {
        removeNode(last);
        last = last->next;
    }
    return last;
}

inline int sign(double v) {
    return v > 0 ? 1 : (v < 0 ? -1 : 0);
}

inline bool onSegment(Node* p, Node* q, Node* r) {
    return q->x <= std::max(p->x, r->x) && q->x >= std::min(p->x, r->x) &&
           q->y <= std::max(p->y, r->y) && q->y >= std::min(p->y, r->y);
}

inline bool intersects(Node* p1, Node* q1, Node* p2, Node* q2) {
    int o1 = sign(area(p1, q1, p2));
    int o2 = sign(area(p1, q1, q2));
    int o3 = sign(area(p2, q2, p1));
    int o4 = sign(area(p2, q2, q1));
    if (o1 != o2 && o3 != o4) return true;
    if (o1 == 0 && onSegment(p1, p2, q1)) return true;
    if (o2 == 0 && onSegment(p1, q2, q1)) return true;
    if (o3 == 0 && onSegment(p2, p1, q2)) return true;
    if (o4 == 0 && onSegment(p2, q1, q2)) return true;
    return false;
}

inline bool pointInTriangle(double ax,double ay,double bx,double by,double cx,double cy,double px,double py) {
    return ( (cx - px)*(ay - py) >= (ax - px)*(cy - py) &&
             (ax - px)*(by - py) >= (bx - px)*(ay - py) &&
             (bx - px)*(cy - py) >= (cx - px)*(by - py) );
}

inline bool pointInTriangleExceptFirst(double ax,double ay,double bx,double by,double cx,double cy,double px,double py) {
    if (ax == px && ay == py) return false;
    return pointInTriangle(ax,ay,bx,by,cx,cy,px,py);
}

inline Node* filterPoints(Node* start, Node* end = nullptr) {
    if (!start) return start;
    if (!end) end = start;
    Node* p = start;
    bool again = true;
    while (again || p != end) {
        again = false;
        if (!p->steiner && (equals(p, p->next) || area(p->prev, p, p->next) == 0)) {
            Node* n = p->next;
            removeNode(p);
            p = end = p->prev;
            if (p == p->next) break;
            p = n;
            again = true;
        } else {
            p = p->next;
        }
    }
    return end;
}

inline bool locallyInside(Node* a, Node* b) {
    if (area(a->prev, a, a->next) < 0)
        return area(a, b, a->next) >= 0 && area(a, a->prev, b) >= 0;
    return area(a, b, a->prev) < 0 || area(a, a->next, b) < 0;
}

inline bool middleInside(Node* a, Node* b) {
    double px = (a->x + b->x) / 2.0;
    double py = (a->y + b->y) / 2.0;
    bool inside = false;
    Node* p = a;
    do {
        bool cond = ((p->y > py) != (p->next->y > py)) &&
                    (p->next->y != p->y) &&
                    (px < (p->next->x - p->x) * (py - p->y) / (p->next->y - p->y) + p->x);
        if (cond) inside = !inside;
        p = p->next;
    } while (p != a);
    return inside;
}

inline bool intersectsPolygon(Node* a, Node* b) {
    Node* p = a;
    do {
        if (p->i != a->i && p->next->i != a->i &&
            p->i != b->i && p->next->i != b->i &&
            intersects(p, p->next, a, b)) {
            return true;
        }
        p = p->next;
    } while (p != a);
    return false;
}

inline bool isValidDiagonal(Node* a, Node* b) {
    if (a->next->i == b->i || a->prev->i == b->i) return false;
    if (intersectsPolygon(a, b)) return false;
    if (locallyInside(a, b) && locallyInside(b, a) && middleInside(a, b) &&
        (area(a->prev, a, b->prev) || area(a, b->prev, b)))
        return true;
    if (equals(a, b) && area(a->prev, a, a->next) > 0 && area(b->prev, b, b->next) > 0)
        return true;
    return false;
}

inline Node* splitPolygon(Node* a, Node* b, std::vector<Node*>& pool) {
    Node* a2 = createNode(a->i, a->x, a->y, pool);
    Node* b2 = createNode(b->i, b->x, b->y, pool);
    Node* an = a->next;
    Node* bp = b->prev;

    a->next = b;
    b->prev = a;

    a2->next = an;
    an->prev = a2;

    b2->next = a2;
    a2->prev = b2;

    bp->next = b2;
    b2->prev = bp;

    return b2;
}

inline Node* getLeftmost(Node* start) {
    Node* p = start;
    Node* leftmost = start;
    do {
        if (p->x < leftmost->x || (p->x == leftmost->x && p->y < leftmost->y))
            leftmost = p;
        p = p->next;
    } while (p != start);
    return leftmost;
}

inline bool sectorContainsSector(Node* m, Node* p) {
    return area(m->prev, m, p->prev) < 0 && area(p->next, m, m->next) < 0;
}

inline bool pointInTriangleX(double ax,double ay,double bx,double by,double cx,double cy,double px,double py) {
    return pointInTriangle(ax,ay,bx,by,cx,cy,px,py);
}

inline Node* findHoleBridge(Node* hole, Node* outer) {
    Node* p = outer;
    double hx = hole->x, hy = hole->y;
    double qx = -std::numeric_limits<double>::infinity();
    Node* m = nullptr;
    if (equals(hole, p)) return p;
    do {
        if (equals(hole, p->next)) return p->next;
        if (hy <= p->y && hy >= p->next->y && p->next->y != p->y) {
            double x = p->x + (hy - p->y) * (p->next->x - p->x) / (p->next->y - p->y);
            if (x <= hx && x > qx) {
                qx = x;
                m = p->x < p->next->x ? p : p->next;
                if (x == hx) return m;
            }
        }
        p = p->next;
    } while (p != outer);
    if (!m) return nullptr;

    Node* stop = m;
    double mx = m->x, my = m->y;
    double tanMin = std::numeric_limits<double>::infinity();
    p = m;
    do {
        if (hx >= p->x && p->x >= mx && hx != p->x &&
            pointInTriangleX(hy < my ? hx : qx, hy, mx, my, hy < my ? qx : hx, hy, p->x, p->y)) {
            double tanv = std::fabs(hy - p->y) / (hx - p->x);
            if (locallyInside(p, hole) &&
                (tanv < tanMin || (tanv == tanMin && (p->x > m->x || (p->x == m->x && sectorContainsSector(m, p))))))
            {
                m = p;
                tanMin = tanv;
            }
        }
        p = p->next;
    } while (p != stop);
    return m;
}

inline Node* eliminateHole(Node* hole, Node* outer, std::vector<Node*>& /*pool*/) {
    Node* bridge = findHoleBridge(hole, outer);
    if (!bridge) return outer;
    Node* bridgeReverse = splitPolygon(bridge, hole, *(new std::vector<Node*>())); // leak avoided: we won't use this pool
    (void)bridgeReverse;
    return outer;
}

inline Node* eliminateHoles(const std::vector<double>& data,
                            const std::vector<size_t>& holeIndices,
                            Node* outer,
                            int dim,
                            std::vector<Node*>& pool)
{
    std::vector<Node*> queue;
    for (size_t i = 0; i < holeIndices.size(); ++i) {
        int start = static_cast<int>(holeIndices[i]) * dim;
        int end = (i + 1 < holeIndices.size()) ? static_cast<int>(holeIndices[i + 1]) * dim : static_cast<int>(data.size());
        Node* list = linkedList(data, start, end, dim, false, pool);
        if (list == list->next) list->steiner = true;
        queue.push_back(getLeftmost(list));
    }
    std::sort(queue.begin(), queue.end(), [](Node* a, Node* b) {
        if (a->x != b->x) return a->x < b->x;
        if (a->y != b->y) return a->y < b->y;
        double aSlope = (a->next->x - a->x) != 0 ? (a->next->y - a->y)/(a->next->x - a->x) : std::numeric_limits<double>::infinity();
        double bSlope = (b->next->x - b->x) != 0 ? (b->next->y - b->y)/(b->next->x - b->x) : std::numeric_limits<double>::infinity();
        return aSlope < bSlope;
    });

    for (Node* h : queue)
        outer = eliminateHole(h, outer, pool);
    return outer;
}

inline unsigned int zOrder(double x, double y, double minX, double minY, double invSize) {
    unsigned int ix = invSize ? (unsigned int)((x - minX) * invSize) : 0;
    unsigned int iy = invSize ? (unsigned int)((y - minY) * invSize) : 0;

    ix = (ix | (ix << 8)) & 0x00FF00FF;
    ix = (ix | (ix << 4)) & 0x0F0F0F0F;
    ix = (ix | (ix << 2)) & 0x33333333;
    ix = (ix | (ix << 1)) & 0x55555555;
    iy = (iy | (iy << 8)) & 0x00FF00FF;
    iy = (iy | (iy << 4)) & 0x0F0F0F0F;
    iy = (iy | (iy << 2)) & 0x33333333;
    iy = (iy | (iy << 1)) & 0x55555555;
    return ix | (iy << 1);
}

inline Node* sortLinked(Node* list) {
    if (!list) return list;
    int inSize = 1;
    for (;;) {
        Node* p = list;
        list = nullptr;
        Node* tail = nullptr;
        int numMerges = 0;
        while (p) {
            numMerges++;
            Node* q = p;
            int pSize = 0;
            for (int i = 0; i < inSize; ++i) {
                pSize++;
                q = q->nextZ;
                if (!q) break;
            }
            int qSize = inSize;
            while (pSize > 0 || (qSize > 0 && q)) {
                Node* e = nullptr;
                if (pSize != 0 && (qSize == 0 || !q || p->z <= q->z)) {
                    e = p;
                    p = p->nextZ;
                    pSize--;
                } else {
                    e = q;
                    q = q->nextZ;
                    qSize--;
                }
                if (tail) tail->nextZ = e;
                else list = e;
                e->prevZ = tail;
                tail = e;
            }
            p = q;
        }
        tail->nextZ = nullptr;
        inSize *= 2;
        if (numMerges <= 1)
            return list;
    }
}

inline void indexCurve(Node* start, double minX, double minY, double invSize) {
    Node* p = start;
    do {
        if (p->z == 0)
            p->z = (double)zOrder(p->x, p->y, minX, minY, invSize);
        p->prevZ = p->prev;
        p->nextZ = p->next;
        p = p->next;
    } while (p != start);
    p = start;
    p->prevZ->nextZ = nullptr;
    p->prevZ = nullptr;
    sortLinked(p);
}

inline bool isEar(Node* ear) {
    Node* a = ear->prev;
    Node* b = ear;
    Node* c = ear->next;
    if (area(a, b, c) >= 0) return false;

    double ax = a->x, ay = a->y;
    double bx = b->x, by = b->y;
    double cx = c->x, cy = c->y;
    double x0 = std::min({ ax, bx, cx });
    double y0 = std::min({ ay, by, cy });
    double x1 = std::max({ ax, bx, cx });
    double y1 = std::max({ ay, by, cy });

    Node* p = c->next;
    while (p != a) {
        if (p->x >= x0 && p->x <= x1 && p->y >= y0 && p->y <= y1 &&
            pointInTriangleExceptFirst(ax, ay, bx, by, cx, cy, p->x, p->y) &&
            area(p->prev, p, p->next) >= 0)
            return false;
        p = p->next;
    }
    return true;
}

inline bool isEarHashed(Node* ear, double minX, double minY, double invSize) {
    Node* a = ear->prev;
    Node* b = ear;
    Node* c = ear->next;
    if (area(a, b, c) >= 0) return false;

    double ax = a->x, ay = a->y;
    double bx = b->x, by = b->y;
    double cx = c->x, cy = c->y;
    double x0 = std::min({ ax, bx, cx });
    double y0 = std::min({ ay, by, cy });
    double x1 = std::max({ ax, bx, cx });
    double y1 = std::max({ ay, by, cy });

    unsigned int minZ = zOrder(x0, y0, minX, minY, invSize);
    unsigned int maxZ = zOrder(x1, y1, minX, minY, invSize);

    Node* p = ear->prevZ;
    Node* n = ear->nextZ;

    auto check = [&](Node* v)->bool {
        return v->x >= x0 && v->x <= x1 && v->y >= y0 && v->y <= y1 &&
               v != a && v != c &&
               pointInTriangleExceptFirst(ax, ay, bx, by, cx, cy, v->x, v->y) &&
               area(v->prev, v, v->next) >= 0;
    };

    while (p && p->z >= minZ && n && n->z <= maxZ) {
        if (check(p)) return false;
        if (check(n)) return false;
        p = p->prevZ;
        n = n->nextZ;
    }
    while (p && p->z >= minZ) {
        if (check(p)) return false;
        p = p->prevZ;
    }
    while (n && n->z <= maxZ) {
        if (check(n)) return false;
        n = n->nextZ;
    }
    return true;
}

inline Node* cureLocalIntersections(Node* start, std::vector<double>& triangles) {
    Node* p = start;
    do {
        Node* a = p->prev;
        Node* b = p->next->next;
        if (!equals(a, b) && intersects(a, p, p->next, b) &&
            locallyInside(a, b) && locallyInside(b, a))
        {
            triangles.push_back(a->i);
            triangles.push_back(p->i);
            triangles.push_back(b->i);
            removeNode(p);
            removeNode(p->next);
            p = start = b;
        }
        p = p->next;
    } while (p != start);
    return filterPoints(p);
}

inline void earcutLinked(Node* ear,
                         std::vector<double>& triangles,
                         int dim,
                         double minX,
                         double minY,
                         double invSize,
                         int pass);

inline void splitEarcut(Node* start,
                        std::vector<double>& triangles,
                        int dim,
                        double minX,
                        double minY,
                        double invSize)
{
    Node* a = start;
    do {
        Node* b = a->next->next;
        while (b != a->prev) {
            if (a->i != b->i && isValidDiagonal(a, b)) {
                Node* c = splitPolygon(a, b, *(new std::vector<Node*>())); // local leak okay (demo)
                a = filterPoints(a, a->next);
                c = filterPoints(c, c->next);
                earcutLinked(a, triangles, dim, minX, minY, invSize, 0);
                earcutLinked(c, triangles, dim, minX, minY, invSize, 0);
                return;
            }
            b = b->next;
        }
        a = a->next;
    } while (a != start);
}

inline void earcutLinked(Node* ear,
                         std::vector<double>& triangles,
                         int dim,
                         double minX,
                         double minY,
                         double invSize,
                         int pass)
{
    if (!ear) return;
    if (!pass && invSize) indexCurve(ear, minX, minY, invSize);
    Node* stop = ear;
    for (;;) {
        Node* prev = ear->prev;
        Node* next = ear->next;
        bool isEarOk = invSize ? isEarHashed(ear, minX, minY, invSize) : isEar(ear);
        if (isEarOk) {
            triangles.push_back(prev->i);
            triangles.push_back(ear->i);
            triangles.push_back(next->i);
            removeNode(ear);
            ear = next->next;
            stop = ear;
            if (ear->prev == ear->next) return;
            continue;
        }
        ear = next;
        if (ear == stop) {
            if (pass == 0) {
                earcutLinked(filterPoints(ear), triangles, dim, minX, minY, invSize, 1);
            } else if (pass == 1) {
                Node* ear2 = cureLocalIntersections(filterPoints(ear), triangles);
                earcutLinked(ear2, triangles, dim, minX, minY, invSize, 2);
            } else if (pass == 2) {
                splitEarcut(ear, triangles, dim, minX, minY, invSize);
            }
            break;
        }
    }
}

} // namespace detail

std::vector<int> earcut(const std::vector<double>& data,
                        const std::vector<size_t>* holeIndices,
                        int dim)
{
    using namespace detail;
    std::vector<double> triangles_double;
    std::vector<int> triangles;
    if (data.empty()) return triangles;

    bool hasHoles = holeIndices && !holeIndices->empty();
    int outerLen = hasHoles ? static_cast<int>((*holeIndices)[0]) * dim
                            : static_cast<int>(data.size());

    std::vector<Node*> pool;
    Node* outerNode = linkedList(data, 0, outerLen, dim, true, pool);
    if (!outerNode || outerNode->next == outerNode->prev) {
        for (Node* n : pool) delete n;
        return triangles;
    }

    if (hasHoles)
        outerNode = eliminateHoles(data, *holeIndices, outerNode, dim, pool);

    double minX = 0, minY = 0, invSize = 0;
    if ((int)data.size() > 80 * dim) {
        minX = data[0];
        minY = data[1];
        double maxX = minX;
        double maxY = minY;
        for (int i = dim; i < outerLen; i += dim) {
            double x = data[i];
            double y = data[i + 1];
            if (x < minX) minX = x;
            if (y < minY) minY = y;
            if (x > maxX) maxX = x;
            if (y > maxY) maxY = y;
        }
        double size = std::max(maxX - minX, maxY - minY);
        invSize = size != 0 ? (32767.0 / size) : 0.0;
    }

    earcutLinked(outerNode, triangles_double, dim, minX, minY, invSize, 0);

    // Convert double indices to int
    triangles.reserve(triangles_double.size());
    for (double idx : triangles_double) {
        triangles.push_back(static_cast<int>(std::round(idx)));
    }

    for (Node* n : pool) delete n;
    return triangles;
}

double deviation(const std::vector<double>& data,
                 const std::vector<size_t>* holeIndices,
                 int dim,
                 const std::vector<double>& triangles)
{
    using namespace detail;
    bool hasHoles = holeIndices && !holeIndices->empty();
    int outerLen = hasHoles ? static_cast<int>((*holeIndices)[0]) * dim
                            : static_cast<int>(data.size());

    double polygonArea = std::fabs(signedArea(data, 0, outerLen, dim));
    if (hasHoles) {
        for (size_t i = 0; i < holeIndices->size(); ++i) {
            int start = static_cast<int>((*holeIndices)[i]) * dim;
            int end = (i + 1 < holeIndices->size()) ? static_cast<int>((*holeIndices)[i + 1]) * dim : (int)data.size();
            polygonArea -= std::fabs(signedArea(data, start, end, dim));
        }
    }

    double triArea = 0.0;
    for (size_t i = 0; i + 2 < triangles.size(); i += 3) {
        int a = (int)triangles[i] * dim;
        int b = (int)triangles[i + 1] * dim;
        int c = (int)triangles[i + 2] * dim;
        triArea += std::fabs(
            (data[a] - data[c]) * (data[b + 1] - data[a + 1]) -
            (data[a] - data[b]) * (data[c + 1] - data[a + 1])
        );
    }
    if (polygonArea == 0 && triArea == 0) return 0.0;
    return polygonArea ? std::fabs((triArea - polygonArea) / polygonArea)
                       : std::numeric_limits<double>::infinity();
}

FlattenResult flatten(const std::vector<std::vector<std::array<double,2>>>& rings) {
    FlattenResult f;
    if (rings.empty()) return f;
    f.dimensions = 2;
    size_t holeIndex = 0;
    size_t prevLen = 0;
    for (size_t r = 0; r < rings.size(); ++r) {
        const auto& ring = rings[r];
        for (const auto& p : ring) {
            f.vertices.push_back(p[0]);
            f.vertices.push_back(p[1]);
        }
        if (r > 0) {
            holeIndex += prevLen;
            f.holes.push_back(holeIndex);
        }
        prevLen = ring.size();
    }
    return f;
}

} // namespace mb

namespace MotionByte {

    VertexList triangulate(const std::vector<VertexList>& outerRings, 
                           const std::vector<VertexList>& holes) {
        VertexList result;
        
        // Process each outer ring separately
        for (const auto& outerRing : outerRings) {
            // Convert VertexList to flat coordinate array
            std::vector<double> coords;
            std::vector<size_t> holeIndices;
            
            // Add outer ring vertices
            for (size_t i = 0; i < outerRing.size(); ++i) {
                const auto& vertex = const_cast<VertexList&>(outerRing).getVertex(i);
                coords.push_back(static_cast<double>(vertex.x));
                coords.push_back(static_cast<double>(vertex.y));
            }
            
            // Add hole vertices (for this specific outer ring)
            // Note: In this implementation, holes apply to all outer rings
            // You might want to modify this to associate specific holes with specific outer rings
            for (const auto& hole : holes) {
                holeIndices.push_back(coords.size() / 2); // Store vertex index where hole starts
                for (size_t i = 0; i < hole.size(); ++i) {
                    const auto& vertex = const_cast<VertexList&>(hole).getVertex(i);
                    coords.push_back(static_cast<double>(vertex.x));
                    coords.push_back(static_cast<double>(vertex.y));
                }
            }
            
            // Perform triangulation for this outer ring
            std::vector<int> triangleIndices = mb::earcut(coords, holeIndices.empty() ? nullptr : &holeIndices, 2);
            
            // Convert indices back to actual vertices and add to result
            for (size_t idx : triangleIndices) {
                if (idx * 2 + 1 < coords.size()) {
                    result.addVertex(static_cast<float>(coords[idx * 2]), 
                                static_cast<float>(coords[idx * 2 + 1]));
                }
            }
        }
        
        return result;
    }

}