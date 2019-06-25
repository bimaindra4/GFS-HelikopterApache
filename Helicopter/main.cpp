#include "windows.h"
#include "GL/glut.h"
#include "math.h"
#define checkImageWidth 64
#define checkImageHeight 64

float rx=-90.0f, ry=0.0f, rz=-180.0f;
float p=3.0f, l=10.0f, t=3.5f;

GLUquadric *q = gluNewQuadric();

static GLubyte checkImage[checkImageHeight][checkImageWidth][4];
static GLuint texName;

void makeCheckImage(void) {
   int i, j, c;

   for (i = 0; i < checkImageHeight; i++) {
      for (j = 0; j < checkImageWidth; j++) {
         c = (((((i&0x8)==0)^((j&0x8)))==0))*255;
         checkImage[i][j][0] = (GLubyte) c;
         checkImage[i][j][1] = (GLubyte) c;
         checkImage[i][j][2] = (GLubyte) c;
         checkImage[i][j][3] = (GLubyte) 255;
      }
   }
}

void initGL() {
  GLfloat sun_direction[] = { 0.0, 2.0, -1.0, 1.0};
  GLfloat sun_intensity[] = {0.7, 0.7, 0.7, 1.0};
  GLfloat ambient_intensity[] = { 0.3, 0.3, 0.3, 1.0 };

  glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // set clear color to white
  glClearDepth(1.0f);
  glEnable(GL_DEPTH_TEST); // draw only closest surface
  glDepthFunc(GL_LEQUAL);
  glShadeModel(GL_SMOOTH);
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

  glEnable(GL_LIGHTING);
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient_intensity);

  glEnable(GL_LIGHT0);                // Set up sunlight.
  glLightfv(GL_LIGHT0, GL_POSITION, sun_direction);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, sun_intensity);

  glEnable(GL_COLOR_MATERIAL);        // Configure glColor().
  glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

  makeCheckImage();
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glGenTextures(1, &texName);

  glBindTexture(GL_TEXTURE_2D, texName);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, checkImageWidth,
    checkImageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE,
    checkImage);
}

void timer(int value) {
  glutPostRedisplay();
  glutTimerFunc(15, timer, 0);
}

void reshape(GLsizei width, GLsizei height) {
  if (height == 0)
    height = 1;
  GLfloat aspect = (GLfloat)width / (GLfloat)height;
  glViewport(0, 0, width, height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45.0f, aspect, 0.1f, 100.0f);
}

void persegi(float p, float l, float t=0) {
    glColor3f(0,0,0);
    glBegin(GL_QUADS);
        glVertex3f(0,0,t);
        glVertex3f(p,0,t);
        glVertex3f(p,l,t);
        glVertex3f(0,l,t);
    glEnd();
}

void kubus(float p, float l, float t) {
    persegi(p,t,0); // ABCD
    persegi(p,t,l); // EFGH
    glBegin(GL_QUADS); // FBCG
        glVertex3f(p,0,l);
        glVertex3f(p,0,0);
        glVertex3f(p,t,0);
        glVertex3f(p,t,l);
    glEnd();
    glBegin(GL_QUADS); // EADH
        glVertex3f(0,0,l);
        glVertex3f(0,0,0);
        glVertex3f(0,t,0);
        glVertex3f(0,t,l);
    glEnd();
    glBegin(GL_QUADS); // HGCD
        glVertex3f(0,t,l);
        glVertex3f(p,t,l);
        glVertex3f(p,t,0);
        glVertex3f(0,t,0);
    glEnd();
    glBegin(GL_QUADS); // EFBA
        glVertex3f(0,0,l);
        glVertex3f(p,0,l);
        glVertex3f(p,0,0);
        glVertex3f(0,0,0);
    glEnd();
}

