#include <bits/stdc++.h>
using namespace std;

#ifdef __linux__
    #include <GL/glut.h>
#elif
    #include <windows.h>
    #include <glut.h>
#endif

#include <math.h>
#include "helper.h"

// Visual attributes
GLfloat faceColors[6][3] = {
    {0.0f, 1.0f, 0.0f},      // Green
    {1.0f, 0.0f, 0.0f},      // Red
    {1.0f, 1.0f, 0.0f},      // Yellow
    {0.0f, 1.0f, 1.0f},      // Cyan
    {0.580f, 0.0f, 0.827f},  // Violet
    {1.0f, 1.0f, 1.0f}       // White
};

// Physics and simulation state
bool simulationRunning = false;
bool showVelocityArrow = false;
bool speedChanged = false;


point ballPosition(0, 0, 0.1);
point ballVelocity(0.0, 0.0, 0.0);
point ballAngularVelocity(0.0, 0.0, 0.0);

float restitution = 0.75;
float initialSpeed = 1.0;
const float gravity = -9.8f;
float ballRadius = 0.1f;
float timeStep = 0.016f;

float cubeSize = 2.5f; 

void resetBall() {
    ballPosition = point(0, 0, ballRadius);
    float angle = (rand() % 360) * M_PI / 180.0;
    float vx = initialSpeed * cos(angle);
    float vy = initialSpeed * sin(angle);
    float vz = initialSpeed;
    ballVelocity = point(vx, vy, vz);
    ballAngularVelocity = point(0, 0, 0);
}

void init() {
    position_of_camera = point(6, 7.19919, 7.14119);
    camera_up = point(0.818987, -0.409701, -0.401753);
    camera_right = point(0.00718421, 0.707414, -0.706763);
    camera_look = point(-0.573767, -0.575943, -0.582305);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(80, 1, 1, 1000);
}

void drawSquare(double a) {
    glBegin(GL_QUADS); {
        glVertex3f(a, a, 0);
        glVertex3f(a, -a, 0);
        glVertex3f(-a, -a, 0);
        glVertex3f(-a, a, 0);
    } glEnd();
}

double speed = 0.1;
double angle = 0.02;

point look_at = camera_look;  

void keyboard(unsigned char key, int a, int b) {
    switch (key) {
        case '1':
            camera_look = RodriGeneral(camera_look, camera_up, -angle);
            camera_right = RodriGeneral(camera_right, camera_up, -angle);
            break;
        case '2':
            camera_look = RodriGeneral(camera_look, camera_up, angle);
            camera_right = RodriGeneral(camera_right, camera_up, angle);
            break;
        case '3':
            camera_look = RodriGeneral(camera_look, camera_right, angle);
            camera_up = RodriGeneral(camera_up, camera_right, angle);
            break;
        case '4':
            camera_look = RodriGeneral(camera_look, camera_right, -angle);
            camera_up = RodriGeneral(camera_up, camera_right, -angle);
            break;
        case '5':
            camera_right = RodriGeneral(camera_right, camera_look, -angle);
            camera_up = RodriGeneral(camera_up, camera_look, -angle);
            break;
        case '6':
            camera_right = RodriGeneral(camera_right, camera_look, angle);
            camera_up = RodriGeneral(camera_up, camera_look, angle);
            break; 
        case 'w': {
            point delta = camera_up.scalarMul(speed);
            position_of_camera = position_of_camera.addition(delta);
            camera_look = look_at.subtraction(position_of_camera);
            break;
        }
        case 's': {
            point delta = camera_up.scalarMul(speed);
            position_of_camera = position_of_camera.subtraction(delta);
            camera_look = look_at.subtraction(position_of_camera);
            break;
        }        
        case ' ':
            simulationRunning = !simulationRunning;
                if (simulationRunning && speedChanged) {
                    ballVelocity.z = initialSpeed;  // Give the vertical kick
                    speedChanged = false;           // Reset flag after the first kick
                }
            break;
        case 'r':
            if (!simulationRunning) {
                // Set ball position to a random location within the cube bounds
                float randX = (rand() % 2001 - 1000) / 1000.0f * (cubeSize - 0.5f - ballRadius);
                float randY = (rand() % 2001 - 1000) / 1000.0f * (cubeSize - 0.5f - ballRadius);
                ballPosition = point(randX, randY, ballRadius);
                
                // Set ball velocity to a random direction with upward velocity
                float angle = (rand() % 360) * M_PI / 180.0;
                float vx = initialSpeed * cos(angle);
                float vy = initialSpeed * sin(angle);
                float vz = initialSpeed;  
                ballVelocity = point(vx, vy, vz);
        
                ballAngularVelocity = point(0, 0, 0);
                // Make sure velocity arrow shows the initial upward direction
                //showVelocityArrow = true;
            }
            break;
        case '+':
            initialSpeed += 1.0f;
            cout << "Initial Speed Increased to: " << initialSpeed << endl;
            speedChanged = true;
            break;
        case '-':
            initialSpeed = max(0.0f, initialSpeed - 1.0f);
            cout << "Initial Speed Decreased to: " << initialSpeed << endl;
            speedChanged = true;
            break;
        case 'v':
            showVelocityArrow = !showVelocityArrow;
            break;
    }
}

