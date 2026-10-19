#include <bits/stdc++.h>
#include <ctime>
#include <cstdlib>
#include <chrono>
#include <GL/glut.h>
#include <math.h>
#include "2005119_classes.h"
#include "bitmap_image.hpp"
using namespace std;

// Global variables
int recursion_level;
int number_of_pixels;
int number_of_objects;
int number_of_pointLights;
int number_of_spotLights;
bool useFloorTexture = false;
string floorTextureFile = "";

vector<Object*> objects;
vector<PointLight> point_lights;
vector<SpotLight> spot_lights;

double floorwidth = 1000.0;
double tile_width = 50.0;
double cameraGlutAngle = 80.0;
int imageCount = 1;

//Camera vectors
Vector3D position_of_camera;
Vector3D camera_up;
Vector3D camera_look;
Vector3D camera_right;

// Utility to show camera vectors
void showVector3DInfo() {
    camera_up.show("camera_up");
    camera_look.show("camera_look");
    camera_right.show("camera_right");
    position_of_camera.show("position_of_camera");
}

// Capture the current scene to an image
void capture() {
    cout << "capturing starts.." << endl;
    int capture_pixels = max(800, number_of_pixels);  
    bitmap_image bitImage(capture_pixels, capture_pixels);
    for(int i = 0; i < capture_pixels; i++) {
        for(int j = 0; j < capture_pixels; j++) {
            bitImage.set_pixel(i, j, 0, 0, 0);
        }
    }
    string imageName = "Output_1" + to_string(imageCount) + ".bmp";
    double window_width = 500.0;
    double window_height = 500.0;
    double viewAngle = 80.0;
    double planeDistance = (window_height/2.0) / tan((viewAngle * M_PI / 180.0) / 2.0);
    Vector3D center = position_of_camera.addition(camera_look.scalarMul(planeDistance));
    Vector3D left_offset = camera_right.scalarMul(window_width / 2.0);
    Vector3D up_offset = camera_up.scalarMul(window_height / 2.0);
    Vector3D topLeft = center.subtraction(left_offset).addition(up_offset);
    double du = window_width / number_of_pixels;
    double dv = window_height / number_of_pixels;
    topLeft = topLeft.addition(camera_right.scalarMul(du/2.0)).subtraction(camera_up.scalarMul(dv/2.0));
    cout << "Starting ray tracing..." << endl;
    for(int i = 0; i < number_of_pixels; i++) {
        if(i % (number_of_pixels/10) == 0) {
            cout << "Processing row " << i << "/" << number_of_pixels << endl;
        }
        for(int j = 0; j < number_of_pixels; j++) {
            Vector3D current_pixel = topLeft.addition(camera_right.scalarMul(i*du)).subtraction(camera_up.scalarMul(j*dv));
            Ray ray(position_of_camera, current_pixel.subtraction(position_of_camera));
            double tMin = 1e9;
            int nearest = -1;
            int obj_count = 0;
            for(Object *obj : objects) {
                Color color;
                double t = obj->intersect(ray, color, 0);
                if(t > 0 && t < tMin) {
                    tMin = t;
                    nearest = obj_count;
                }
                obj_count++;
            }
            if(nearest != -1) {
                Color color;
                Object *nearestObject = objects[nearest];
                tMin = nearestObject->intersect(ray, color, 1);
                color.normalize();
                bitImage.set_pixel(i, j, color.r*255, color.g*255, color.b*255);
            }
        }
    }
    bitImage.save_image(imageName);
    cout << "Image saved as " << imageName << endl;
    imageCount++;
}

// Add the floor to the scene
void pushFloor() {
    Vector3D start = Vector3D(-floorwidth/2, -floorwidth/2, 0);
    Color color = Color(0, 0, 0);
    Object *temp;
    
    if (useFloorTexture && !floorTextureFile.empty()) {
        temp = new Floor(start, floorwidth, tile_width, color, 0.5, 0.4, 0.3, 0.5, 0.6, floorTextureFile.c_str());
    } else {
        temp = new Floor(start, floorwidth, tile_width, color, 0.5, 0.4, 0.3, 0.5, 0.6);
    }
    objects.push_back(temp);
}

