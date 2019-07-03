#include "windows.h"
#include "GL/glut.h"
#include "math.h"
#define checkImageWidth 64
#define checkImageHeight 64


float angle=0;
float rx=-90.0f, ry=0.0f, rz=-180.0f;
float p=3.0f, l=10.0f, t=3.5f;
float sudut_ba=0;
float Cx = 0.0f, Cy = 2.5f, Cz = 0.0f;
float Lx = 0.0f, Ly = 2.5f, Lz = -20.0f;
float angle_depanBelakang = 0.0f;
float angle_depanBelakang2 = 0.0f;
float angle_samping = 0.0f;
float angle_samping2 = 0.0f;
float angle_vertikal = 0.0f;
float angle_vertikal2 = 0.0f;
float silinderX = 0.0f, silinderZ = 0.0f, silinderY = 0.0f;
bool ori = true,
  silinderZP = false, silinderZM = false,
  silinderYP = false, silinderYM = false,
  silinderXP = false, silinderXM = false,
kamera = false, kamera2 = false, kamera3 = false, kamera4 = false, kamera5 = false, kamera6 = false;

float toRadians(float angle) {
  return angle * M_PI / 180;
}

class Vector{
  public:
  float x, y, z;
  void set_values (float startX, float startY, float startZ) {
        x = startX;
        y = startY;
        z = startZ;
    }

    void vectorRotation(Vector refs, float angle) {
        Vector temp = refs;
        float magnitude = sqrt(pow(temp.x, 2) + pow(temp.y, 2) + pow(temp.z, 2));
        temp.x = temp.x / magnitude;
        temp.y = temp.y / magnitude;
        temp.z = temp.z / magnitude;
        float dot_product = (x * temp.x)+(y * temp.y)+(z * temp.z);
        float cross_product_x = (y * temp.z) - (temp.z * z);
        float cross_product_y = -((x * temp.z) - (z * temp.x));
        float cross_product_z = (x * temp.y) - (y * temp.x);
        float last_factor_rodrigues = 1.0f - cos(toRadians(fmod(angle, 360.0f)));
        x = (x * cos(toRadians(fmod(angle, 360.0f)))) + (cross_product_x * sin(toRadians(fmod(angle, 360.0f)))) + (dot_product * last_factor_rodrigues * x);
        y = (y * cos(toRadians(fmod(angle, 360.0f)))) + (cross_product_y * sin(toRadians(fmod(angle, 360.0f)))) + (dot_product * last_factor_rodrigues * y);
        z = (z * cos(toRadians(fmod(angle, 360.0f)))) + (cross_product_z * sin(toRadians(fmod(angle, 360.0f)))) + (dot_product * last_factor_rodrigues * z);
  }
};

Vector depanBelakang, samping, vertikal;

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


static void BigBox(){
  float amb[]={0.0f, 0.5f, 0.0f, 1.0f};
  float diff[]={0.0f, 0.5f, 0.0f, 1.0f};
  float spec[]={0.0f, 0.5f, 0.0f, 1.0f};
  float shine=0.0f;
  glMaterialfv(GL_FRONT,GL_AMBIENT,amb);
  glMaterialfv(GL_FRONT,GL_DIFFUSE,diff);
  glMaterialfv(GL_FRONT,GL_SPECULAR,spec);
  glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shine);
  glBegin(GL_QUADS);
  glVertex3f(-10000.0f, 0.0f, -10000.0f);
  glVertex3f(10000.0f, 0.0f, -10000.0f);
  glVertex3f(10000.0f, 0.0f, 10000.0f);
  glVertex3f(-10000.0f, 0.0f, 10000.0f);
  glEnd();
}

void vectorMovement(Vector toMove, float magnitude, float direction){
  float speedX = toMove.x * magnitude * direction;
  float speedY = toMove.y * magnitude * direction;
  float speedZ= toMove.z * magnitude * direction;
  Cx += speedX;
  Cy += speedY;
  Cz += speedZ;
  Lx += speedX;
  Ly += speedY;
  Lz += speedZ;
}