void keystrokehandler(int key , int x  , int y) {
    switch (key) {
        case GLUT_KEY_UP:
            position_of_camera = position_of_camera.addition(camera_look.scalarMul(speed));
            break;
        case GLUT_KEY_DOWN:
            position_of_camera = position_of_camera.subtraction(camera_look.scalarMul(speed));
            break;
        case GLUT_KEY_LEFT:
            position_of_camera = position_of_camera.addition(camera_right.scalarMul(speed));
            break;
        case GLUT_KEY_RIGHT:
            position_of_camera = position_of_camera.subtraction(camera_right.scalarMul(speed));
            break;
        case GLUT_KEY_PAGE_UP:
            position_of_camera = position_of_camera.addition(camera_up.scalarMul(speed));
            break;
        case GLUT_KEY_PAGE_DOWN:
            position_of_camera = position_of_camera.subtraction(camera_up.scalarMul(speed));
            break;
    }
}

void drawCheckerboard(double a, int n = 20) {
    double step = (2 * a) / n;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if ((i + j) % 2 == 0)
                glColor3f(1, 1, 1);
            else
                glColor3f(0, 0, 0);

            double x = -a + j * step;
            double y = -a + i * step;

            glBegin(GL_QUADS); {
                glVertex3f(x, y, 0);
                glVertex3f(x + step, y, 0);
                glVertex3f(x + step, y + step, 0);
                glVertex3f(x, y + step, 0);
            } glEnd();
        }
    }
}

void drawStripedBall(float radius, int slices, int stacks) {
    for (int j = 0; j < slices; ++j) {
        float lng0 = 2 * M_PI * j / slices;
        float lng1 = 2 * M_PI * (j + 1) / slices;

        for (int i = 0; i < stacks; ++i) {
            float lat0 = M_PI * (-0.5 + (float)i / stacks);
            float lat1 = M_PI * (-0.5 + (float)(i + 1) / stacks);

            float x00 = cos(lng0) * cos(lat0);
            float y00 = sin(lng0) * cos(lat0);
            float z00 = sin(lat0);

            float x01 = cos(lng0) * cos(lat1);
            float y01 = sin(lng0) * cos(lat1);
            float z01 = sin(lat1);

            float x10 = cos(lng1) * cos(lat0);
            float y10 = sin(lng1) * cos(lat0);
            float z10 = sin(lat0);

            float x11 = cos(lng1) * cos(lat1);
            float y11 = sin(lng1) * cos(lat1);
            float z11 = sin(lat1);

            bool is_upper = (i < stacks / 2);
            bool stripe = (j % 2 == 0);

            if (is_upper) {
                glColor3f(stripe ? 1 : 0, stripe ? 0 : 1, 0);
            } else {
                glColor3f(stripe ? 0 : 1, stripe ? 1 : 0, 0);
            }

            glBegin(GL_QUADS);
            glVertex3f(radius * x00, radius * y00, radius * z00);
            glVertex3f(radius * x10, radius * y10, radius * z10);
            glVertex3f(radius * x11, radius * y11, radius * z11);
            glVertex3f(radius * x01, radius * y01, radius * z01);
            glEnd();
        }
    }
}

void drawBallWithRotation() {
    glPushMatrix();
    glTranslatef(-ballPosition.x, -ballPosition.y, -ballPosition.z);

    // Only apply rotation if the ball is spinning
    if (ballAngularVelocity.magnitude() > 1e-3) {
        float spinAngle = ballAngularVelocity.magnitude() * glutGet(GLUT_ELAPSED_TIME) / 1000.0;
        point axis = ballAngularVelocity.normalize();
        glRotatef(spinAngle * 180 / M_PI, axis.x, axis.y, axis.z);
    }

    drawStripedBall(ballRadius, 20, 20);

    if (showVelocityArrow) {
        point dir = ballVelocity.normalize().scalarMul(0.3);
        glColor3f(1.0f, 1.0f, 0.0f);
        glLineWidth(3.0f); 
    
        // Draw the shaft of the arrow
        glBegin(GL_LINES);
        glVertex3f(0, 0, 0);
        glVertex3f(-dir.x, -dir.y, -dir.z);
        glEnd();

        // Draw the arrowhead
        glPushMatrix();
        // Move to the tip of the arrow (add a small offset from the end of the shaft)
        glTranslatef(-dir.x, -dir.y, -dir.z);
        glRotatef(90, 0.0f, 1.0f, 0.0f);
        glutSolidCone(0.025, 0.05, 10, 10);
        glPopMatrix();
    
        glLineWidth(1.0f); 
    }
    glPopMatrix();
}


