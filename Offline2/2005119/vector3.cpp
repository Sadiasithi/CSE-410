#include "vector3.h"

using namespace std;

vector3::vector3() : x(0), y(0), z(0), t(1) {}

vector3::vector3(double x, double y, double z, double t) {
    this->x = x;
    this->y = y;
    this->z = z;
    this->t = t;
}

void vector3::setX(double x) { this->x = x; }
double vector3::getX() const { return x; }
void vector3::setY(double y) { this->y = y; }
double vector3::getY() const { return y; }
void vector3::setZ(double z) { this->z = z; }
double vector3::getZ() const { return z; }
void vector3::setT(double t) { this->t = t; }
double vector3::getT() const { return t; }

vector3 vector3::operator+(const vector3& vct) const {
    return vector3(x + vct.getX(), y + vct.getY(), z + vct.getZ());
}

vector3 vector3::operator-(const vector3& vct) const {
    return vector3(x - vct.getX(), y - vct.getY(), z - vct.getZ());
}

vector3 vector3::operator/(double n) const {
    return vector3(x / n, y / n, z / n, t);
}

vector3 operator*(double n, const vector3& vct) {
    return vector3(n * vct.x, n * vct.y, n * vct.z, vct.t);
}

ostream& operator<<(ostream& out, const vector3& vct) {
    out << vct.x << " " << vct.y << " " << vct.z << endl;
    return out;
}

double dot(const vector3& vct1, const vector3& vct2) {
    return vct1.getX() * vct2.getX() + vct1.getY() * vct2.getY() + vct1.getZ() * vct2.getZ();
}

double dot4(const vector3& vct1, const vector3& vct2) {
    return vct1.getX() * vct2.getX() + vct1.getY() * vct2.getY() + vct1.getZ() * vct2.getZ() + vct1.getT() * vct2.getT();
}

vector3 cross(const vector3& vct1, const vector3& vct2) {
    double x = vct1.getY() * vct2.getZ() - vct1.getZ() * vct2.getY();
    double y = vct1.getZ() * vct2.getX() - vct1.getX() * vct2.getZ();
    double z = vct1.getX() * vct2.getY() - vct1.getY() * vct2.getX();
    return vector3(x, y, z);
}

double max3(double a, double b, double c) {
    return max(a, max(b, c));
}

double min3(double a, double b, double c) {
    return min(a, min(b, c));
}

double slopeXY(const vector3& a, const vector3& b) {
    double dy = a.getY() - b.getY();
    double dx = a.getX() - b.getX();
    return dy / dx;
}

double slopeYZ(const vector3& a, const vector3& b) {
    double dz = a.getZ() - b.getZ();
    double dy = a.getY() - b.getY();
    return dz / dy;
}

double slopeZX(const vector3& a, const vector3& b) {
    double dx = a.getX() - b.getX();
    double dz = a.getZ() - b.getZ();
    return dx / dz;
}

vector3 normalize(const vector3& vct) {
    double val = dot(vct, vct);
    val = sqrt(val);
    return vct / val;
}

vector3 rodriguez(const vector3& vct1, const vector3& vct2, double theta) {
    theta = M_PI * theta / 180;
    double a = (1 - cos(theta)) * dot(vct1, vct2);

    vector3 temp_vct1 = cos(theta) * vct1;
    vector3 temp_vct2 = a * vct2;
    vector3 temp_vct3 = sin(theta) * cross(vct2, vct1);
    return temp_vct1 + temp_vct2 + temp_vct3;
}