void recreateFloor() {
    for (int i = 0; i < objects.size(); i++) {
        Floor* floor = dynamic_cast<Floor*>(objects[i]);
        if (floor != nullptr) {
            delete objects[i];
            objects.erase(objects.begin() + i);
            break;
        }
    }
    pushFloor();
}

// GLUT keyboard handler for camera rotation
void keyboard(unsigned char key, int a, int b) {
    showVector3DInfo();
    double rotation_angle = 0.1;
    switch (key) {
        case '0': capture(); break;
        case '1':
            camera_look = RodriGeneral(camera_look, camera_up, -rotation_angle);
            camera_right = RodriGeneral(camera_right, camera_up, -rotation_angle);
            break;
        case '2':
            camera_look = RodriGeneral(camera_look, camera_up, rotation_angle);
            camera_right = RodriGeneral(camera_right, camera_up, rotation_angle);
            break;
        case '3':
            camera_look = RodriGeneral(camera_look, camera_right, rotation_angle);
            camera_up = RodriGeneral(camera_up, camera_right, rotation_angle);
            break;
        case '4':
            camera_look = RodriGeneral(camera_look, camera_right, -rotation_angle);
            camera_up = RodriGeneral(camera_up, camera_right, -rotation_angle);
            break;
        case '6':
            camera_right = RodriGeneral(camera_right, camera_look, rotation_angle);
            camera_up = RodriGeneral(camera_up, camera_look, rotation_angle);
            break;
        case '5':
            camera_right = RodriGeneral(camera_right, camera_look, -rotation_angle);
            camera_up = RodriGeneral(camera_up, camera_look, -rotation_angle);
            break;
        case 't': case 'T':
            // Toggle texture mode
            useFloorTexture = !useFloorTexture;
            if (useFloorTexture) {
                floorTextureFile = "brick_texture.bmp";
                cout << "Switched to texture mode using: " << floorTextureFile << endl;
            } else {
                cout << "Switched to checkerboard mode" << endl;
            }
            recreateFloor();
            break;
        case 'b': case 'B':
            useFloorTexture = true;
            floorTextureFile = "wall_texture.bmp";
            cout << "Switched to brick texture: " << floorTextureFile << endl;
            recreateFloor();
            break;
        case 'f': case 'F':
            useFloorTexture = true;
            floorTextureFile = "floor_texture.bmp";
            cout << "Switched to floor texture: " << floorTextureFile << endl;
            recreateFloor();
            break;
        case 'c': case 'C':
            useFloorTexture = false;
            cout << "Switched to checkerboard mode" << endl;
            recreateFloor();
            break;
        case ' ': exit(0); break;
        default: break;
    }
}

// GLUT special key handler for camera movement
void keystrokehandler(int key, int x, int y) {
    double move_step = 4.0;
    switch (key) {
        case GLUT_KEY_UP:
            position_of_camera = position_of_camera.addition(camera_look.scalarMul(move_step));
            break;
        case GLUT_KEY_DOWN:
            position_of_camera = position_of_camera.subtraction(camera_look.scalarMul(move_step));
            break;
        case GLUT_KEY_LEFT:
            position_of_camera = position_of_camera.addition(camera_right.scalarMul(move_step));
            break;
        case GLUT_KEY_RIGHT:
            position_of_camera = position_of_camera.subtraction(camera_right.scalarMul(move_step));
            break;
        case GLUT_KEY_PAGE_UP:
            position_of_camera = position_of_camera.addition(camera_up.scalarMul(move_step));
            break;
        case GLUT_KEY_PAGE_DOWN:
            position_of_camera = position_of_camera.subtraction(camera_up.scalarMul(move_step));
            break;
        default: break;
    }
}

