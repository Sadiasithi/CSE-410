#include <GL/glut.h>
#include <math.h>
#include <ctime>
#include <chrono>

#define WINDOW_WIDTH 512
#define WINDOW_HEIGHT 512

#define PI acos(-1.0)
#define degToRad(x) (x * PI / 180.0)
#define radToDeg(x) (x * 180.0 / PI)

typedef struct {
    double x, y;
} Point;

void drawCircle(Point center, double radius, int segments = 360) {
    glColor3f(1.0f, 1.0f, 1.0f);
    glLineWidth(1.0f);
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < segments; i++) {
        double theta = 2.0 * PI * i / segments;
        double x = radius * cos(theta);
        double y = radius * sin(theta);
        glVertex2d(x + center.x, y + center.y);
    }
    glEnd();
}

void drawClockTicks(Point center, double radius, int tickCount = 60) {
    for (int i = 0; i < tickCount; i++) {
        double angleDeg = i * 360.0 / tickCount;
        double angleRad = degToRad(angleDeg);
        double outerX = center.x + radius * cos(angleRad);
        double outerY = center.y + radius * sin(angleRad);
        double innerRadius = (i % 5 == 0) ? radius - 0.05 : radius - 0.025;

        double innerX = center.x + innerRadius * cos(angleRad);
        double innerY = center.y + innerRadius * sin(angleRad);

        glLineWidth((i % 5 == 0) ? 1.0f : 1.5f);
        glColor3f(1.0f, 1.0f, 1.0f);

        glBegin(GL_LINES);
        glVertex2d(innerX, innerY);
        glVertex2d(outerX, outerY);
        glEnd();
    }
}

void drawSquare(Point center, double size) {
    double halfSize = size / 2.0;
    glBegin(GL_QUADS);
    glVertex2d(center.x - halfSize, center.y - halfSize);
    glVertex2d(center.x + halfSize, center.y - halfSize);
    glVertex2d(center.x + halfSize, center.y + halfSize);
    glVertex2d(center.x - halfSize, center.y + halfSize);
    glEnd();
}

Point pointOnCircle(Point center, double radius, double angleDeg) {
    double angleRad = degToRad(angleDeg);
    return {
        center.x + radius * cos(angleRad),
        center.y + radius * sin(angleRad)
    };
}

void drawLine(Point a, double length, double theta, double width = 5) {
    glLineWidth(width);
    glBegin(GL_LINES);
    glVertex2d(a.x, a.y);
    theta = degToRad(theta);
    glVertex2d(a.x + length * cos(theta), a.y + length * sin(theta));
    glEnd();
}

void drawClockHands(Point clockCenter, double hDeg, double mDeg, double sDeg) {
    double hourHandLength = 0.25;
    double minuteHandLength = 0.3;
    double secondHandLength = 0.35;
    double hourHandWidth = 5;
    double minuteHandWidth = 3;
    double secondHandWidth = 2;
    double squareSize = 0.04;
    double circleRadius = 0.5;

    // Draw hands
    glColor3f(1.0, 1.0, 1.0);
    drawLine(clockCenter, hourHandLength, hDeg, hourHandWidth);

    glColor3f(1.0, 1.0, 1.0);
    drawLine(clockCenter, minuteHandLength, mDeg, minuteHandWidth);

    glColor3f(1.0, 0.0, 0.0);
    drawLine(clockCenter, secondHandLength, sDeg, secondHandWidth);

    // Draw squares on the circle
    Point hPos = pointOnCircle(clockCenter, circleRadius, hDeg);
    glColor3f(1.0f, 1.0f, 1.0f);
    drawSquare(hPos, squareSize);

    Point mPos = pointOnCircle(clockCenter, circleRadius, mDeg);
    glColor3f(1.0f, 1.0f, 1.0f);
    drawSquare(mPos, squareSize);

    Point sPos = pointOnCircle(clockCenter, circleRadius, sDeg);
    glColor3f(1.0, 0.3, 0.3);
    drawSquare(sPos, squareSize);
}

void drawClock(double hour, double minute, double second) {
    Point center = {0, 0};
    double hDeg = 90 - (((int)hour % 12) + (minute / 60)) * 30;
    double mDeg = 90 - (minute + second / 60) * 6;
    double sDeg = 90 - (second * 6);

    drawCircle(center, 0.5);
    drawClockTicks(center, 0.48);
    drawClockHands(center, hDeg, mDeg, sDeg);
}

void display() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    using namespace std::chrono;

    auto now = system_clock::now();
    time_t now_c = system_clock::to_time_t(now);
    tm* localTime = localtime(&now_c);
    auto ms = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;

    double hour = localTime->tm_hour;
    double minute = localTime->tm_min;
    double second = localTime->tm_sec + ms.count() / 1000.0;

    drawClock(hour, minute, second);
    glFlush();
}

void idle() {
    glutPostRedisplay(); // Continuously update
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow("Analogue Clock");

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1, 1, -1, 1);

    glutDisplayFunc(display);
    glutIdleFunc(idle);

    glutMainLoop();
    return 0;
}
