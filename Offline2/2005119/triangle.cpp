#include "triangle.h"

using namespace std;

Triangle::Triangle() {}

Triangle::Triangle(vector3 a, vector3 b, vector3 c) {
    vtx[0] = a;
    vtx[1] = b;
    vtx[2] = c;
}

Triangle::Triangle(matrix M) {
    for (int i = 0; i < 3; i++) {
        vtx[i] = M.vct[i];
    }
}

vector<Triangle> TriangleSet;