void kerucut(float rb, float rt, float t) {
    float BODY_LENGTH = t;
    float BODY_RADIUS_BOTTOM = rb;
    float BODY_RADIUS_TOP = rt;
    int SLICES = 120;
    int STACKS = 120;
    GLUquadric *q = gluNewQuadric();
    gluCylinder(q, BODY_RADIUS_BOTTOM, BODY_RADIUS_TOP, BODY_LENGTH, SLICES, STACKS);
    gluDisk(q, 0.0f, BODY_RADIUS_BOTTOM, SLICES, STACKS);
    glTranslatef(0.0f, 0.0f, BODY_LENGTH);
    gluDisk(q, 0.0f, BODY_RADIUS_TOP, SLICES, STACKS);
}

void weapon_sts() {
    float pw=2.0f, lw=1.0f, tw=1.0f;

    // weapon kanan
    glPushMatrix();
        glTranslatef(-lw, t-tw, l-pw-0.5f);
        kubus(lw, pw, tw);
        glBegin(GL_QUADS); // buntut weapon kanan
            glVertex3f(0,0,0);
            glVertex3f(0,0,-(pw/4*3));
            glVertex3f(0,tw,0);
        glEnd();
        glBegin(GL_QUADS); // buntut weapon kiri
            glVertex3f(lw,0,0);
            glVertex3f(lw,0,-(pw/4*3));
            glVertex3f(lw,tw,0);
        glEnd();
        glBegin(GL_QUADS); // buntut weapon atas
            glVertex3f(0,0,0);
            glVertex3f(lw,0,0);
            glVertex3f(lw,0,-(pw/4*3));
            glVertex3f(0,0,-(pw/4*3));
        glEnd();
        glBegin(GL_QUADS); // buntut weapon bawah
            glVertex3f(0,tw,0);
            glVertex3f(lw,tw,0);
            glVertex3f(lw,0,-(pw/4*3));
            glVertex3f(0,0,-(pw/4*3));
        glEnd();

        glTranslatef(lw/2, tw/2, 1.25f);
        kerucut(lw/2, 0.15f, 1.25f);
    glPopMatrix();

    // weapon kiri
    glPushMatrix();
        glTranslatef(p-0.5f, t-tw, l-pw-0.5f);
        kubus(lw, pw, tw);
        glBegin(GL_QUADS); // buntut weapon kanan
            glVertex3f(0,0,0);
            glVertex3f(0,0,-(pw/4*3));
            glVertex3f(0,tw,0);
        glEnd();
        glBegin(GL_QUADS); // buntut weapon kiri
            glVertex3f(lw,0,0);
            glVertex3f(lw,0,-(pw/4*3));
            glVertex3f(lw,tw,0);
        glEnd();
        glBegin(GL_QUADS); // buntut weapon atas
            glVertex3f(0,0,0);
            glVertex3f(lw,0,0);
            glVertex3f(lw,0,-(pw/4*3));
            glVertex3f(0,0,-(pw/4*3));
        glEnd();
        glBegin(GL_QUADS); // buntut weapon bawah
            glVertex3f(0,tw,0);
            glVertex3f(lw,tw,0);
            glVertex3f(lw,0,-(pw/4*3));
            glVertex3f(0,0,-(pw/4*3));
        glEnd();

        glTranslatef(lw/2, tw/2, 1.25f);
        kerucut(lw/2, 0.15f, 1.25f);
    glPopMatrix();
}

