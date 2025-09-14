"""
Python implementation of the Earcut polygon triangulation algorithm.

Ported and adapted from the original JavaScript implementation (mapbox/earcut)
which is ISC licensed. Original Copyright (c) 2016, Mapbox.

This version provides:
 - earcut(data, hole_indices=None, dim=2) -> list of triangle vertex indices
 - deviation(data, hole_indices, dim, triangles)
 - flatten(multi_ring_polygon)
 - Example usage & matplotlib plot when run as a script

Notes:
 - data: flat list of coordinates [x0, y0, x1, y1, ...]
 - hole_indices: list of starting indices (in vertex units, not flat array units)
   for each hole inside data (like earcut.js)
 - dim: coordinate dimension (2 for (x,y); 3 also supported though z ignored)

"""
from __future__ import annotations
from dataclasses import dataclass
from typing import List, Optional, Tuple, Iterable, Dict, Any
import math

# -----------------------------------------------------------------------------
# Node structure (doubly-linked list for polygon rings + z-order links)
# -----------------------------------------------------------------------------

@dataclass
class Node:
    i: float            # index of point in the vertices array (vertex index, not flat index)
    x: float            # x coordinate
    y: float            # y coordinate
    prev: 'Node' = None # previous node (ring)
    next: 'Node' = None # next node (ring)
    z: float = 0.0      # z-order curve value
    prevZ: 'Node' = None
    nextZ: 'Node' = None
    steiner: bool = False

    # For easier debugging
    def __repr__(self):  # pragma: no cover (debug helper)
        return f"Node(i={self.i}, x={self.x:.3f}, y={self.y:.3f})"

# -----------------------------------------------------------------------------
# Public API
# -----------------------------------------------------------------------------

def earcut(data: List[float], hole_indices: Optional[List[float]] = None, dim: int = 2) -> List[float]:
    has_holes = bool(hole_indices)
    outer_len = (hole_indices[0] * dim) if has_holes else len(data)
    outer_node = _linked_list(data, 0, outer_len, dim, True)
    triangles: List[float] = []

    if (not outer_node) or (outer_node.next == outer_node.prev):
        return triangles

    min_x = min_y = inv_size = None

    if has_holes:
        outer_node = _eliminate_holes(data, hole_indices, outer_node, dim)

    if len(data) > 80 * dim:  # large enough for z-order optimization
        min_x = data[0]
        min_y = data[1]
        max_x = min_x
        max_y = min_y
        for i in range(dim, outer_len, dim):
            x = data[i]
            y = data[i + 1]
            if x < min_x: min_x = x
            if y < min_y: min_y = y
            if x > max_x: max_x = x
            if y > max_y: max_y = y
        inv_size = max(max_x - min_x, max_y - min_y)
        inv_size = (32767 / inv_size) if inv_size != 0 else 0

    _earcut_linked(outer_node, triangles, dim, min_x, min_y, inv_size, 0)
    return triangles

def deviation(data: List[float], hole_indices: Optional[List[float]], dim: int, triangles: List[float]) -> float:
    has_holes = bool(hole_indices)
    outer_len = (hole_indices[0] * dim) if has_holes else len(data)

    polygon_area = abs(_signed_area(data, 0, outer_len, dim))
    if has_holes:
        for i, start in enumerate(hole_indices):
            end = (hole_indices[i+1] * dim) if i < len(hole_indices) - 1 else len(data)
            polygon_area -= abs(_signed_area(data, start * dim, end, dim))

    triangles_area = 0.0
    for i in range(0, len(triangles), 3):
        a = triangles[i] * dim
        b = triangles[i + 1] * dim
        c = triangles[i + 2] * dim
        triangles_area += abs(
            (data[a] - data[c]) * (data[b + 1] - data[a + 1]) -
            (data[a] - data[b]) * (data[c + 1] - data[a + 1])
        )

    if polygon_area == 0 and triangles_area == 0:
        return 0.0
    return abs((triangles_area - polygon_area) / polygon_area) if polygon_area else float('inf')

def flatten(data: List[List[Tuple[float, float]]]) -> Dict[str, Any]:
    vertices: List[float] = []
    holes: List[float] = []
    dimensions = len(data[0][0])
    hole_index = 0
    prev_len = 0
    for ring in data:
        for p in ring:
            for d in range(dimensions):
                vertices.append(p[d])
        if prev_len:
            hole_index += prev_len
            holes.append(hole_index)
        prev_len = len(ring)
    return {"vertices": vertices, "holes": holes, "dimensions": dimensions}

