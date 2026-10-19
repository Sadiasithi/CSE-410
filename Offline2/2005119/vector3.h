#ifndef VECTOR3_H
#define VECTOR3_H

#include <iostream>
#include <cmath>

class vector3{
private:
    double x, y, z, t;

public:
    vector3();
    vector3(double x, double y, double z, double t = 1);

    void setX(double x);
    double getX() const;
    void setY(double y);
    double getY() const;
    void setZ(double z);
    double getZ() const;
    void setT(double t);
    double getT() const;

    vector3 operator+(const vector3& vct) const;
    vector3 operator-(const vector3& vct) const;
    vector3 operator/(double n) const;

    friend vector3 operator*(double n, const vector3& vct);
    friend std::ostream& operator<<(std::ostream& out, const vector3& vct);
};

double dot(const vector3& vct1, const vector3& vct2);
double dot4(const vector3& vct1, const vector3& vct2);
vector3 cross(const vector3& vct1, const vector3& vct2);

double max3(double a, double b, double c);
double min3(double a, double b, double c);

double slopeXY(const vector3& a, const vector3& b);
double slopeYZ(const vector3& a, const vector3& b);
double slopeZX(const vector3& a, const vector3& b);

vector3 normalize(const vector3& vct);
vector3 rodriguez(const vector3& vct1, const vector3& vct2, double theta);

#endif // VECTOR3_H