void sayap() {
    float ps=5.0f, ls=2.5f, ts=0.1f;

    // sayap kanan
    glPushMatrix();
        glTranslatef(0.0f, (t/2), (l/3)+0.5f);
        glBegin(GL_QUADS); // bag atas
            glVertex3f(0,0,0);
            glVertex3f(-ps,0,0);
            glVertex3f(-ps,0,(ls/2));
            glVertex3f(0,0,ls);
        glEnd();

        // tutupan e
        glBegin(GL_QUADS);
            glVertex3f(0,0,0);
            glVertex3f(-ps,0,0);
            glVertex3f(-ps,ts,0);
            glVertex3f(0,ts,0);
        glEnd();
        glBegin(GL_QUADS);
            glVertex3f(0,0,0);
            glVertex3f(0,ts,0);
            glVertex3f(0,ts,ls);
            glVertex3f(0,0,ls);
        glEnd();
        glBegin(GL_QUADS);
            glVertex3f(0,0,ls);
            glVertex3f(0,ts,ls);
            glVertex3f(-ps,ts,(ls/2));
            glVertex3f(-ps,0,(ls/2));
        glEnd();
        glBegin(GL_QUADS);
            glVertex3f(-ps,0,(ls/2));
            glVertex3f(-ps,ts,(ls/2));
            glVertex3f(-ps,ts,0);
            glVertex3f(-ps,0,0);
        glEnd();

        glBegin(GL_QUADS); // bag bawah
            glVertex3f(0,ts,0);
            glVertex3f(-ps,ts,0);
            glVertex3f(-ps,ts,(ls/2));
            glVertex3f(0,ts,ls);
        glEnd();
    glPopMatrix();

    // sayap kiri
    glPushMatrix();
        glTranslatef(p, (t/2), (l/3)+0.5f);
        glRotatef(180, 0.0f, 0.0f, 1.0f);

        glBegin(GL_QUADS); // bag atas
            glVertex3f(0,0,0);
            glVertex3f(-ps,0,0);
            glVertex3f(-ps,0,(ls/2));
            glVertex3f(0,0,ls);
        glEnd();

        // tutupan e
        glBegin(GL_QUADS);
            glVertex3f(0,0,0);
            glVertex3f(-ps,0,0);
            glVertex3f(-ps,ts,0);
            glVertex3f(0,ts,0);
        glEnd();
        glBegin(GL_QUADS);
            glVertex3f(0,0,0);
            glVertex3f(0,ts,0);
            glVertex3f(0,ts,ls);
            glVertex3f(0,0,ls);
        glEnd();
        glBegin(GL_QUADS);
            glVertex3f(0,0,ls);
            glVertex3f(0,ts,ls);
            glVertex3f(-ps,ts,(ls/2));
            glVertex3f(-ps,0,(ls/2));
        glEnd();
        glBegin(GL_QUADS);
            glVertex3f(-ps,0,(ls/2));
            glVertex3f(-ps,ts,(ls/2));
            glVertex3f(-ps,ts,0);
            glVertex3f(-ps,0,0);
        glEnd();

        glBegin(GL_QUADS); // bag bawah
            glVertex3f(0,ts,0);
            glVertex3f(-ps,ts,0);
            glVertex3f(-ps,ts,(ls/2));
            glVertex3f(0,ts,ls);
        glEnd();
    glPopMatrix();
}

void kepala() {
    glPushMatrix();
      glColor3d(0,1,0);
      glTranslated(-1,0,-10);
      glRotated(90,0,1,0);
      gluCylinder(q, 2, 0.5, 2, 5, 10);
      glTranslated(0,0,-3);
      gluCylinder(q, 2, 2, 3, 5, 10);
      glTranslated(0,0,-1.5);
      gluCylinder(q, 1, 2, 1.5, 5, 1);
      glTranslated(0,0,-0.4);
      gluCylinder(q, 0, 1, 0.4, 5, 1);
    glPopMatrix();
}

void sikilan() {
    glPushMatrix();
        glTranslated(-4,-1.5,-11);
        glRotated(90,1,0,0);
        glRotated(45,0,1,0);
    gluCylinder(q, 0.05, 0.05, 1, 10, 10);
    glPopMatrix();
    glPushMatrix();
        glTranslated(-2,-1.5,-11);
        glRotated(90,1,0,0);
        glRotated(45,0,1,0);
    gluCylinder(q, 0.05, 0.05, 1, 10, 10);
    glPopMatrix();
    glPushMatrix();
        glTranslated(-4,-1.5,-9);
        glRotated(90,1,0,0);
        glRotated(45,0,1,0);
    gluCylinder(q, 0.05, 0.05, 1, 10, 10);
    glPopMatrix();
    glPushMatrix();
        glTranslated(-2,-1.5,-9);
        glRotated(90,1,0,0);
        glRotated(45,0,1,0);
    gluCylinder(q, 0.05, 0.05, 1, 10, 10);
    glPopMatrix();
    glPushMatrix();
        glTranslated(-1.3,-2.2,-11);
    gluCylinder(q, 0.05, 0.05, 2, 10, 10);
        glTranslated(-2,0,0);
    gluCylinder(q, 0.05, 0.05, 2, 10, 10);
        glRotated(90,0,1,0);
    gluCylinder(q, 0.05, 0.05, 2, 10, 10);
        glTranslated(-2,0,0);
    gluCylinder(q, 0.05, 0.05, 2, 10, 10);
    glPopMatrix();
}