void cameraRotation(Vector refer, double angle){
  float M = sqrt(pow(refer.x, 2) + pow(refer.y, 2) + pow(refer.z, 2));
  float Up_x1 = refer.x / M;
  float Up_y1 = refer.y / M;
  float Up_z1 = refer.z / M;
  float VLx = Lx - Cx;
  float VLy = Ly - Cy;
  float VLz = Lz - Cz;
  float dot_product = (VLx * Up_x1) + (VLy * Up_y1) + (VLz * Up_z1);
  float cross_product_x = (Up_y1 * VLz) - (VLy * Up_z1);
  float cross_product_y = -((Up_x1 * VLz) - (Up_z1 * VLx));
  float cross_product_z = (Up_x1 * VLy) - (Up_y1 * VLx);
  float last_factor_rodrigues = 1.0f - cos(toRadians(angle));
  float Lx1 = (VLx * cos(toRadians(angle))) + (cross_product_x * sin(toRadians(angle))) + (dot_product * last_factor_rodrigues * VLx);
  float Ly1 = (VLy * cos(toRadians(angle))) + (cross_product_y * sin(toRadians(angle))) + (dot_product * last_factor_rodrigues * VLy);
  float Lz1 = (VLz * cos(toRadians(angle))) + (cross_product_z * sin(toRadians(angle))) + (dot_product * last_factor_rodrigues * VLz);
  Lx = Lx1+Cx;
  Ly = Ly1+Cy;
  Lz = Lz1+Cz;
}