# -----------------------------------------------------------------------------
# Internal helpers (direct translations & minor pythonic adjustments)
# -----------------------------------------------------------------------------

def _linked_list(data: List[float], start: int, end: int, dim: int, clockwise: bool) -> Optional[Node]:
    last = None
    if clockwise == (_signed_area(data, start, end, dim) > 0):
        i_range = range(start, end, dim)
    else:
        i_range = range(end - dim, start - dim, -dim)
    for i in i_range:
        # Create fractional index based on coordinate values for more precision
        base_index = i // dim
        x_frac = (data[i] % 1.0) * 0.1  # Use fractional part of x coordinate
        y_frac = (data[i + 1] % 1.0) * 0.01  # Use fractional part of y coordinate  
        fractional_index = base_index + x_frac + y_frac
        last = _insert_node(fractional_index, data[i], data[i + 1], last)
    if last and _equals(last, last.next):
        _remove_node(last)
        last = last.next
    return last

def _filter_points(start: Optional[Node], end: Optional[Node] = None) -> Optional[Node]:
    if not start:
        return start
    if not end:
        end = start
    p = start
    again = True
    while again or (p != end):
        again = False
        if (not p.steiner) and (_equals(p, p.next) or _area(p.prev, p, p.next) == 0):
            _remove_node(p)
            p = end = p.prev
            if p == p.next:
                break
            again = True
        else:
            p = p.next
    return end

def _earcut_linked(ear: Optional[Node], triangles: List[float], dim: int, min_x, min_y, inv_size, _pass: int):
    if not ear:
        return
    if (not _pass) and (inv_size):
        _index_curve(ear, min_x, min_y, inv_size)
    stop = ear
    while ear.prev != ear.next:
        prev = ear.prev
        nxt = ear.next
        if (inv_size and _is_ear_hashed(ear, min_x, min_y, inv_size)) or ((not inv_size) and _is_ear(ear)):
            triangles.extend([float(prev.i), float(ear.i), float(nxt.i)])
            _remove_node(ear)
            ear = nxt.next
            stop = nxt.next
            continue
        ear = nxt
        if ear == stop:
            if _pass == 0:
                _earcut_linked(_filter_points(ear), triangles, dim, min_x, min_y, inv_size, 1)
            elif _pass == 1:
                ear2 = _cure_local_intersections(_filter_points(ear), triangles)
                _earcut_linked(ear2, triangles, dim, min_x, min_y, inv_size, 2)
            elif _pass == 2:
                _split_earcut(ear, triangles, dim, min_x, min_y, inv_size)
            break

def _is_ear(ear: Node) -> bool:
    a = ear.prev; b = ear; c = ear.next
    if _area(a, b, c) >= 0:
        return False
    ax, ay, bx, by, cx, cy = a.x, a.y, b.x, b.y, c.x, c.y
    x0 = min(ax, bx, cx); y0 = min(ay, by, cy)
    x1 = max(ax, bx, cx); y1 = max(ay, by, cy)
    p = c.next
    while p != a:
        if (p.x >= x0 and p.x <= x1 and p.y >= y0 and p.y <= y1 and
            _point_in_triangle_except_first(ax, ay, bx, by, cx, cy, p.x, p.y) and
            _area(p.prev, p, p.next) >= 0):
            return False
        p = p.next
    return True

def _is_ear_hashed(ear: Node, min_x, min_y, inv_size) -> bool:
    a = ear.prev; b = ear; c = ear.next
    if _area(a, b, c) >= 0:
        return False
    ax, ay, bx, by, cx, cy = a.x, a.y, b.x, b.y, c.x, c.y
    x0 = min(ax, bx, cx); y0 = min(ay, by, cy)
    x1 = max(ax, bx, cx); y1 = max(ay, by, cy)
    min_z = _z_order(x0, y0, min_x, min_y, inv_size)
    max_z = _z_order(x1, y1, min_x, min_y, inv_size)
    p = ear.prevZ
    n = ear.nextZ
    while p and p.z >= min_z and n and n.z <= max_z:
        if (p.x >= x0 and p.x <= x1 and p.y >= y0 and p.y <= y1 and p != a and p != c and
            _point_in_triangle_except_first(ax, ay, bx, by, cx, cy, p.x, p.y) and _area(p.prev, p, p.next) >= 0):
            return False
        p = p.prevZ
        if (n.x >= x0 and n.x <= x1 and n.y >= y0 and n.y <= y1 and n != a and n != c and
            _point_in_triangle_except_first(ax, ay, bx, by, cx, cy, n.x, n.y) and _area(n.prev, n, n.next) >= 0):
            return False
        n = n.nextZ
    while p and p.z >= min_z:
        if (p.x >= x0 and p.x <= x1 and p.y >= y0 and p.y <= y1 and p != a and p != c and
            _point_in_triangle_except_first(ax, ay, bx, by, cx, cy, p.x, p.y) and _area(p.prev, p, p.next) >= 0):
            return False
        p = p.prevZ
    while n and n.z <= max_z:
        if (n.x >= x0 and n.x <= x1 and n.y >= y0 and n.y <= y1 and n != a and n != c and
            _point_in_triangle_except_first(ax, ay, bx, by, cx, cy, n.x, n.y) and _area(n.prev, n, n.next) >= 0):
            return False
        n = n.nextZ
    return True

