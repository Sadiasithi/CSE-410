#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <vector>
#include "vector3.h"
#include "matrix.h"

class Triangle {
public:
    vector3 vtx[3];

    Triangle();
    Triangle(vector3 a, vector3 b, vector3 c);
    Triangle(matrix M);
};

extern std::vector<Triangle> TriangleSet;

#endif // TRIANGLE_H
