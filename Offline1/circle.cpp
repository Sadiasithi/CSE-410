#ifdef __linux__
    #include <GL/glut.h>
#elif _WIN32
    #include <windows.h>
    #include <GL/glut.h>
#endif

#include <cmath>

// Angles for animation
float angleSun = 0.0f;
float anglePlanet = 0.0f;
float angleMoon = 0.0f;

// Display function
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(0.0, 0.0, 20.0,   // Camera position
              0.0, 0.0, 0.0,    // Look-at point
              0.0, 1.0, 0.0);   // Up direction

    // --- Draw the Sun ---
    glPushMatrix();
        glColor3f(1.0, 1.0, 0.0); // Yellow
        glutSolidSphere(2.0, 50, 50); // Sun (radius 2)
    glPopMatrix();

    // --- Draw Planet orbiting the Sun ---
    glPushMatrix();
        glRotatef(anglePlanet, 0.0, 1.0, 0.0); // Revolution around Sun
        glTranslatef(7.0, 0.0, 0.0); // Orbit radius
        glColor3f(0.0, 0.0, 1.0); // Blue
        glutSolidSphere(1.0, 30, 30); // Planet (radius 1)

        // --- Draw Moon orbiting the Planet ---
        glRotatef(angleMoon, 0.0, 1.0, 0.0);
        glTranslatef(2.0, 0.0, 0.0); // Moon orbit
        glColor3f(0.5, 0.5, 0.5); // Grey
        glutSolidSphere(0.3, 20, 20); // Moon (radius 0.3)
    glPopMatrix();

    glutSwapBuffers();
}

// Idle function for animation
void update(int value) {
    angleSun += 0.1f;     // Optional: sun rotation (not used in this example)
    anglePlanet += 0.5f;  // Planet revolves around sun
    angleMoon += 2.0f;    // Moon revolves around planet

    if (anglePlanet > 360) anglePlanet -= 360;
    if (angleMoon > 360) angleMoon -= 360;

    glutPostRedisplay();
    glutTimerFunc(16, update, 0); // ~60 FPS
}

// OpenGL initialization
void init() {
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.05f, 1.0f); // Dark space background

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, 800.0 / 600.0, 1.0, 100.0);

    glMatrixMode(GL_MODELVIEW);
}

// Main function
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Solar System Model");

    init();
    glutDisplayFunc(display);
    glutTimerFunc(0, update, 0); // Start animation loop
    glutMainLoop();
    return 0;
}