def _cure_local_intersections(start: Node, triangles: List[float]) -> Node:
    p = start
    while True:
        a = p.prev
        b = p.next.next
        if (not _equals(a, b) and _intersects(a, p, p.next, b) and
            _locally_inside(a, b) and _locally_inside(b, a)):
            triangles.extend([float(a.i), float(p.i), float(b.i)])
            _remove_node(p)
            _remove_node(p.next)
            p = start = b
        p = p.next
        if p == start:
            break
    return _filter_points(p)

def _split_earcut(start: Node, triangles: List[float], dim: int, min_x, min_y, inv_size):
    a = start
    while True:
        b = a.next.next
        while b != a.prev:
            if (a.i != b.i) and _is_valid_diagonal(a, b):
                c = _split_polygon(a, b)
                a = _filter_points(a, a.next)
                c = _filter_points(c, c.next)
                _earcut_linked(a, triangles, dim, min_x, min_y, inv_size, 0)
                _earcut_linked(c, triangles, dim, min_x, min_y, inv_size, 0)
                return
            b = b.next
        a = a.next
        if a == start:
            break

def _eliminate_holes(data: List[float], hole_indices: List[float], outer_node: Node, dim: int) -> Node:
    queue: List[Node] = []
    for i in range(len(hole_indices)):
        start = hole_indices[i] * dim
        end = (hole_indices[i+1] * dim) if i < len(hole_indices) - 1 else len(data)
        lst = _linked_list(data, start, end, dim, False)
        if lst == lst.next:
            lst.steiner = True
        queue.append(_get_leftmost(lst))
    queue.sort(key=lambda n: (n.x, n.y, (n.next.y - n.y) / (n.next.x - n.x) if (n.next.x - n.x)!=0 else math.inf))
    for h in queue:
        outer_node = _eliminate_hole(h, outer_node)
    return outer_node

def _eliminate_hole(hole: Node, outer_node: Node) -> Node:
    bridge = _find_hole_bridge(hole, outer_node)
    if not bridge:
        return outer_node
    bridge_reverse = _split_polygon(bridge, hole)
    _filter_points(bridge_reverse, bridge_reverse.next)
    return _filter_points(bridge, bridge.next)

def _find_hole_bridge(hole: Node, outer_node: Node) -> Optional[Node]:
    p = outer_node
    hx, hy = hole.x, hole.y
    qx = -math.inf
    m = None
    if _equals(hole, p):
        return p
    while True:
        if _equals(hole, p.next):
            return p.next
        if (hy <= p.y and hy >= p.next.y and p.next.y != p.y):
            x = p.x + (hy - p.y) * (p.next.x - p.x) / (p.next.y - p.y)
            if x <= hx and x > qx:
                qx = x
                m = p if p.x < p.next.x else p.next
                if x == hx:
                    return m
        p = p.next
        if p == outer_node:
            break
    if not m:
        return None
    stop = m
    mx, my = m.x, m.y
    tan_min = math.inf
    p = m
    while True:
        if (hx >= p.x >= mx and hx != p.x and
            _point_in_triangle(hy < my and hx or qx, hy, mx, my, hy < my and qx or hx, hy, p.x, p.y)):
            tan = abs(hy - p.y) / (hx - p.x)
            if (_locally_inside(p, hole) and
                (tan < tan_min or (tan == tan_min and (p.x > m.x or (p.x == m.x and _sector_contains_sector(m, p)))))):
                m = p
                tan_min = tan
        p = p.next
        if p == stop:
            break
    return m

def _sector_contains_sector(m: Node, p: Node) -> bool:
    return _area(m.prev, m, p.prev) < 0 and _area(p.next, m, m.next) < 0

def _index_curve(start: Node, min_x, min_y, inv_size):
    p = start
    while True:
        if p.z == 0:
            p.z = _z_order(p.x, p.y, min_x, min_y, inv_size)
        p.prevZ = p.prev
        p.nextZ = p.next
        p = p.next
        if p == start:
            break
    p_prevZ = p.prevZ
    p_prevZ.nextZ = None
    p.prevZ = None
    _sort_linked(p)