// GLUT display function
void RayTracing() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(position_of_camera.getx(), position_of_camera.gety(), position_of_camera.getz(),
              position_of_camera.getx() + camera_look.getx(),
              position_of_camera.gety() + camera_look.gety(),
              position_of_camera.getz() + camera_look.getz(),
              camera_up.getx(), camera_up.gety(), camera_up.getz());
    glPushMatrix();
    for(Object *o : objects) {
        glPushMatrix();
        glTranslatef(o->reference_point.x, o->reference_point.y, o->reference_point.z);
        o->draw();
        glPopMatrix();
    }
    for(PointLight pl : point_lights) pl.draw();
    for(SpotLight sl : spot_lights) sl.draw();
    glPopMatrix();
    glutSwapBuffers();
}

void idle() { glutPostRedisplay(); }
void timer(int val) {}

// Load scene data from file
void loaddata() {
    ifstream fin("scene.txt");
    pushFloor();
    fin >> recursion_level;
    fin >> number_of_pixels;
    fin >> number_of_objects;
    for(int i = 0; i < number_of_objects; i++) {
        string objectType;
        fin >> objectType;
        if(objectType == "sphere") {
            Sphere s;
            fin >> s;
            Object *temp = new Sphere(s.center, s.radius, s.color, s.ambient, s.diffuse, s.specular, s.reflection_coeff, s.shine);
            temp->type = "sphere";
            objects.push_back(temp);
        } 
        else if(objectType == "triangle") {
            Triangle t;
            fin >> t;
            Object *temp = new Triangle(t.point1, t.point2, t.point3, t.color, t.ambient, t.diffuse, t.specular, t.reflection_coeff, t.shine);
            temp->type = "triangle";
            objects.push_back(temp);
        } 
        else if(objectType == "general") {
            GeneralQuadSurface general;
            fin >> general;
            Object *temp = new GeneralQuadSurface(general.a, general.b, general.c, general.d, general.e, general.f, general.g, general.h, general.i, general.j, 
                                                  general.cube_reference_point, general.length, general.width, general.height, general.color, 
                                                  general.ambient, general.diffuse, general.specular, general.reflection_coeff, general.shine);
            temp->type = "general";
            objects.push_back(temp);
        }
    }
    fin >> number_of_pointLights;
    for(int i = 0; i < number_of_pointLights; i++) {
        PointLight pl;
        fin >> pl;
        point_lights.push_back(pl);
    }
    fin >> number_of_spotLights;
    for(int i = 0; i < number_of_spotLights; i++) {
        SpotLight sl;
        fin >> sl;
        spot_lights.push_back(sl);
    }
}

// Initialize OpenGL and camera
void init() {
    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(cameraGlutAngle, 1, 1, 1000);
    // Set initial camera position and orientation
    position_of_camera = Vector3D(100, 100, 50);
    camera_look = Vector3D(-0.707, -0.707, 0);
    camera_right = Vector3D(-0.707, 0.707, 0);
    camera_up = Vector3D(0, 0, 1);
}

// Main function
int main(int argc, char** argv) {
    loaddata();
    
    cout << "\n=== Floor Texture Controls ===" << endl;
    cout << "T/t: Toggle between texture and checkerboard" << endl;
    cout << "B/b: Switch to wall texture" << endl;
    cout << "F/f: Switch to floor texture" << endl;
    cout << "C/c: Switch to checkerboard pattern" << endl;
    cout << "0: Capture image" << endl;
    cout << "Space: Exit" << endl;
    cout << "=============================\n" << endl;
    
    glutInit(&argc, argv);
    glutInitWindowPosition(1000, 200);
    glutInitWindowSize(600, 600);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("Ray tracing");
    init();
    glutSpecialFunc(keystrokehandler);
    glutKeyboardFunc(keyboard);
    glutDisplayFunc(RayTracing);
    glutTimerFunc(50, timer, 0);
    glutIdleFunc(idle);
    glutMainLoop();
    for(Object *o : objects) delete o;
    point_lights.clear();
    spot_lights.clear();
    return 0;
}