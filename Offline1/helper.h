#include<bits/stdc++.h>
using namespace std ; 
#ifdef __linux__
    #include <GL/glut.h>
#elif 
    #include <windows.h>
    #include <glut.h>
#endif

#define pi (2 * acos(0.0)) 

class point {
    public:
    double x, y, z;
     
    point() {
        x = 0; y = 0; z = 0;
    }

    point(double a ,double b , double c) {
        x = a; y = b; z = c;
    }

    double getx(){return x;} 
    double gety(){return y;} 
    double getz(){return z;} 

    void setx(double a){x = a;}
    void sety(double a){y = a;}
    void setz(double a){z = a;}

    float magnitude() {
        return sqrt(x * x + y * y + z * z);
    }

    point addition(point p) {
        point newpoint(x + p.getx(), y + p.gety(), z + p.getz()); 
        return newpoint; 
    }

    void setattr(double a , double b , double c) {
        x = a; y = b; z = c;
    }

    point subtraction(point p) {
        point newpoint(x - p.getx(), y - p.gety(), z - p.getz()); 
        return newpoint; 
    }

    point scalarMul(double n) {
        point newpoint(x * n, y * n, z * n); 
        return newpoint; 
    }

    point crossMul(point p) {
        point newpoint(y * p.getz() - z * p.gety(), z * p.getx() - x * p.getz(), x * p.gety() - y * p.getx()); 
        return newpoint; 
    }

    void show(string str = "KEYBOARD") {
        cout << str << " " << x << "," << y << "," << z << "---" << endl; 
    }

    point normalize() {
        double len = sqrt(x * x + y * y + z * z);
        return point(x / len, y / len, z / len);
    }
    
}; 

struct Sphere_point {
	double x, y, z;
    public:
    Sphere_point scalarMul(double n) {
        Sphere_point newpoint;
        newpoint.x = x * n; 
        newpoint.y = y * n; 
        newpoint.z = z * n;          
        return newpoint ; 
    }

    Sphere_point crossMul(point p) {
        Sphere_point newpoint ; 
        newpoint.x = y * p.getz() - z * p.gety(); 
        newpoint.y = z * p.getx() - x * p.getz(); 
        newpoint.z = x * p.gety() - y * p.getx();
        return newpoint ; 
    }

    Sphere_point addition(Sphere_point p) {
        Sphere_point newpoint;
        newpoint.x = x + p.x; 
        newpoint.y = y + p.y; 
        newpoint.z = z + p.z;          
        return newpoint ; 
    }
};

point position_of_camera ; 
point camera_up; 
point camera_look; 
point camera_right; 

point RodriGeneral(point rotatingVector, point withRespectTo, double angle) {
    point perpendicular; 
    perpendicular = rotatingVector.crossMul(withRespectTo); 
    point newpoint ; 
    newpoint = rotatingVector.scalarMul(cos(angle)).addition(perpendicular.scalarMul(sin(angle))); 
    return newpoint; 
}

struct Sphere_point points[100][100];