def _sort_linked(lst: Node) -> Node:
    in_size = 1
    while True:
        p = lst
        lst = None
        tail = None
        num_merges = 0
        while p:
            num_merges += 1
            q = p
            p_size = 0
            for _ in range(in_size):
                p_size += 1
                q = q.nextZ
                if not q:
                    break
            q_size = in_size
            while p_size > 0 or (q_size > 0 and q):
                if p_size != 0 and (q_size == 0 or (not q) or p.z <= q.z):
                    e = p
                    p = p.nextZ
                    p_size -= 1
                else:
                    e = q
                    q = q.nextZ
                    q_size -= 1
                if tail:
                    tail.nextZ = e
                else:
                    lst = e
                e.prevZ = tail
                tail = e
            p = q
        tail.nextZ = None
        in_size *= 2
        if num_merges <= 1:
            return lst

def _z_order(x: float, y: float, min_x: float, min_y: float, inv_size: float) -> int:
    x = int((x - min_x) * inv_size) if inv_size else 0
    y = int((y - min_y) * inv_size) if inv_size else 0
    x = (x | (x << 8)) & 0x00FF00FF
    x = (x | (x << 4)) & 0x0F0F0F0F
    x = (x | (x << 2)) & 0x33333333
    x = (x | (x << 1)) & 0x55555555
    y = (y | (y << 8)) & 0x00FF00FF
    y = (y | (y << 4)) & 0x0F0F0F0F
    y = (y | (y << 2)) & 0x33333333
    y = (y | (y << 1)) & 0x55555555
    return x | (y << 1)

def _get_leftmost(start: Node) -> Node:
    p = start
    leftmost = start
    while True:
        if (p.x < leftmost.x) or (p.x == leftmost.x and p.y < leftmost.y):
            leftmost = p
        p = p.next
        if p == start:
            break
    return leftmost

def _point_in_triangle(ax, ay, bx, by, cx, cy, px, py) -> bool:
    return ((cx - px) * (ay - py) >= (ax - px) * (cy - py) and
            (ax - px) * (by - py) >= (bx - px) * (ay - py) and
            (bx - px) * (cy - py) >= (cx - px) * (by - py))

def _point_in_triangle_except_first(ax, ay, bx, by, cx, cy, px, py) -> bool:
    return not (ax == px and ay == py) and _point_in_triangle(ax, ay, bx, by, cx, cy, px, py)

def _is_valid_diagonal(a: Node, b: Node) -> bool:
    return (a.next.i != b.i and a.prev.i != b.i and not _intersects_polygon(a, b) and
            ((_locally_inside(a, b) and _locally_inside(b, a) and _middle_inside(a, b) and
              (_area(a.prev, a, b.prev) or _area(a, b.prev, b))) or
             (_equals(a, b) and _area(a.prev, a, a.next) > 0 and _area(b.prev, b, b.next) > 0)))

def _area(p: Node, q: Node, r: Node) -> float:
    return (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y)

def _equals(p1: Node, p2: Node) -> bool:
    return p1.x == p2.x and p1.y == p2.y

def _intersects(p1: Node, q1: Node, p2: Node, q2: Node) -> bool:
    o1 = _sign(_area(p1, q1, p2))
    o2 = _sign(_area(p1, q1, q2))
    o3 = _sign(_area(p2, q2, p1))
    o4 = _sign(_area(p2, q2, q1))
    if o1 != o2 and o3 != o4:
        return True
    if o1 == 0 and _on_segment(p1, p2, q1): return True
    if o2 == 0 and _on_segment(p1, q2, q1): return True
    if o3 == 0 and _on_segment(p2, p1, q2): return True
    if o4 == 0 and _on_segment(p2, q1, q2): return True
    return False

def _on_segment(p: Node, q: Node, r: Node) -> bool:
    return (q.x <= max(p.x, r.x) and q.x >= min(p.x, r.x) and
            q.y <= max(p.y, r.y) and q.y >= min(p.y, r.y))

def _sign(num: float) -> int:
    return 1 if num > 0 else (-1 if num < 0 else 0)

def _intersects_polygon(a: Node, b: Node) -> bool:
    p = a
    while True:
        if (p.i != a.i and p.next.i != a.i and p.i != b.i and p.next.i != b.i and
            _intersects(p, p.next, a, b)):
            return True
        p = p.next
        if p == a:
            break
    return False

