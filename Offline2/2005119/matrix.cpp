#include "matrix.h"
#include <iostream>

using namespace std;

matrix::matrix() {
    vct[0] = vector3(1, 0, 0, 0);
    vct[1] = vector3(0, 1, 0, 0);
    vct[2] = vector3(0, 0, 1, 0);
    vct[3] = vector3(0, 0, 0, 1);
}

matrix::matrix(vector3 vct0, vector3 vct1, vector3 vct2, vector3 vct3) {
    vct[0] = vct0;
    vct[1] = vct1;
    vct[2] = vct2;
    vct[3] = vct3;
}

matrix matrix::operator*(const matrix& M) const {
    vector3 vct2[4];

    vct2[0] = vector3(vct[0].getX(), vct[1].getX(), vct[2].getX(), vct[3].getX());
    vct2[1] = vector3(vct[0].getY(), vct[1].getY(), vct[2].getY(), vct[3].getY());
    vct2[2] = vector3(vct[0].getZ(), vct[1].getZ(), vct[2].getZ(), vct[3].getZ());
    vct2[3] = vector3(vct[0].getT(), vct[1].getT(), vct[2].getT(), vct[3].getT());

    vector3 vct3[4];
    for (int i = 0; i < 4; i++) {
        vct3[i].setX(dot4(vct2[0], M.vct[i]));
        vct3[i].setY(dot4(vct2[1], M.vct[i]));
        vct3[i].setZ(dot4(vct2[2], M.vct[i]));
        vct3[i].setT(dot4(vct2[3], M.vct[i]));
    }
    return matrix(vct3[0], vct3[1], vct3[2], vct3[3]);
}

void matrix::scale() {
    for (int i = 0; i < 3; i++) {
        vct[i] = vct[i] / vct[i].getT();
        vct[i].setT(1);
    }
}

ostream& operator<<(ostream& out, const matrix& mat) {
    for (int i = 0; i < 3; i++) {
        out << mat.vct[i];
    }
    out << endl;
    return out;
}