void idle() {
    if (simulationRunning) {
        // Update the ball's velocity
        ballVelocity.z += gravity * timeStep;
        point prev = ballPosition;
        ballPosition = ballPosition.addition(ballVelocity.scalarMul(timeStep));

        // Handle collision with the ground
        if (ballPosition.z - ballRadius < 0) {
            ballPosition.z = ballRadius;
            if (fabs(ballVelocity.z) > 0.5)
                ballVelocity.z *= -restitution;
            else
                ballVelocity.z = 0.0;
        }
        // Handle collision with the walls
        if (fabs(ballPosition.x) > cubeSize - 0.5 - ballRadius) {
            ballPosition.x = (ballPosition.x > 0 ? 1 : -1) * (cubeSize - 0.5 - ballRadius);
            ballVelocity.x *= -restitution;
        }
        if (fabs(ballPosition.y) > cubeSize - 0.5 - ballRadius) {
            ballPosition.y = (ballPosition.y > 0 ? 1 : -1) * (cubeSize - 0.5 - ballRadius);
            ballVelocity.y *= -restitution;
        }
        
        // Handle angular velocity for rolling
        point horizontal = point(ballVelocity.x, ballVelocity.y, 0);
        if (horizontal.magnitude() > 1e-3) {
            point axis = point(0, 0, 1).crossMul(horizontal).normalize();
            float angularSpeed = horizontal.magnitude() / ballRadius;
            ballAngularVelocity = axis.scalarMul(angularSpeed);
        }

        // Log ball position and camera position
        //cout << "Ball Position: (" << ballPosition.x << ", " << ballPosition.y << ", " << ballPosition.z << ")" << endl;
        //cout << "Camera Position: (" << position_of_camera.getx() << ", " << position_of_camera.gety() << ", " << position_of_camera.getz() << ")" << endl;
    } 
    else {
        // If simulation is paused, stop spinning the ball
        ballAngularVelocity = point(0.0, 0.0, 0.0);
    }

    glutPostRedisplay();
}

void showCube() {
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(position_of_camera.getx(), position_of_camera.gety(), position_of_camera.getz(),
              position_of_camera.getx() + camera_look.getx(), position_of_camera.gety() + camera_look.gety(), position_of_camera.getz() + camera_look.getz(),
              camera_up.getx(), camera_up.gety(), camera_up.getz());

    GLfloat angles[6] = { 0, 180, 90, -90, 90, -90 };
    GLfloat axes[6][3] = {
        {0, 0, 0},
        {1, 0, 0},
        {1, 0, 0},
        {1, 0, 0},
        {0, 1, 0},
        {0, 1, 0}
    };

    for (int i = 0; i < 6; ++i) {
        glPushMatrix();
        glScalef(1.5, 2.5, 2.5);
        glColor3fv(faceColors[i]);

        if (angles[i] != 0)
            glRotatef(angles[i], axes[i][0], axes[i][1], axes[i][2]);

        glTranslatef(0, 0, 2);

        if (i == 5) {
            drawCheckerboard(2);
            drawBallWithRotation();
        } else {
            drawSquare(2);
        }

        glPopMatrix();
    }
    glutSwapBuffers();
}

void displayControlMenuConsole() {
    cout << "--- 3D Bouncing Ball Controls ---\n";
    cout << "Space: Toggle simulation on/off\n";
    cout << "v: Toggle velocity arrow display\n";
    cout << "+/-: Increase/decrease ball speed (when paused)\n";
    cout << "r: Reset ball to random position (when paused)\n";
    cout << "\n";
    cout << "--- Camera Controls ---\n";
    cout << "Arrow keys: Move camera left/right/forward/backward\n";
    cout << "Page Up/Down: Move camera up/down\n";
    cout << "w/s: Move camera up/down (keeping reference point)\n";
    cout << "1/2: Look left/right\n";
    cout << "3/4: Look up/down\n";
    cout << "5/6: Tilt camera clockwise/counterclockwise\n";
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowPosition(1000, 400);
    glutInitWindowSize(800, 800);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("Camera Movement & Ball Simulation");
    displayControlMenuConsole();
    init();
    resetBall();
    glutIdleFunc(idle);
    glutSpecialFunc(keystrokehandler);
    glutKeyboardFunc(keyboard);
    glutDisplayFunc(showCube);
    glutMainLoop();

    return 0;
}
