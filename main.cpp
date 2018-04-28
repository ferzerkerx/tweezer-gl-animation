#include <iostream>
#include <cstdio>
#include <GL/glut.h>
#include <memory.h>

#define SMALL_SEGMENT_HEIGHT 3
#define BIG_SEGMENT_HEIGHT 5

int slices = 50;
int stacks = 50;

float xpos = 0.0f;
float delta = 0.5;
bool anim = true;
GLUquadricObj *sphere;
GLUquadricObj *cylinder;

float bigSegmentAngle = 90.0f;
float smallSegmentAngle = 0.0f;

GLfloat light0_specular[] = {0.5f, 0.5f, 0.5f, 0.5f};
GLfloat light0_ambient[] = {0.1f, 0.1f, 0.3f, 1.0f};
GLfloat light0_diffuse[] = {.6f, .6f, 1.0f, 1.0f};
GLint light0_position[] = {0, 4, 0, 0};

GLfloat mat_ambient[] = {0.7f, 0.7f, 0.7f, 1.0f};
GLfloat mat_diffuse[] = {1.0f, 1.0f, 1.0f, 0.0f};
GLfloat mat_specular[] = {0.3f, 0.3f, 0.3f, 1.0f};
GLfloat high_shininess[] = {80.0f};

void drawTweezer();

void drawCylinderSegment(int height);

void drawSphere();

void updateScene();

void drawTweezerSection(float bigSegmentAngle, float smallSegmentAngle);

void drawLeftTweezerPart();

void drawRightTweezerPart();

void processKeyEvents(int key, int mouseX, int mouseY);

void resize(int width, int height);

void myTimer(int i);

void renderScene();

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(640, 480);
    glutCreateWindow("Tweezer");

    glutReshapeFunc(resize);
    glutDisplayFunc(renderScene);
    glutSpecialFunc(processKeyEvents);

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);

    glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
    glLightiv(GL_LIGHT0, GL_POSITION, light0_position);

    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);


    glutTimerFunc(10, myTimer, 1);
    drawTweezer();

    glutMainLoop();
    gluDeleteQuadric(sphere);
    gluDeleteQuadric(cylinder);
}

void resize(int width, int height) {
    const float ar = (float) width / (float) height;

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(30.0, ar, 1.0, 100.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void myTimer(int i) {
    updateScene();

    glutTimerFunc(10, myTimer, 1);
    glutPostRedisplay();
}

void updateScene() {
    if (anim) {
        if (bigSegmentAngle > 45.0f) {
            bigSegmentAngle -= delta;
        }
        if (smallSegmentAngle < 90.0f) {
            smallSegmentAngle += delta;
        }

    } else {
        if (bigSegmentAngle < 90.0f) {
            bigSegmentAngle += delta;
        }
        if (smallSegmentAngle > 0.0f) {
            smallSegmentAngle -= delta;
        }
    }
}

void renderScene() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    glShadeModel(GL_SMOOTH);
    gluLookAt(0, 0, 35, 0, 0, 0, 0, 1, 0);

    glTranslatef(xpos, 0.0, 0.0);

    glTranslated(0, 7, 0);
    drawSphere();

    glTranslated(0, -0.5, 0);
    drawCylinderSegment(SMALL_SEGMENT_HEIGHT);

    glTranslated(0, -SMALL_SEGMENT_HEIGHT, 0);
    drawSphere();

    glPushMatrix();
    drawLeftTweezerPart();
    glTranslated(0.2, -0.0, 0);
    drawRightTweezerPart();
    glPopMatrix();

    glutSwapBuffers();
}

void drawRightTweezerPart() { drawTweezerSection(bigSegmentAngle, -smallSegmentAngle); }

void drawLeftTweezerPart() { drawTweezerSection(-bigSegmentAngle, smallSegmentAngle); }

void drawTweezerSection(float bigSegmentAngle, float smallSegmentAngle) {
    glPushMatrix();
        glTranslated(-0.2, -0.3, 0);
        glRotated(bigSegmentAngle, 0, 0, 1);
        drawCylinderSegment(BIG_SEGMENT_HEIGHT);
        glTranslated(0, -BIG_SEGMENT_HEIGHT, 0);
        drawSphere();

        glPushMatrix();
            glRotated(smallSegmentAngle, 0, 0, 1);
            glTranslated(0, -0.5, 0);
            drawCylinderSegment(SMALL_SEGMENT_HEIGHT);
            glTranslated(0, -SMALL_SEGMENT_HEIGHT, 0);
            drawSphere();
        glPopMatrix();
    glPopMatrix();
}

void drawCylinderSegment(int height) {
    glPushMatrix();
    glRotated(90, 1, 0, 0);
    gluCylinder(cylinder, 0.5, 0.5, height, slices, stacks);
    glPopMatrix();
}

void drawSphere() {
    glPushMatrix();
    glRotated(90, 1, 0, 0);
    gluSphere(sphere, 0.7, slices, stacks);
    glPopMatrix();
}


void processKeyEvents(int key, int mouseX, int mouseY) {
    switch (key) {
        case GLUT_KEY_DOWN:
            anim = true;
            break;
        case GLUT_KEY_UP:
            anim = false;
            break;
        case GLUT_KEY_LEFT:
            xpos -= delta;
            break;
        case GLUT_KEY_RIGHT:
            xpos += delta;
            break;
    }
    glutPostRedisplay();
}

void drawTweezer() {
    sphere = gluNewQuadric();
    gluQuadricDrawStyle(sphere, GLU_FILL);
    gluQuadricNormals(sphere, GLU_SMOOTH);

    cylinder = gluNewQuadric();
    gluQuadricDrawStyle(cylinder, GLU_FILL);
    gluQuadricNormals(cylinder, GLU_SMOOTH);
}