def _locally_inside(a: Node, b: Node) -> bool:
    if _area(a.prev, a, a.next) < 0:
        return _area(a, b, a.next) >= 0 and _area(a, a.prev, b) >= 0
    else:
        return _area(a, b, a.prev) < 0 or _area(a, a.next, b) < 0

def _middle_inside(a: Node, b: Node) -> bool:
    p = a
    inside = False
    px = (a.x + b.x) / 2.0
    py = (a.y + b.y) / 2.0
    while True:
        if ((p.y > py) != (p.next.y > py)) and (p.next.y != p.y) and (px < (p.next.x - p.x) * (py - p.y) / (p.next.y - p.y) + p.x):
            inside = not inside
        p = p.next
        if p == a:
            break
    return inside

def _split_polygon(a: Node, b: Node) -> Node:
    a2 = _create_node(a.i, a.x, a.y)
    b2 = _create_node(b.i, b.x, b.y)
    an = a.next
    bp = b.prev
    a.next = b
    b.prev = a
    a2.next = an
    an.prev = a2
    b2.next = a2
    a2.prev = b2
    bp.next = b2
    b2.prev = bp
    return b2

def _insert_node(i: float, x: float, y: float, last: Optional[Node]) -> Node:
    p = _create_node(i, x, y)
    if not last:
        p.prev = p
        p.next = p
    else:
        p.next = last.next
        p.prev = last
        last.next.prev = p
        last.next = p
    return p

def _remove_node(p: Node):
    p.next.prev = p.prev
    p.prev.next = p.next
    if p.prevZ:
        p.prevZ.nextZ = p.nextZ
    if p.nextZ:
        p.nextZ.prevZ = p.prevZ

def _create_node(i: float, x: float, y: float) -> Node:
    return Node(i=i, x=x, y=y)

def _signed_area(data: List[float], start: int, end: int, dim: int) -> float:
    s = 0.0
    j = end - dim
    i = start
    while i < end:
        s += (data[j] - data[i]) * (data[i + 1] + data[j + 1])
        j = i
        i += dim
    return s

# -----------------------------------------------------------------------------
# Example usage / plotting
# -----------------------------------------------------------------------------

def _example():  # pragma: no cover
    import matplotlib.pyplot as plt
    # Define outer square and a rectangular hole
    outer = [
        (21.1406,11), (25.875,11), (25.875,7.45312), (21.1406,7.45312), (21.1406,0), (16.7812,0), (16.7812,7.45312), (1.25,7.45312), (1.25,10.0156), (16.5312,34), (21.1406,34), (21.1406,11)
    ]
    hole = [
        (6.17188,11), (16.7812,11), (16.7812,28.0938), (16.2656,27.1406), (6.17188,11)
    ]
    flat = flatten([outer, hole])
    verts = flat['vertices']
    holes = flat['holes']
    dim = flat['dimensions']
    tris = earcut(verts, holes, dim)
    print("Triangles (vertex indices):", tris)
    print(f"Number of vertices: {len(verts)//dim}")
    print(f"Number of triangles: {len(tris)//3}")
    
    # Convert flat vertices to list of (x,y)
    points = [(verts[i], verts[i+1]) for i in range(0, len(verts), dim)]
    print(f"Points: {points}")
    
    fig, ax = plt.subplots(figsize=(10, 8))
    
    # Draw triangles - map fractional indices back to integer array indices
    for i in range(0, len(tris), 3):
        ia, ib, ic = int(tris[i]), int(tris[i+1]), int(tris[i+2])
        print(f"Triangle {i//3}: indices {ia}, {ib}, {ic}")
        
        # Check if indices are valid
        if ia >= len(points) or ib >= len(points) or ic >= len(points):
            print(f"Warning: Invalid indices for triangle {i//3}")
            continue
            
        tri = [points[ia], points[ib], points[ic], points[ia]]
        xs, ys = zip(*tri)
        ax.fill(xs, ys, facecolor='#1f77b430', edgecolor='#555555', linewidth=0.8)
    # Draw outer polygon outline
    ox, oy = zip(*(outer + [outer[0]]))
    ax.plot(ox, oy, 'k-', lw=2)
    hx, hy = zip(*(hole + [hole[0]]))
    ax.plot(hx, hy, 'r-', lw=2)
    ax.set_aspect('equal')
    ax.set_title('Earcut Triangulation Example')
    ax.set_xlim(0, 27)
    ax.set_ylim(-1, 35)
    plt.savefig('triangulation_result.png', dpi=150, bbox_inches='tight')
    plt.show()

if __name__ == '__main__':  # pragma: no cover
    _example()
