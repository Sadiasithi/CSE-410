#include "vector3.cpp"
#include "matrix.cpp"
#include "triangle.cpp"
#include <iomanip>
#include <fstream>
#include <string>
#include <sstream>
#include <stack>
#include <filesystem> 

using namespace std;
namespace fs = std::filesystem;

stack<matrix> S;
matrix M, I, V, P;
vector3 eye, look, up, l, r, u;
double fovY, aspectRatio, near, far;

vector3 parse_vector3(const string& line) {
    double x, y, z;
    stringstream ss(line);
    ss >> x >> y >> z;
    return vector3(x, y, z);
}

void apply_translation(double tx, double ty, double tz) {
    matrix t = I;
    t.vct[3] = vector3(tx, ty, tz, 1);
    M = M * t;
}

void apply_scaling(double sx, double sy, double sz) {
    matrix s = I;
    s.vct[0].setX(sx);
    s.vct[1].setY(sy);
    s.vct[2].setZ(sz);
    M = M * s;
}

void apply_rotation(double angleDeg, double ax, double ay, double az) {
    vector3 axis = normalize(vector3(ax, ay, az));
    matrix r = I;
    for (int i = 0; i < 3; ++i) {
        r.vct[i] = rodriguez(I.vct[i], axis, angleDeg);
        r.vct[i].setT(0);
    }
    M = M * r;
}


void generate_stage() {
    fs::create_directory("output");
    ifstream inputFile("input/scene.txt");
    ofstream stage1File("output/stage1.txt");
    ofstream stage2File("output/stage2.txt");
    ofstream stage3File("output/stage3.txt");

    stage1File << fixed << setprecision(7);
    stage2File << fixed << setprecision(7);
    stage3File << fixed << setprecision(7);

    string line;
    getline(inputFile, line); eye = parse_vector3(line);
    getline(inputFile, line); look = parse_vector3(line);
    getline(inputFile, line); up = parse_vector3(line);
    getline(inputFile, line); stringstream(line) >> fovY >> aspectRatio >> near >> far;

    // View matrix
    l = normalize(look - eye);
    r = normalize(cross(l, up));
    u = cross(r, l);

    matrix vT = I;
    vT.vct[3] = vector3(-eye.getX(), -eye.getY(), -eye.getZ(), 1);

    matrix vR = I;
    vR.vct[0] = vector3(r.getX(), u.getX(), -l.getX(), 0);
    vR.vct[1] = vector3(r.getY(), u.getY(), -l.getY(), 0);
    vR.vct[2] = vector3(r.getZ(), u.getZ(), -l.getZ(), 0);

    V = vR * vT;

    // Projection matrix
    double fovX = fovY * aspectRatio;
    fovY = fovY * (M_PI / 180.0);
    fovX = fovX * (M_PI / 180.0);

    double top = near * tan(fovY / 2);
    double right = near * tan(fovX / 2);

    P = I;
    P.vct[0].setX(near / right);
    P.vct[1].setY(near / top);
    P.vct[2].setZ((far + near) / (near - far));
    P.vct[2].setT(-1);
    P.vct[3].setZ((2 * far * near) / (near - far));
    P.vct[3].setT(0);

    M = I;
    vector3 trianglePoints[3];

    while (getline(inputFile, line)){
        if (line == "triangle") {
            for (int i = 0; i < 3; ++i){
                getline(inputFile, line);
                trianglePoints[i] = parse_vector3(line);
            }

            matrix triMatrix(trianglePoints[0], trianglePoints[1], trianglePoints[2], I.vct[3]);
            triMatrix = M * triMatrix;
            triMatrix.scale(); 
            stage1File << triMatrix;

            triMatrix = V * triMatrix;
            triMatrix.scale(); 
            stage2File << triMatrix;

            triMatrix = P * triMatrix;
            triMatrix.scale(); 
            stage3File << triMatrix;

            TriangleSet.push_back(Triangle(triMatrix));
        }
        else if (line == "translate"){
            double tx, ty, tz;
            getline(inputFile, line);
            stringstream(line) >> tx >> ty >> tz;
            apply_translation(tx, ty, tz);
        }
        else if (line == "scale"){
            double sx, sy, sz;
            getline(inputFile, line);
            stringstream(line) >> sx >> sy >> sz;
            apply_scaling(sx, sy, sz);
        }
        else if (line == "rotate"){
            double angleDeg, ax, ay, az;
            getline(inputFile, line);
            stringstream(line) >> angleDeg >> ax >> ay >> az;
            apply_rotation(angleDeg, ax, ay, az);
        }
        else if (line == "push"){
            S.push(M);
        }
        else if (line == "pop"){
            if (!S.empty()){
                M = S.top();
                S.pop();
            }
        }
        else if (line == "end"){
            break;
        }
    }
    inputFile.close();
    stage1File.close();
    stage2File.close();
    stage3File.close();
}
