#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include "vector3.h"

class matrix {
public:
    vector3 vct[4];

    matrix();
    matrix(vector3 vct0, vector3 vct1, vector3 vct2, vector3 vct3);

    matrix operator*(const matrix& M) const;
    void scale();
    friend std::ostream& operator<<(std::ostream& out, const matrix& mat);
};

#endif // MATRIX_H
