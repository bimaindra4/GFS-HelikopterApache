// Compile: g++ filename.cpp -o filename -lglut -lGL -lGLU

#include "GL/glut.h"
#include "math.h"
#define checkImageWidth 64
#define checkImageHeight 64

GLUquadric *q = gluNewQuadric();

static GLubyte checkImage[checkImageHeight][checkImageWidth][4];
static GLuint texName;

void makeCheckImage(void)
{
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


void initGL()
{
  GLfloat sun_direction[] = { 0.0, 2.0, -1.0, 1.0 };
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

void timer(int value)
{
  glutPostRedisplay();
  glutTimerFunc(15, timer, 0);
}


void reshape(GLsizei width, GLsizei height)
{
  if (height == 0)
    height = 1;
  GLfloat aspect = (GLfloat)width / (GLfloat)height;
  glViewport(0, 0, width, height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45.0f, aspect, 0.1f, 100.0f);
}

void kepala()
{
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

void sikilan()
{
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

void ekor()
{
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

void display()
{
   const double t = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glTranslated(0,0,-10);

  kepala();
  sikilan();
  ekor();

  //kurang kamera dan baling-baling

  glFlush();
  glutSwapBuffers();
}

int main(int argc, char **argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowSize(600, 600);
  glutInitWindowPosition(100, 100);
  glutCreateWindow("Helicopter");
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  initGL();
  glutTimerFunc(0, timer, 0);
  glutMainLoop();
  return 0;
}