void ekor() {
    glPushMatrix();
    glColor3d(0,1,0);
        glTranslated(0,0,-10);
        glRotated(90,0,1,0);
    gluCylinder(q, 1, 0, 6, 4, 8);
    glPopMatrix();
    glPushMatrix();
    glColor3d(0,1,0);
        glTranslated(5.4,1,-10);
        glBegin(GL_QUADS);
            glVertex3f(0,1,0);
            glVertex3f(1,1,0);
            glVertex3f(0,-1,0);
            glVertex3f(-1,-1,0);
        glEnd();
    glPopMatrix();
    glPushMatrix();
    glColor3d(0,1,0);
        glTranslated(5.4,0,-10);
        glBegin(GL_QUADS);
            glVertex3f(-0.5,0,1.5);
            glVertex3f(0.5,0,1.5);
            glVertex3f(0.5,0,-1.5);
            glVertex3f(-0.5,0,-1.5);
        glEnd();
    glPopMatrix();
}

void axis() {
    glTranslatef(1,1,1);
    glBegin(GL_LINES);
        glColor3f(1,0,0);
        glVertex3f(0,0,0);
        glVertex3f(2,0,0);

        glColor3f(0,1,0);
        glVertex3f(0,0,0);
        glVertex3f(0,2,0);

        glColor3f(0,0,1);
        glVertex3f(0,0,0);
        glVertex3f(0,0,2);
    glEnd();
}

void display() {
  const double t = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(4,4,4, 0,0,0, 0,0,1);

  axis(); // primary axis
  glTranslatef(-10.0f, -14.0f, -8.0f);
  glRotatef(rx, 1.0f, 0.0f, 0.0f);
  glRotatef(ry, 0.0f, 1.0f, 0.0f);
  glRotatef(rz, 0.0f, 0.0f, 1.0f);
  glTranslatef(3.0f, -5.0f, 5.0f);

  //axis();
  glPushMatrix();
    kepala();
    sikilan();
    ekor();
  glPopMatrix();
  
  glPushMatrix();
    glTranslatef(5.0f,-5.0f,-12.25f);
    glRotatef(-90, 0.0f, 1.0f, 0.0f);
    axis();
    weapon_sts();
  glPopMatrix();

  glPushMatrix();
    glTranslatef(4.0f,-3.0f,-12.5f);
    glRotatef(-90, 0.0f, 1.0f, 0.0f);
    axis();
    sayap();
  glPopMatrix();

  glFlush();
  glutSwapBuffers();
}

void keyFunction(unsigned char key, int x, int y){
    switch(key) {
        case 68: // D
            rz += 5;
            break;
        case 65: // A
            rz -= 5;
            break;
    }
}

void keyControl(int k, int x, int y) {
    switch(k) {
        case GLUT_KEY_UP:
            ry += 5;
            break;
        case GLUT_KEY_DOWN:
            ry -= 5;
            break;
        case GLUT_KEY_LEFT:
            rx += 5;
            break;
        case GLUT_KEY_RIGHT:
            rx -= 5;
            break;
    }
}

int main(int argc, char **argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowSize(600, 600);
  glutInitWindowPosition(100, 100);
  glutCreateWindow("Helicopter");
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  initGL();
  glutTimerFunc(0, timer, 0);
  glutSpecialFunc(keyControl);
  glutKeyboardFunc(keyFunction);
  glutMainLoop();
  return 0;
}