void initGL() {
  depanBelakang.set_values(0.0f, 0.0f, -1.0f);
  samping.set_values(1.0f, 0.0f, 0.0f);
  vertikal.set_values(0.0f, 1.0f, 0.0f);

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

void tabung(float r, float t) {
    float BODY_LENGTH = t;
    float BODY_RADIUS = r;
    int SLICES = 120;
    int STACKS = 120;
    GLUquadric *q = gluNewQuadric();
    gluCylinder(q, BODY_RADIUS, BODY_RADIUS, BODY_LENGTH, SLICES, STACKS);
    gluDisk(q, 0.0f, BODY_RADIUS, SLICES, STACKS);
    glTranslatef(0.0f, 0.0f, BODY_LENGTH);
    gluDisk(q, 0.0f, BODY_RADIUS, SLICES, STACKS);
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

void baling_atas() {
  float pjg_sayap = 6.0f;
  glPushMatrix();
    glTranslatef(0.0f,0.0f,-1.0f);
    kerucut(0.7f, 0.5f, 0.5f);
    tabung(0.3f, 1.0f);
  glPopMatrix();

  glPushMatrix();
    glTranslatef(0.0f,0.0f,0.0f);
    glRotatef(sudut_ba, 0.0f, 0.0f, 1.0f);
    glBegin(GL_QUADS);
      glVertex3f(0.25f,0.25f,0.0f);
      glVertex3f(0.25f,-0.25f,0.0f);
      glVertex3f(pjg_sayap,-0.5f,0.0f);
      glVertex3f(pjg_sayap,0.5f,0.0f);
    glEnd();
    glBegin(GL_QUADS);
      glVertex3f(0.25f,0.25f,0.0f);
      glVertex3f(-0.25f,0.25f,0.0f);
      glVertex3f(-0.5f,pjg_sayap,0.0f);
      glVertex3f(0.5f,pjg_sayap,0.0f);
    glEnd();
    glBegin(GL_QUADS);
      glVertex3f(-0.25f,0.25f,0.0f);
      glVertex3f(-0.25f,-0.25f,0.0f);
      glVertex3f(-pjg_sayap,-0.5f,0.0f);
      glVertex3f(-pjg_sayap,0.5f,0.0f);
    glEnd();
    glBegin(GL_QUADS);
      glVertex3f(-0.25f,-0.25f,0.0f);
      glVertex3f(0.25f,-0.25f,0.0f);
      glVertex3f(0.5f,-pjg_sayap,0.0f);
      glVertex3f(-0.5f,-pjg_sayap,0.0f);
    glEnd();
  glPopMatrix();

  sudut_ba += 50;
  if(sudut_ba == 360) {
      sudut_ba = 0;
  }
}

void baling_belakang() {
glPushMatrix();
    glColor3d(0,0,1);
        glTranslated(5.8,1.8,-9.9);
        glRotatef(angle, 0, 0, 1);
        glBegin(GL_QUADS);
            glVertex3f(-0.2,1,0);
            glVertex3f(0.2,1,0);
            glVertex3f(0.2,-1,0);
            glVertex3f(-0.2,-1,0);
        glEnd();
    glPopMatrix();
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
    float ps=5.0f, ls=2.0f, ts=0.1f;

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
      glTranslated(0,0,-3);
      gluCylinder(q, 2, 2, 3.5, 5, 10);
      glTranslated(0,0,3.5);
      gluCylinder(q, 2, 0.5, 2, 5, 10);
      glTranslated(0,0,-5);
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
        glTranslated(5.4,1,-10);
        glBegin(GL_QUADS);
            glVertex3f(0,1,0);
            glVertex3f(1,1,0);
            glVertex3f(0,-1,0);
            glVertex3f(-1,-1,0);
        glEnd();
    glPopMatrix();
    glPushMatrix();
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

void helikopter() {
  glPushMatrix();
    kepala();
    sikilan();
    ekor();
    baling_belakang();
  glPopMatrix();

  // weapon station
  glPushMatrix();
    glTranslatef(5.0f,-5.0f,-12.25f);
    glRotatef(-90, 0.0f, 1.0f, 0.0f);
    axis();
    weapon_sts();
  glPopMatrix();

  // sayap
  glPushMatrix();
    glTranslatef(4.0f,-3.0f,-12.5f);
    glRotatef(-90, 0.0f, 1.0f, 0.0f);
    axis();
    sayap();
  glPopMatrix();

  // kinciran pring
  glPushMatrix();
    glTranslatef(-3.0f,1.5f,-9.0f);
    glRotatef(-90, 1.0f, 0.0f, 0.0f);
    axis();
    baling_atas();
  glPopMatrix();
}

void display() {
  const double t = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  //gluLookAt(4,4,4, 0,0,0, 0,0,1);
  gluLookAt(Cx, Cy, Cz,
            Lx, Ly, Lz,
            vertikal.x, vertikal.y, vertikal.z);

  /*axis(); // primary axis
  glTranslatef(-10.0f, -14.0f, -8.0f);
  glRotatef(rx, 1.0f, 0.0f, 0.0f);
  glRotatef(ry, 0.0f, 1.0f, 0.0f);
  glRotatef(rz, 0.0f, 0.0f, 1.0f);
  glTranslatef(3.0f, -5.0f, 5.0f);*/

  glRotatef(silinderZ, 1.0f, 0.0f, 0.0f);
  glRotatef(silinderY, 0.0f, 1.0f, 0.0f);
  glRotatef(silinderX, 0.0f, 0.0f, 1.0f);

  glPushMatrix();
    glTranslatef(0.0f, 5.0f, -15.0f);
    glRotatef(silinderZ, 1.0f, 0.0f, 0.0f);
    glRotatef(silinderY, 0.0f, 1.0f, 0.0f);
    glRotatef(silinderX, 0.0f, 0.0f, 1.0f);
    helikopter();
  glPopMatrix();
  glPushMatrix();
    BigBox();
  glPopMatrix();

  angle += 25;
  glFlush();
  glutSwapBuffers();
}

/*void keyFunction(unsigned char key, int x, int y){
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
*/

void keyControl(int k, int x, int y) {
  switch(k) {
    case GLUT_KEY_UP: //arrow up
      vectorMovement(vertikal, 2.0f, 1.0f);
      break;
    case GLUT_KEY_DOWN: //arrow down
      vectorMovement(vertikal, 2.0f, -1.0f);
      break;
  }
}

void keyFunction(unsigned char key, int x, int y){
  /* Agar fungsi ini bekerja, pastikan CapsLock menyala, kecuali tombol-tombol khusus seperti Spasi, dll */
  switch(key){
    case 68: // D
      vectorMovement(samping, 2.0f, 1.0f);
      break;
    case 65: // A
      vectorMovement(samping, 2.0f, -1.0f);
      break;
    case 87: // W
      vectorMovement(depanBelakang, 2.0f, 1.0f);
      break;
    case 83: // S
      vectorMovement(depanBelakang, 2.0f, -1.0f);
      break;
    case 90: // Z
      if(silinderZP) { silinderZP = false; }
      else { silinderZP = true; }
      break;
    case 88: // X
      if(silinderZM) { silinderZM = false; }
      else { silinderZM = true; }
      break;
    case 67: // C
      if(silinderYP) { silinderYP = false; }
      else { silinderYP = true; }
      break;
    case 86: // V
      if(silinderYM) { silinderYM = false; }
      else { silinderYM = true; }
      break;
    case 66: // B
      if(silinderXP) { silinderXP = false; }
      else { silinderXP = true; }
      break;
    case 78: // N
      if(silinderXM) { silinderXM = false; }
      else { silinderXM = true; }
      break;
    case 69: // E
      if (kamera){ kamera = false; }
      else { kamera = true; }
      break;
        case 82: // R
      if (kamera2){ kamera2 = false; }
      else { kamera2 = true; }
      break;
        case 84: // T
      if (kamera3){ kamera3 = false; }
      else { kamera3 = true; }
      break;
        case 89: // Y
      if (kamera4){ kamera4 = false; }
      else { kamera4 = true; }
      break;
        case 70: // F
      if (kamera5){ kamera5 = false; }
      else { kamera5 = true; }
      break;
        case 71: // G
      if (kamera6){ kamera6 = false; }
      else { kamera6 = true; }
      break;
    case 74: // J
      angle_vertikal += 5.0f;
      samping.vectorRotation(vertikal, angle_vertikal - angle_vertikal2);
      // memutar vector sumbu z terhadap x (target, patokan)
      depanBelakang.vectorRotation(vertikal, angle_vertikal - angle_vertikal2);
      cameraRotation(vertikal, angle_vertikal - angle_vertikal2); // lookat
      angle_vertikal2 = angle_vertikal;
      break;
    case 76: // L
      angle_vertikal -= 5.0f;
      samping.vectorRotation(vertikal, angle_vertikal - angle_vertikal2);
      depanBelakang.vectorRotation(vertikal, angle_vertikal - angle_vertikal2);
      cameraRotation(vertikal, angle_vertikal - angle_vertikal2);
      angle_vertikal2 = angle_vertikal;
      break;
    case 75: // K
      angle_samping -= 5.0f;
      // vertikal.vectorRotation(samping, angle_samping - angle_samping2);
      depanBelakang.vectorRotation(samping, angle_samping - angle_samping2);
      cameraRotation(samping, angle_samping - angle_samping2);
      angle_samping2 = angle_samping;
      break;
    case 73: // I
      angle_samping += 5.0f;
      // vertikal.vectorRotation(samping, angle_samping-angle_samping2);
      depanBelakang.vectorRotation(samping, angle_samping - angle_samping2);
      cameraRotation(samping, angle_samping - angle_samping2);
      angle_samping2 = angle_samping;
            break;
        case 85: // U
            angle_depanBelakang += 5.0f;
            samping.vectorRotation(depanBelakang, angle_depanBelakang - angle_depanBelakang2);
            vertikal.vectorRotation(depanBelakang, angle_depanBelakang - angle_depanBelakang2);
            //cameraRotation(vertikal, angle_samping-angle_samping2);
            angle_depanBelakang2 = angle_depanBelakang;
            break;
        case 79: // 0
            angle_depanBelakang -= 5.0f;
            samping.vectorRotation(depanBelakang, angle_depanBelakang - angle_depanBelakang2);
            vertikal.vectorRotation(depanBelakang, angle_depanBelakang - angle_depanBelakang2);
            //cameraRotation(vertikal, angle_samping-angle_samping2);
            angle_depanBelakang2 = angle_depanBelakang;
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
