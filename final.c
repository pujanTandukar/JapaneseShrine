#include "CSCIx229.h"
#include "texture.h"

//  Globals
int axes=0;       //  Display axes
int th=35;       // Azimuth of view angle
int ntex=0;    //  Texture
int ph=25;       // Elevation of view anglef
int mode=1;       //  Projection mode
int move=1;       //  Move light
int fov=65;       //  Field of view (for perspective)
int light=1;      //  Lighting
double asp=1;     //  Aspect ratio
double dim=5.0;   //  Size of world
char* text[] = {"Orthogonal","Perspective","First Person"};  // Dimension display text

// Light values
int one       =   1;  // Unit value
int distance  =   15;  // Light distance
int inc       =  100;  // Ball increment
int smooth    =   1;  // Smooth/Flat shading
int local     =   0;  // Local Viewer Model
int emission  =   0;  // Emission intensity (%)
int ambient   =  30;  // Ambient intensity (%)
int diffuse   = 100;  // Diffuse intensity (%)
int specular  =   0;  // Specular intensity (%)
int shininess =   0;  // Shininess (power of two)
float shiny   =   1;  // Shininess (value)
int zh        =  90;  // Light azimuth
float ylight  =   20;  // Elevation of light

// angle of rotation for the camera direction
float angle=0.0;
// actual vector representing the camera's direction
float lx=0.0f,lz=-1.0f;
// XZ position of the camera
float x1=0.0f,z=8.0f;
float fraction = 0.1f;

/*
 *  Set projection
 */
static void Project(int ab, double bc, double cd)
{
   //  Tell OpenGL we want to manipulate the projection matrix
   glMatrixMode(GL_PROJECTION);
   //  Undo previous transformations
   glLoadIdentity();
   //  Perspective transformation
   if (mode == 1)
      gluPerspective(fov,asp,dim/4,4*dim);
  //  First Person Perspective transformation
   else if (mode == 2)
      gluPerspective(fov,asp,dim/10,10*dim);
   //  Orthogonal projection
   else if (mode == 0)
      glOrtho(-asp*dim,+asp*dim, -dim,+dim, -dim,+dim);
   //  Switch to manipulating the model matrix
   glMatrixMode(GL_MODELVIEW);
   //  Undo previous transformations
   glLoadIdentity();
}
/*
 *  Draw vertex in polar coordinates with normal
 */
static void Vertex(double th,double ph)
{
   double xx = Sin(th)*Cos(ph);
   double yy = Cos(th)*Cos(ph);
   double zz = Sin(ph);
   //  For a sphere at the origin, the position
   //  and normal vectors are the same
   glNormal3d(xx,yy,zz);
   glVertex3d(xx,yy,zz);
}

//make pillars using front bottom left point in the pillar. 
static void makePillars(double a, double b, double c){
  
  double x = a+0.1;
  double y = b+1;
  double z = c-0.1;

  glEnable(GL_TEXTURE_2D);
  glTexEnvi(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_MODULATE);

  //front face
  if(ntex == 0) glBindTexture(GL_TEXTURE_2D,texture[0]);
  glBegin(GL_POLYGON);
  glNormal3f(0,0,1);
  glTexCoord2f(0,0); glVertex3f(a,b,c);
  glTexCoord2f(1,0); glVertex3f(x,b,c);
  glTexCoord2f(1,1); glVertex3f(x,y,c);
  glTexCoord2f(0,1); glVertex3f(a,y,c);
  glEnd();

  //backface
  glBegin(GL_POLYGON);
  glNormal3f(0,0,-1);
  glTexCoord2f(0,0); glVertex3f(a,b,z);
  glTexCoord2f(1,0); glVertex3f(x,b,z);
  glTexCoord2f(1,1); glVertex3f(x,y,z);
  glTexCoord2f(0,1); glVertex3f(a,y,z);
  glEnd();

  //left face
  glBegin(GL_POLYGON);
  glNormal3f(-1,0,0);
  glTexCoord2f(1,0); glVertex3f(a,b,c);
  glTexCoord2f(0,0); glVertex3f(a,b,z);
  glTexCoord2f(0,1); glVertex3f(a,y,z);
  glTexCoord2f(1,1); glVertex3f(a,y,c);
  glEnd();

  //right face
  glBegin(GL_POLYGON);
  glNormal3f(1,0,0);
  glTexCoord2f(1,0); glVertex3f(x,b,c);
  glTexCoord2f(0,0); glVertex3f(x,b,z);
  glTexCoord2f(0,1); glVertex3f(x,y,z);
  glTexCoord2f(1,1); glVertex3f(x,y,c);
  glEnd();

  glDisable(GL_TEXTURE_2D); 
}


//make wall decor
static void makeWallDecor(double a, double b, double c){
  
  double x = a+0.02;
  double y = b+1;
  double z = c-0.02;

  glEnable(GL_TEXTURE_2D);
  glTexEnvi(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_MODULATE);

  //front face
  if(ntex == 0) glBindTexture(GL_TEXTURE_2D,texture[0]);
  glBegin(GL_POLYGON);
  glNormal3f(0,0,1);
  glTexCoord2f(0,0); glVertex3f(a,b,c);
  glTexCoord2f(0,1); glVertex3f(x,b,c);
  glTexCoord2f(1,0); glVertex3f(x,y,c);
  glTexCoord2f(1,1); glVertex3f(a,y,c);
  glEnd();

  //backface
  glBegin(GL_POLYGON);
  glNormal3f(0,0,-1);
  glTexCoord2f(0,0); glVertex3f(a,b,z);
  glTexCoord2f(1,0); glVertex3f(x,b,z);
  glTexCoord2f(0,1); glVertex3f(x,y,z);
  glTexCoord2f(1,1); glVertex3f(a,y,z);
  glEnd();

  //left face
  glBegin(GL_POLYGON);
  glNormal3f(-1,0,0);
  glTexCoord2f(0,0); glVertex3f(a,b,c);
  glTexCoord2f(0,1); glVertex3f(a,b,z);
  glTexCoord2f(1,0); glVertex3f(a,y,z);
  glTexCoord2f(1,1); glVertex3f(a,y,c);
  glEnd();

  //right face
  glBegin(GL_POLYGON);
  glNormal3f(1,0,0);
  glTexCoord2f(0,0); glVertex3f(x,b,c);
  glTexCoord2f(0,1); glVertex3f(x,b,z);
  glTexCoord2f(1,0); glVertex3f(x,y,z);
  glTexCoord2f(1,1); glVertex3f(x,y,c);
  glEnd();

  glDisable(GL_TEXTURE_2D); 
}


/* 
 *  Draw sky box
 */
static void Sky(double D)
{
   glColor3f(1,1,1);
   glEnable(GL_TEXTURE_2D);

   //  Sides
   glBindTexture(GL_TEXTURE_2D,texture[4]);
   glBegin(GL_QUADS);
   glTexCoord2f(0.00,0); glVertex3f(-D,-D,-D);
   glTexCoord2f(0.25,0); glVertex3f(+D,-D,-D);
   glTexCoord2f(0.25,1); glVertex3f(+D,+D,-D);
   glTexCoord2f(0.00,1); glVertex3f(-D,+D,-D);

   glTexCoord2f(0.25,0); glVertex3f(+D,-D,-D);
   glTexCoord2f(0.50,0); glVertex3f(+D,-D,+D);
   glTexCoord2f(0.50,1); glVertex3f(+D,+D,+D);
   glTexCoord2f(0.25,1); glVertex3f(+D,+D,-D);

   glTexCoord2f(0.50,0); glVertex3f(+D,-D,+D);
   glTexCoord2f(0.75,0); glVertex3f(-D,-D,+D);
   glTexCoord2f(0.75,1); glVertex3f(-D,+D,+D);
   glTexCoord2f(0.50,1); glVertex3f(+D,+D,+D);

   glTexCoord2f(0.75,0); glVertex3f(-D,-D,+D);
   glTexCoord2f(1.00,0); glVertex3f(-D,-D,-D);
   glTexCoord2f(1.00,1); glVertex3f(-D,+D,-D);
   glTexCoord2f(0.75,1); glVertex3f(-D,+D,+D);
   glEnd();

   glDisable(GL_TEXTURE_2D);
}

static void makeCylinder(float base, float height, float radius, int a){

  glEnable(GL_LINE_SMOOTH);
  glEnable(GL_TEXTURE_2D);
  glTexEnvi(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_MODULATE);
  if(a == 0) glBindTexture(GL_TEXTURE_2D,texture[16]);
  if(a == 1) glBindTexture(GL_TEXTURE_2D,texture[0]);

  glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0,1,0);
    glTexCoord2f( 0.5, 0.5 );
    glVertex3f(0, height, 0); 
    for (float i = 0; i <= 2 * 3.1414; i += 0.1){
        glTexCoord2f( 0.5f * cos(i) + 0.5f, 0.5f * sin(i) + 0.5f );
        glVertex3f(radius * cos(i), height, radius * sin(i));
    }
    glTexCoord2f( 0.0, 0.0 );
    glVertex3f(radius, height, 0);
  glEnd();

  glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0,-1,0);
    glTexCoord2f( 0.5, 0.5 );
    glVertex3f(0, 0, 0);
    for (float i = 2 * 3.1415; i >= 0; i -= 0.1){
        glTexCoord2f( 0.5f * cos(i) + 0.5f, 0.5f * sin(i) + 0.5f );
        glVertex3f(radius * cos(i), 0, radius * sin(i));
    }
  glEnd();

  glBegin(GL_QUAD_STRIP);
    for (float i = 0; i <= 2 * 3.1415; i += 0.1)
    {
        const float tc = ( i / (float)( 2 * 3.1415 ) );
        glNormal3f(0,0,1);
        glTexCoord2f( tc, 0.0 );
        glVertex3f(radius * cos(i), 0, radius * sin(i));
        glTexCoord2f( tc, 1.0 );
        glVertex3f(radius * cos(i), height, radius * sin(i));
    }
    glTexCoord2f( 0.0, 0.0 );
    glVertex3f(radius, 0, 0);
    glTexCoord2f( 0.0, 1.0 );
    glVertex3f(radius, height, 0);
  glEnd();
  glDisable(GL_TEXTURE_2D);
}

double bezier_x(double x1, double x2, double x3, double x4, double t) {
  double x = (1 - t)*(1 - t)*(1 - t)*x1 + 3*t*(1 - t)*(1 - t)*x2 + 3*t*t*(1 - t)*x3 + t*t*t*x4;
  return x;
}

double bezier_y(double y1, double y2, double y3, double y4, double t) {
  double y = (1 - t)*(1 - t)*(1 - t)*y1 + 3*t*(1 - t)*(1 - t)*y2 + 3*t*t*(1 - t)*y3 + t*t*t*y4;
  return y;
}

void makePot(double f, double s, double x, double y, double z, double *curve) {
  glPushMatrix();
  glTranslated(x,y,z);
  glScaled(s, s, s);
  int i;
  int j;
  int inc = 30;
  double range = 1;
  double shift = -1.0;
  double angle = 0;
  double hMax = 60.0;

  glEnable(GL_TEXTURE_2D);
  glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE, GL_MODULATE);
  glBindTexture(GL_TEXTURE_2D,texture[15]);

  glColor3f(1,0.8,0.8);

  for(i = 0; i < 360; i+= inc) {
    double nx = bezier_x(curve[0]/f, curve[1]/f, curve[2]/f, curve[3]/f, ((double)i)/360.0);
    double ny = bezier_y(curve[4]/f, curve[5]/f, curve[6]/f, curve[7]/f, ((double)i)/360.0);
    double nx1 = bezier_x(curve[0]/f, curve[1]/f, curve[2]/f, curve[3]/f, ((double)i+inc)/360.0);
    double ny1 = bezier_y(curve[4]/f, curve[5]/f, curve[6]/f, curve[7]/f, ((double)i+inc)/360.0);

    for(j = 0; j < 360; j+= inc) {
      glBegin(GL_QUADS);
      angle = (double) j;
      
      glTexCoord2f(((double) j)/(360.0/range) + shift, (((double) ny)*(360.0/hMax))/(360.0/range) + shift);
      glNormal3f(nx*Cos(angle),ny,nx*Sin(angle));glVertex3f(nx*Cos(angle),ny,nx*Sin(angle));

      glTexCoord2f(((double) j)/(360.0/range) + shift, (((double) ny1)*(360.0/hMax))/(360.0/range) + shift);
      glNormal3f(nx1*Cos(angle),ny1,nx1*Sin((angle))); glVertex3f(nx1*Cos(angle),ny1,nx1*Sin((angle)));

      glTexCoord2f(((double) j + ((double) inc))/(360.0/range) + shift, (((double) ny1)*(360.0/hMax))/(360.0/range) + shift);
      glNormal3f(nx1*Cos((angle + (double) inc)),ny1,nx1*Sin((angle + (double) inc))); glVertex3f(nx1*Cos((angle + (double) inc)),ny1,nx1*Sin((angle + (double) inc)));

      glTexCoord2f(((double) j + ((double) inc))/(360.0/range) + shift, (((double) ny)*(360.0/hMax))/(360.0/range) + shift);
      glNormal3f(nx*Cos((angle + (double) inc)),ny,nx*Sin((angle + (double) inc))); glVertex3f(nx*Cos((angle + (double) inc)),ny,nx*Sin((angle + (double) inc)));

      glEnd();
     }
  }
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

/*
 *  Draw a ball at (x,y,z) radius (r)
 */
static void ball(double x,double y,double z,double r)
{
   int th,ph;
   float yellow[] = {1.0,1.0,0.0,1.0};
   float Emission[]  = {0.0,0.0,0.01*emission,1.0};
   //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glScaled(r,r,r);
   //  White ball
  //  glColor3f(1,0.85,0);
   glMaterialf(GL_FRONT,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT,GL_SPECULAR,yellow);
   glMaterialfv(GL_FRONT,GL_EMISSION,Emission);
   //  Bands of latitude
   for (ph=-90;ph<90;ph+=inc)
   {
      glBegin(GL_QUAD_STRIP);
      for (th=0;th<=360;th+=2*inc)
      {
         Vertex(th,ph);
         Vertex(th,ph+inc);
      }
      glEnd();
   }
   //  Undo transofrmations
   glPopMatrix();
}



static void makePolygons(float a, float b, float c, float height, float width, float x, float y, float z){
  glEnable(GL_LINE_SMOOTH);
  glEnable(GL_TEXTURE_2D);
  glTexEnvi(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_MODULATE);
  if(ntex == 0) glBindTexture(GL_TEXTURE_2D,texture[6]);
  glBegin(GL_POLYGON);
  glNormal3f(x,y,z);
  glTexCoord2f(0,0);
  glVertex3f(a,b,c);
  glTexCoord2f(1,0);
  glVertex3f(a+width,b,c);
  glTexCoord2f(1,1);
  glVertex3f(a+width,b+height,c);
  glTexCoord2f(0,1);
  glVertex3f(a,b+height,c);
  glEnd();
  glDisable(GL_TEXTURE_2D);
}



void fracTree(GLfloat x1, GLfloat y1,GLfloat x2, GLfloat y2, GLfloat angle,GLint n,GLint branchCount, GLfloat z1, GLfloat z2)  {
    glEnable(GL_LINE_SMOOTH);
    glPushMatrix();
    glLineWidth(5); 
    glBegin(GL_LINE_STRIP);
    glVertex3f(x1,y1,z1);
    glVertex3f(x2,y2,z2);
    glEnd();

    if(n<1)
    return;   

    int nn = n-1; 
    double factor = 2;    
    int x3 = (x2-x1)*factor+x1-x2; 
    int y3 = (y2-y1)*factor+y1-y2; 
    fracTree(x2,y2, x3 * cos(angle) + y3 * sin(angle) + x2, -x3 * sin(angle) + y3 * cos(angle) + y2, angle,nn,branchCount,z1,z2);
    fracTree(x2,y2, x3 * cos(-angle) + y3 * sin(-angle) + x2,  -x3 * sin(-angle) + y3 * cos(-angle) + y2, angle,nn,branchCount,z1,z2);

    glPopMatrix();
}

static void makeGate(){
  glPushMatrix();
    glTranslatef(-1,0,4);
    makeCylinder(.3,2.5,.1,1);
  glPopMatrix();

  glPushMatrix();
    glTranslatef(1,0,4);
    makeCylinder(.3,2.5,.1,1);
  glPopMatrix();

  glPushMatrix();
    glTranslatef(1.5,2,4);
    glRotated(90,0,0,1);
    makeCylinder(.3,3,.1,1);
  glPopMatrix();

  glPushMatrix();
    glRotatef(90,0,0,1);
    glScalef(2,4,2);
    glTranslatef(1.2,-0.5,2.05);
    makePillars(0,0,0);
  glPopMatrix();

  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D,texture[10]);
  glBegin(GL_POLYGON);
  glNormal3f(0,1,0);
  glTexCoord2f(0,0); glVertex3f(-0.7,0,5);
  glTexCoord2f(3,0); glVertex3f(0.8,0,5);
  glTexCoord2f(3,3); glVertex3f(0.6,0,1);
  glTexCoord2f(0,3); glVertex3f(-0.5,0,1);
  glEnd();
  glBindTexture(GL_TEXTURE_2D,texture[17]);
  glBegin(GL_POLYGON);
  glNormal3f(0,1,0);
  glTexCoord2f(0,0); glVertex3f(-10,-0.001,5);
  glTexCoord2f(7,0); glVertex3f(10,-0.001,5);
  glTexCoord2f(7,7); glVertex3f(10,-0.001,-5);
  glTexCoord2f(0,7); glVertex3f(-10,-0.001,-5);
  glEnd();
  glDisable(GL_TEXTURE_2D);

}



static void makeMainShrine(){
  makePolygons(-0.15,0.5,-1.9,1.2,.5,0,0,1);  

  double pot1[8] = {0.32, 2.11, -0.3, 0.6,-0.01, 0.55, 1.88, 2.32};

  glPushMatrix();
    glScalef(.04,.04,.04);
    double potInc = 0.001;
    for(int i = -2; i<1; i++){
      makePot(0.05 - potInc, 0.2, 50, 0.5, i*13, pot1);
      potInc+=0.008;
    }
  glPopMatrix();
  
  glEnable(GL_TEXTURE_2D);
  glTexEnvi(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_MODULATE);
  glBindTexture(GL_TEXTURE_2D,texture[12]);
  glBegin(GL_POLYGON);
  glNormal3f(0,1,0);
  glTexCoord2d(0,0); glVertex3f(-0.15, 0.47, -1.39);
  glTexCoord2d(1,0); glVertex3f(0.25, 0.47, -1.39);
  glTexCoord2d(1,1); glVertex3f(0.25, 0.47, -1.40);
  glTexCoord2d(0,1); glVertex3f(-0.15, 0.47, -1.40);
  glEnd();
  glBegin(GL_POLYGON);
  glNormal3f(0,-1,0);
  glTexCoord2d(0,0); glVertex3f(-0.15, 0.45, -1.39);
  glTexCoord2d(1,0); glVertex3f(0.25, 0.45, -1.39);
  glTexCoord2d(1,1); glVertex3f(0.25, 0.45, -1.40);
  glTexCoord2d(0,1); glVertex3f(-0.15, 0.45, -1.40);
  glEnd();
  glBegin(GL_POLYGON);
  glNormal3f(0,0,1);
  glTexCoord2d(0,0); glVertex3f(-0.15, 0.45, -1.39);
  glTexCoord2d(1,0); glVertex3f(0.25, 0.45, -1.39);
  glTexCoord2d(1,1); glVertex3f(0.25, 0.47, -1.39);
  glTexCoord2d(0,1); glVertex3f(-0.15, 0.47, -1.39);
  glEnd();
  glBegin(GL_POLYGON);
  glNormal3f(0,0,-1);
  glTexCoord2d(0,0); glVertex3f(-0.15, 0.45, -1.40);
  glTexCoord2d(1,0); glVertex3f(0.25, 0.45, -1.4);
  glTexCoord2d(1,1); glVertex3f(0.25, 0.47, -1.4);
  glTexCoord2d(0,1); glVertex3f(-0.15, 0.47, -1.4);
  glEnd();
  glBegin(GL_POLYGON);
  glNormal3f(0,0,1);
  glTexCoord2d(0,0); glVertex3f(.25, 0.45, -1.39);
  glTexCoord2d(1,0); glVertex3f(0.25, 0.47, -1.39);
  glTexCoord2d(1,1); glVertex3f(0.28, 0.46, -1.395);
  glEnd();
  glBegin(GL_POLYGON);
  glNormal3f(0,0,-1);
  glTexCoord2d(0,0); glVertex3f(.25, 0.45, -1.40);
  glTexCoord2d(1,0); glVertex3f(0.25, 0.47, -1.40);
  glTexCoord2d(1,1); glVertex3f(0.28, 0.46, -1.395);
  glEnd();
  glBegin(GL_POLYGON);
  glNormal3f(0,1,0);
  glTexCoord2d(0,0); glVertex3f(.25, 0.45, -1.40);
  glTexCoord2d(1,0); glVertex3f(0.25, 0.45, -1.39);
  glTexCoord2d(1,1); glVertex3f(0.28, 0.46, -1.395);
  glEnd();
  glBegin(GL_POLYGON);
  glNormal3f(0,-1,0);
  glTexCoord2d(0,0); glVertex3f(.25, 0.47, -1.40);
  glTexCoord2d(1,0); glVertex3f(0.25, 0.47, -1.39);
  glTexCoord2d(1,1); glVertex3f(0.28, 0.46, -1.395);
  glEnd();

  glBindTexture(GL_TEXTURE_2D,texture[13]);
  glBegin(GL_POLYGON);
  glNormal3f(0,1,0);
  glTexCoord2d(0,0); glVertex3f(-0.16, 0.47, -1.38);
  glTexCoord2d(1,0); glVertex3f(-0.06, 0.47, -1.38);
  glTexCoord2d(1,1); glVertex3f(-0.06, 0.47, -1.41);
  glTexCoord2d(0,1); glVertex3f(-0.16, 0.47, -1.41);
  glEnd();
  glBegin(GL_POLYGON);
  glNormal3f(0,-1,0);
  glTexCoord2d(0,0); glVertex3f(-0.16, 0.45, -1.38);
  glTexCoord2d(1,0); glVertex3f(-0.06, 0.45, -1.38);
  glTexCoord2d(1,1); glVertex3f(-0.06, 0.45, -1.41);
  glTexCoord2d(0,1); glVertex3f(-0.16, 0.45, -1.41);
  glEnd();
  glBegin(GL_POLYGON);
  glNormal3f(0,0,1);
  glTexCoord2d(0,0); glVertex3f(-0.16, 0.45, -1.38);
  glTexCoord2d(1,0); glVertex3f(-0.06, 0.45, -1.38);
  glTexCoord2d(1,1); glVertex3f(-0.06, 0.47, -1.38);
  glTexCoord2d(0,1); glVertex3f(-0.16, 0.47, -1.38);
  glEnd();
  glBegin(GL_POLYGON);
  glNormal3f(0,0,-1);
  glTexCoord2d(0,0); glVertex3f(-0.16, 0.45, -1.41);
  glTexCoord2d(1,0); glVertex3f(-0.06, 0.45, -1.41);
  glTexCoord2d(1,1); glVertex3f(-0.06, 0.47, -1.41);
  glTexCoord2d(0,1); glVertex3f(-0.16, 0.47, -1.41);
  glEnd();
  glBegin(GL_POLYGON);
  glNormal3f(-1,0,0);
  glTexCoord2d(0,0); glVertex3f(-0.16, 0.45, -1.41);
  glTexCoord2d(1,0); glVertex3f(-0.16, 0.45, -1.38);
  glTexCoord2d(1,1); glVertex3f(-0.16, 0.47, -1.38);
  glTexCoord2d(0,1); glVertex3f(-0.16, 0.47, -1.41);
  glEnd();


  glBindTexture(GL_TEXTURE_2D,texture[14]);
  glBegin(GL_POLYGON);
  glNormal3f(0,0,1);
  glTexCoord2d(0,0); glVertex3f(-0.05, 0.42, -1.375);
  glTexCoord2d(1,0); glVertex3f(-0.07, 0.42, -1.375);
  glTexCoord2d(1,1); glVertex3f(-0.07, 0.51, -1.375);
  glTexCoord2d(0,1); glVertex3f(-0.05, 0.51, -1.375);
  glEnd();
  glBegin(GL_POLYGON);
  glNormal3f(0,0,-1);
  glTexCoord2d(0,0); glVertex3f(-0.05, 0.42, -1.415);
  glTexCoord2d(1,0); glVertex3f(-0.07, 0.42, -1.415);
  glTexCoord2d(1,1); glVertex3f(-0.07, 0.51, -1.415);
  glTexCoord2d(0,1); glVertex3f(-0.05, 0.51, -1.415);
  glEnd();
  glBegin(GL_POLYGON);
  glNormal3f(0,1,0);
  glTexCoord2d(0,0); glVertex3f(-0.05, 0.51, -1.375);
  glTexCoord2d(1,0); glVertex3f(-0.07, 0.51, -1.375);
  glTexCoord2d(1,1); glVertex3f(-0.07, 0.51, -1.415);
  glTexCoord2d(0,1); glVertex3f(-0.05, 0.51, -1.415);
  glEnd();
  glBegin(GL_POLYGON);
  glNormal3f(0,-1,0);
  glTexCoord2d(0,0); glVertex3f(-0.05, 0.42, -1.375);
  glTexCoord2d(1,0); glVertex3f(-0.07, 0.42, -1.375);
  glTexCoord2d(1,1); glVertex3f(-0.07, 0.42, -1.415);
  glTexCoord2d(0,1); glVertex3f(-0.05, 0.42, -1.415);
  glEnd();
  glBegin(GL_POLYGON);
  glNormal3f(0,1,0);
  glTexCoord2d(0,0); glVertex3f(-0.05, 0.42, -1.415);
  glTexCoord2d(1,0); glVertex3f(-0.05, 0.42, -1.375);
  glTexCoord2d(1,1); glVertex3f(-0.05, 0.51, -1.375);
  glTexCoord2d(0,1); glVertex3f(-0.05, 0.51, -1.415);
  glEnd();
   glBegin(GL_POLYGON);
  glNormal3f(0,1,0);
  glTexCoord2d(0,0); glVertex3f(-0.07, 0.42, -1.415);
  glTexCoord2d(1,0); glVertex3f(-0.07, 0.42, -1.375);
  glTexCoord2d(1,1); glVertex3f(-0.07, 0.51, -1.375);
  glTexCoord2d(0,1); glVertex3f(-0.07, 0.51, -1.415);
  glEnd();

    glPushMatrix();
      glTranslatef(0,.3,-1.5);
      makeCylinder(0.1,0.3,0.01,1);
    glPopMatrix();

    glPushMatrix();
      glTranslatef(.19,.3,-1.5);
      makeCylinder(0.1,0.3,0.01,1);
    glPopMatrix();

    glPushMatrix();
      glScalef(0.2,0.2,0.2);
      glRotatef(-30,0,0,1);
      glTranslatef(-1, .1, -7.0);
      makePillars(0,0,0);
    glPopMatrix();
  
    glPushMatrix();
      glRotatef(30,0,0,1);
      glScalef(0.2,0.2,0.2);
      glTranslatef(1.8,-.4, -7.0);
      makePillars(0,0,0);
    glPopMatrix();

     glPushMatrix();
      glScalef(0.2,0.2,0.2);
      glRotatef(-30,0,0,1);
      glTranslatef(-1, .1, -8);
      makePillars(0,0,0);
    glPopMatrix();
  
    glPushMatrix();
      glRotatef(30,0,0,1);
      glScalef(0.2,0.2,0.2);
      glTranslatef(1.8,-.4, -8.0);
      makePillars(0,0,0);
    glPopMatrix();

  glEnable(GL_TEXTURE_2D);
  glTexEnvi(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_MODULATE);
  glBindTexture(GL_TEXTURE_2D,texture[11]);
  glEnable(GL_LINE_SMOOTH);
  glBegin(GL_POLYGON);
  glNormal3f(0,1,0);
  glTexCoord2d(0,0); glVertex3f(-0.2, 0.26, -1.25);
  glTexCoord2d(1,0); glVertex3f(0.4, 0.26, -1.25);
  glTexCoord2d(1,1); glVertex3f(0.4, 0.26, -1.75);
  glTexCoord2d(0,1); glVertex3f(-0.2, 0.26, -1.75);
  glEnd();
  
  glBegin(GL_POLYGON);
  glNormal3f(0,1,0);
  glTexCoord2d(0,0); glVertex3f(-0.2, 0.30, -1.25);
  glTexCoord2d(1,0); glVertex3f(0.4, 0.30, -1.25);
  glTexCoord2d(1,1); glVertex3f(0.4, 0.30, -1.75);
  glTexCoord2d(0,1); glVertex3f(-0.2, .30, -1.75);
  glEnd();
  
  glBegin(GL_POLYGON);
  glNormal3f(-1,0,0);
  glTexCoord2d(0,0); glVertex3f(-0.2, .26, -1.75);
  glTexCoord2d(1,0); glVertex3f(-0.2, .26, -1.25);
  glTexCoord2d(1,1); glVertex3f(-0.2, .30, -1.25);
  glTexCoord2d(0,1); glVertex3f(-0.2, .30, -1.75);
  glEnd();
  
  glBegin(GL_POLYGON);
  glNormal3f(1,0,0);
  glTexCoord2d(0,0); glVertex3f(0.4, .26, -1.75);
  glTexCoord2d(1,0); glVertex3f(0.4, .26, -1.25);
  glTexCoord2d(1,1); glVertex3f(0.4, .30, -1.25);
  glTexCoord2d(0,1); glVertex3f(0.4, .30, -1.75);
  glEnd();

  glBegin(GL_POLYGON);
  glNormal3f(0,0,1);
  glTexCoord2d(0,0); glVertex3f(-0.2, .26, -1.25);
  glTexCoord2d(1,0); glVertex3f(0.4, .26, -1.25);
  glTexCoord2d(1,1); glVertex3f(0.4, .30, -1.25);
  glTexCoord2d(0,1); glVertex3f(-0.2, .30, -1.25);
  glEnd();
  
  glBegin(GL_POLYGON);
  glNormal3f(0,0,-1);
  glTexCoord2d(0,0); glVertex3f(-0.2, .25, -1.75);
  glTexCoord2d(1,0); glVertex3f(0.4, .25, -1.75);
  glTexCoord2d(1,1); glVertex3f(0.4, 0.28, -1.75);
  glTexCoord2d(0,1); glVertex3f(-0.2 , 0.28, -1.75);
  glEnd();

  // sadasdsd

  glEnable(GL_TEXTURE_2D);
  glTexEnvi(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_MODULATE);
  glBindTexture(GL_TEXTURE_2D,texture[1]);
  glBegin(GL_POLYGON);
  glNormal3f(0,1,0);
  glTexCoord2d(0,0); glVertex3f(-0.4, 0.1, -1.25);
  glTexCoord2d(1,0); glVertex3f(0.6, 0.1, -1.25);
  glTexCoord2d(1,1); glVertex3f(0.6, 0.1, -1.75);
  glTexCoord2d(0,1); glVertex3f(-0.4, 0.1, -1.75);
  glEnd();  
  glBegin(GL_POLYGON);
  glNormal3f(0,1,0);
  glTexCoord2d(0,0); glVertex3f(-0.4, 0.001, -1.25);
  glTexCoord2d(1,0); glVertex3f(0.6, 0.001, -1.25);
  glTexCoord2d(1,1); glVertex3f(0.6, 0.001, -1.75);
  glTexCoord2d(0,1); glVertex3f(-0.4, 0.001, -1.75);
  glEnd();
  
  glBegin(GL_POLYGON);
  glNormal3f(-1,0,0);
  glTexCoord2d(0,0); glVertex3f(-0.4, 0, -1.75);
  glTexCoord2d(1,0); glVertex3f(-0.4, 0, -1.25);
  glTexCoord2d(1,1); glVertex3f(-0.4, 0.1, -1.25);
  glTexCoord2d(0,1); glVertex3f(-0.4, 0.1, -1.75);
  glEnd();
  
  glBegin(GL_POLYGON);
  glNormal3f(1,0,0);
  glTexCoord2d(0,0); glVertex3f(0.6, 0, -1.75);
  glTexCoord2d(1,0); glVertex3f(0.6, 0, -1.25);
  glTexCoord2d(1,1); glVertex3f(0.6, 0.1, -1.25);
  glTexCoord2d(0,1); glVertex3f(0.6, 0.1, -1.75);
  glEnd();

  glBegin(GL_POLYGON);
  glNormal3f(0,0,1);
  glTexCoord2d(0,0); glVertex3f(-0.4, 0, -1.25);
  glTexCoord2d(1,0); glVertex3f(0.6, 0, -1.25);
  glTexCoord2d(1,1); glVertex3f(0.6, 0.1, -1.25);
  glTexCoord2d(0,1); glVertex3f(-0.4, 0.1, -1.25);
  glEnd();
  
  glBegin(GL_POLYGON);
  glNormal3f(0,0,-1);
  glTexCoord2d(0,0); glVertex3f(-0.4, 0, -1.75);
  glTexCoord2d(1,0); glVertex3f(0.6, 0, -1.75);
  glTexCoord2d(1,1); glVertex3f(0.6, 0.1, -1.75);
  glTexCoord2d(0,1); glVertex3f(-0.4, 0.1, -1.75);
  glEnd();

  glEnable(GL_TEXTURE_2D);
  glTexEnvi(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_MODULATE);
  glBindTexture(GL_TEXTURE_2D,texture[7]);
  glBegin(GL_POLYGON);
  glNormal3f(0,1,0);
  glTexCoord2d(0,0); glVertex3f(-0.3, 0.11, -1.26);
  glTexCoord2d(1,0); glVertex3f(0.5, 0.11, -1.26);
  glTexCoord2d(1,1); glVertex3f(0.5, 0.11, -1.68);
  glTexCoord2d(0,1); glVertex3f(-0.3, 0.11, -1.68);
  glEnd();
  glDisable(GL_TEXTURE_2D);
}
// Making the temple
static void temple(double x,double y,double z, double a, double b, double c,double red, double green, double blue)
{
  //  Set specular color to white
  float white[] = {1,1,1,1};
  float black[] = {0,0,0,1};
  glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
  glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
  glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);
 
  // float pillarCord[] = {.5,0,1, -.5,0,1, .5,1,1, -.5,1,1, -2.5,0,1, 2.4,0,1, -2.5,0,-2, 2.4,0,-2, -2.5,1,1, 2.4,1,1,  -2.5,1,-2, 2.4,1,-2, -1.5,2,0, -1.5,2,-1, 1.5,2,0, 1.5,2,-1};
  // for(int i = 0; i<sizeof(pillarCord); i+=3){
  //   makePillars(pillarCord[i],pillarCord[i+1],pillarCord[i+2]);
  // }
  //pillars

  glPushMatrix();
  glScaled(0.6,0.6, 1);
  for(int i = 0; i<4; i+=1){
    fracTree(9+2*i,0,9+2*i,2,125,3,3,-1+i*1.1,-1+i*1.1);
    fracTree(-9-2*i,0,-9-2*i,2,1,3,3,-1+i*1,-1+i*1);
  }
  
  glPopMatrix();


  // glPushMatrix();
  // glRotated(90,0,1,0);
  // glScaled(0.6,0.6, 1);
  // glTranslatef(0,0,4);
  // tree(-8,0,-8,2,100,4,3);
  // tree(-17,0,-17,2,100,4,2);
  // glPopMatrix();

  makePillars(.5,0,1); makePillars(-.5,0,1); makePillars(.5,1,1); makePillars(-.5,1,1);   
  makePillars(-2.5,0,1);makePillars(-2.5,0,0);makePillars(-2.5,0,-1);makePillars(-2.5,1,0);
  makePillars(-2.5,1,-1);makePillars(2.4,0,1);makePillars(2.4,0,-2);makePillars(2.4,0,0);
  makePillars(2.4,0,-1);makePillars(2.4,1,0);makePillars(2.4,1,-1);makePillars(-2.5,0,-2);
  makePillars(-2.5,1,1);makePillars(2.4,1,1);makePillars(-2.5,1,-2);makePillars(2.4,1,-2);
  // roof(0,3,1,1);
  makePillars(-1.5,2,0);makePillars(-1.5,2,-1);makePillars(1.5,2,0);makePillars(1.5,2,-1);

  makeMainShrine();
  makeGate();
  
  float wallCord[] = {1,1,1, 1.5,1,1, 2,1,1, -1,1,1, -1.5,1,1, -2,1,1, -2.5,1,-1.70, -2.5,0,-1.70, -2.5,0,-1.40, -2.5,1,-1.40, 2.5,1,-1.70, 2.5,0,-1.70, 2.5,0,-1.40, 2.5,1,-1.40,  -2.5,1,.60, -2.5,0,.60, -2.5,0,.30, -2.5,1,.30, 2.5,1,.60, 2.5,0,.60, 2.5,0,.30, 2.5,1,.30};
  for(int i = 0; i<66; i+=3){
    makeWallDecor(wallCord[i],wallCord[i+1],wallCord[i+2]);
  }

  glPushMatrix();
  glRotatef(90,1,0,0);
    // for(double i = 1; i<2; i+=0.2){
      glTranslatef(-2.2,-.7,-0.4);
      makeCylinder(0.3,.3,.1,0);
      glTranslatef(-0.05,0,0.14);
      makeCylinder(0.3,.55,.1,0);
      glTranslatef(-0.05,0,0.14);
      makeCylinder(0.3,.75,.1,0);
      glTranslatef(0.2,0,-0.14);
      makeCylinder(0.3,.55,.1,0);
      // makeCylinder(0.3,.55,.1,0);
      glTranslatef(0.05,0,0.14);
      makeCylinder(0.3,.75,.1,0);
      // makeCylinder(0.3,.75,.1,0);
    // }
  glPopMatrix();

  // glRotatef(90,0,0,1);
  // makePillars(0,.5,0);
  // glRotatef(-90,0,0,1);
  
  //actual roof
  glEnable(GL_TEXTURE_2D);
  glTexEnvi(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_MODULATE);
  if(ntex == 0) glBindTexture(GL_TEXTURE_2D,texture[3]);
  glBegin(GL_POLYGON);
  glNormal3f(-1,0,0);
  glTexCoord2f(1,1);glVertex3f(-1.5,2.5,0);
  glTexCoord2f(0,1);glVertex3f(-1.5,2.5,-1);
  glTexCoord2f(0,0);glVertex3f(-2.5,2,-2);
  glTexCoord2f(1,0);glVertex3f(-2.5,2,1);
  glEnd();

  glBegin(GL_POLYGON);
  glNormal3f(-1,0,0);
  glTexCoord2f(1,0);glVertex3f(-1.45,3,0);
  glTexCoord2f(1,1);glVertex3f(-1.45,3,-1.05);
  glTexCoord2f(0,0);glVertex3f(-1,3.5,-.6);
  glEnd();

  glBegin(GL_POLYGON);
  glNormal3f(1,0,0);
  glTexCoord2f(1,1);glVertex3f(1.5,2.5,0);
  glTexCoord2f(0,1);glVertex3f(1.5,2.5,-1);
  glTexCoord2f(0,0);glVertex3f(2.5,2,-2);
  glTexCoord2f(1,0);glVertex3f(2.5,2,1);
  glEnd();
  glBegin(GL_POLYGON);
  glNormal3f(1,0,0);
  glTexCoord2f(1,0);glVertex3f(1.55,3,0);
  glTexCoord2f(0.5,0.5);glVertex3f(1.55,3,-1.05);
  glTexCoord2f(0,0);glVertex3f(1,3.5,-.6);
  glEnd();

  glBegin(GL_POLYGON);
  glNormal3f(0,0,1);
  glTexCoord2f(0,0);glVertex3f(-1.5,2.5,0);
  glTexCoord2f(0,1);glVertex3f(-2.5,2,1);
  glTexCoord2f(1,1);glVertex3f(2.5,2,1);
  glTexCoord2f(1,0);glVertex3f(1.5,2.5,0);
  glEnd();
  glBegin(GL_POLYGON);
  glNormal3f(0,0,1);
  glTexCoord2f(0,0);glVertex3f(-1.45,3,0);
  glTexCoord2f(0,1);glVertex3f(-1,3.5,-.6);
  glTexCoord2f(1,1);glVertex3f(1,3.5,-.6);
  glTexCoord2f(1,0);glVertex3f(1.55,3,0);
  glEnd();

  glBegin(GL_POLYGON);
  glNormal3f(0,0,-1);
  glTexCoord2f(0,0);glVertex3f(-1.5,2.5,-1);
  glTexCoord2f(0,1);glVertex3f(-2.5,2,-2);
  glTexCoord2f(1,1);glVertex3f(2.5,2,-2);
  glTexCoord2f(1,0);glVertex3f(1.5,2.5,-1);
  glEnd();
  glBegin(GL_POLYGON);
  glNormal3f(0,0,-1);
  glTexCoord2f(0,0);glVertex3f(-1.45,3,-1.05);
  glTexCoord2f(0,1);glVertex3f(-1,3.5,-.6);
  glTexCoord2f(1,1);glVertex3f(1,3.5,-.6);
  glTexCoord2f(1,0);glVertex3f(1.55,3,-1.05);
  glEnd();
  glDisable(GL_TEXTURE_2D);

  //floor
  glEnable(GL_TEXTURE_2D);
  glTexEnvi(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_MODULATE);
  if(ntex == 0) glBindTexture(GL_TEXTURE_2D,texture[2]);
  glBegin(GL_POLYGON);
  glNormal3f(0,1,0);
  glTexCoord2f(1,0);glVertex3f(-2.5,0,-2.1);
  glTexCoord2f(1,1);glVertex3f(-2.5,0,1);
  glTexCoord2f(0,1);glVertex3f(2.5,0,1);
  glTexCoord2f(0,0);glVertex3f(2.5,0,-2.1);
  glEnd();
  //topFloor
  glBegin(GL_POLYGON);
  glNormal3f(0,1,0);
  glTexCoord2f(1,0);glVertex3f(-2.45,2.0,0.95);
  glTexCoord2f(1,1);glVertex3f(-2.45,2.0,-1.95);
  glTexCoord2f(0,1);glVertex3f(2.45,2.0,-1.95);
  glTexCoord2f(0,0);glVertex3f(2.45,2.0,0.95);
  glEnd();

  //walls left side
  glEnable(GL_TEXTURE_2D);
  glTexEnvi(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_MODULATE);
  if(ntex == 0) glBindTexture(GL_TEXTURE_2D,texture[1]);
  glColor4f(1,1,1,.1);
  glBegin(GL_POLYGON);
  glNormal3f(-1,0,0);
  glTexCoord2f(1,0);glVertex3f(-2.48,0,1);
  glTexCoord2f(1,1);glVertex3f(-2.48,1,1);
  glTexCoord2f(0,1);glVertex3f(-2.48,1,-2);
  glTexCoord2f(0,0);glVertex3f(-2.48,0,-2);
  glEnd();
  glBegin(GL_POLYGON);
  glNormal3f(1,0,0);
  glTexCoord2f(0,0);glVertex3f(-2.42,0,1);
  glTexCoord2f(0,1);glVertex3f(-2.42,1,1);
  glTexCoord2f(1,0);glVertex3f(-2.42,1,-2);
  glTexCoord2f(1,1);glVertex3f(-2.42,0,-2);
  glEnd();
  glBegin(GL_POLYGON);
  glNormal3f(-1,0,0);
  glTexCoord2f(0,0);glVertex3f(-2.48,1,1);
  glTexCoord2f(0,1);glVertex3f(-2.48,2,1);
  glTexCoord2f(1,0);glVertex3f(-2.48,2,-2);
  glTexCoord2f(1,1);glVertex3f(-2.48,1,-2);
  glEnd();
  glBegin(GL_POLYGON);
  glNormal3f(1,0,0);
  glTexCoord2f(0,0);glVertex3f(-2.42,1,1);
  glTexCoord2f(0,1);glVertex3f(-2.42,2,1);
  glTexCoord2f(1,0);glVertex3f(-2.42,2,-2);
  glTexCoord2f(1,1);glVertex3f(-2.42,1,-2);
  glEnd();


  //walls right side
  glBegin(GL_POLYGON);
  glNormal3f(1,0,0);
  glTexCoord2f(0,0);glVertex3f(2.48,0,1);
  glTexCoord2f(0,1);glVertex3f(2.48,1,1);
  glTexCoord2f(1,0);glVertex3f(2.48,1,-2);
  glTexCoord2f(1,1);glVertex3f(2.48,0,-2);
  glEnd();
  glBegin(GL_POLYGON);
  glNormal3f(-1,0,0);
  glTexCoord2f(0,0);glVertex3f(2.42,0,1);
  glTexCoord2f(0,1);glVertex3f(2.42,1,1);
  glTexCoord2f(1,0);glVertex3f(2.42,1,-2);
  glTexCoord2f(1,1);glVertex3f(2.42,0,-2);
  glEnd();
  glBegin(GL_POLYGON);
  glNormal3f(1,0,0);
  glTexCoord2f(0,0);glVertex3f(2.48,1,1);
  glTexCoord2f(0,1);glVertex3f(2.48,2,1);
  glTexCoord2f(1,0);glVertex3f(2.48,2,-2);
  glTexCoord2f(1,1);glVertex3f(2.48,1,-2);
  glEnd();
  glBegin(GL_POLYGON);
  glNormal3f(-1,0,0);
  glTexCoord2f(0,0);glVertex3f(2.42,1,1);
  glTexCoord2f(0,1);glVertex3f(2.42,2,1);
  glTexCoord2f(1,0);glVertex3f(2.42,2,-2);
  glTexCoord2f(1,1);glVertex3f(2.42,1,-2);
  glEnd();

  

   //walls back side
  glBegin(GL_POLYGON);
  glNormal3f(0,0,1);
  glTexCoord2f(0,0);glVertex3f(-2.4,0,-2.04);
  glTexCoord2f(0,1);glVertex3f(-2.4,1,-2.04);
  glTexCoord2f(1,0);glVertex3f(2.4,1,-2.04);
  glTexCoord2f(1,1);glVertex3f(2.4,0,-2.04);
  glEnd();
  glBegin(GL_POLYGON);
  glNormal3f(0,0,-1);
  glTexCoord2f(0,0);glVertex3f(-2.4,0,-2.08);
  glTexCoord2f(0,1);glVertex3f(-2.4,1,-2.08);
  glTexCoord2f(1,0);glVertex3f(2.4,1,-2.08);
  glTexCoord2f(1,1);glVertex3f(2.4,0,-2.08);
  glEnd();
  glBegin(GL_POLYGON);
  glNormal3f(0,0,1);
  glTexCoord2f(0,0);glVertex3f(-2.4,1,-2.04);
  glTexCoord2f(0,1);glVertex3f(-2.4,2,-2.04);
  glTexCoord2f(1,0);glVertex3f(2.4,2,-2.04);
  glTexCoord2f(1,1);glVertex3f(2.4,1,-2.04);
  glEnd();
  glBegin(GL_POLYGON);
  glNormal3f(0,0,-1);
  glTexCoord2f(0,0);glVertex3f(-2.4,1,-2.08);
  glTexCoord2f(0,1);glVertex3f(-2.4,2,-2.08);
  glTexCoord2f(1,0);glVertex3f(2.4,2,-2.08);
  glTexCoord2f(1,1);glVertex3f(2.4,1,-2.08);
  glEnd();
  glBegin(GL_POLYGON);
  glNormal3f(0,0,1);
  glTexCoord2f(0,0);glVertex3f(-2.4,1,.96);
  glTexCoord2f(0,1);glVertex3f(-2.4,2,.96);
  glTexCoord2f(1,0);glVertex3f(2.4,2,.96);
  glTexCoord2f(1,1);glVertex3f(2.4,1,.96);
  glEnd();
  glBegin(GL_POLYGON);
  glNormal3f(0,0,-1);
  glTexCoord2f(0,0);glVertex3f(-2.4,1,.92);
  glTexCoord2f(0,1);glVertex3f(-2.4,2,.92);
  glTexCoord2f(1,0);glVertex3f(2.4,2,.92);
  glTexCoord2f(1,1);glVertex3f(2.4,1,.92);
  glEnd(); 
  //topmost
  if(ntex == 0) glBindTexture(GL_TEXTURE_2D,texture[9]);
  glBegin(GL_POLYGON);
  glNormal3f(0,0,-1);
  glTexCoord2f(0,0);glVertex3f(-1.5,2,-1.05);
  glTexCoord2f(0,1);glVertex3f(-1.5,3,-1.05);
  glTexCoord2f(1,1);glVertex3f(1.5,3,-1.05);
  glTexCoord2f(1,0);glVertex3f(1.5,2,-1.05);
  glEnd();
  glBegin(GL_POLYGON);
  glNormal3f(0,0,1);
  glTexCoord2f(0,0);glVertex3f(-1.5,2,-0.05);
  glTexCoord2f(0,1);glVertex3f(-1.5,3,-0.05);
  glTexCoord2f(1,1);glVertex3f(1.5,3,-0.05);
  glTexCoord2f(1,0);glVertex3f(1.5,2,-0.05);
  glEnd();
  //topmost
  glBegin(GL_POLYGON);
  glNormal3f(-1,0,0);
  glTexCoord2f(0,0);glVertex3f(-1.45,2,-1);
  glTexCoord2f(0,1);glVertex3f(-1.45,3,-1);
  glTexCoord2f(1,1);glVertex3f(-1.45,3,0);
  glTexCoord2f(1,0);glVertex3f(-1.45,2,0);
  glEnd();
  glBegin(GL_POLYGON);
  glNormal3f(1,0,0);
  glTexCoord2f(0,0);glVertex3f(1.55,2,-1);
  glTexCoord2f(0,1);glVertex3f(1.55,3,-1);
  glTexCoord2f(1,1);glVertex3f(1.55,3,0);
  glTexCoord2f(1,0);glVertex3f(1.55,2,0);
  glEnd();


  // //pillar Support
  glEnable(GL_TEXTURE_2D);
  glTexEnvi(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_MODULATE);
  if(ntex == 0) glBindTexture(GL_TEXTURE_2D,texture[0]);
  glBegin(GL_POLYGON);
  glNormal3f(0,1,0);
  glTexCoord2f(0,0);glVertex3f(-2.6,1,.85);
  glTexCoord2f(0,1);glVertex3f(-2.6,1,1.05);
  glTexCoord2f(1,0);glVertex3f(2.6,1,1.05);
  glTexCoord2f(1,1);glVertex3f(2.6,1,.85);
  glEnd();
  glBegin(GL_POLYGON);
  glNormal3f(0,-1,0);glVertex3f(-2.6,1.05,.85);
  glTexCoord2f(0,0);glVertex3f(-2.6,1.05,1.05);
  glTexCoord2f(0,1);glVertex3f(2.6,1.05,1.05);
  glTexCoord2f(1,0);glVertex3f(2.6,1.05,.85);
  glEnd();
  glBegin(GL_POLYGON);
  glNormal3f(0,0,1);glVertex3f(-2.6,1.0,1.05);
  glTexCoord2f(0,0);glVertex3f(-2.6,1.05,1.05);
  glTexCoord2f(0,1);glVertex3f(2.6,1.05,1.05);
  glTexCoord2f(1,0);glVertex3f(2.6,1.0,1.05);
  glEnd();
  glBegin(GL_POLYGON);
  glNormal3f(0,0,-1);
  glTexCoord2f(0,0);glVertex3f(-2.6,1.0,.85);
  glTexCoord2f(1,0);glVertex3f(-2.6,1.05,.85);
  glTexCoord2f(0,1);glVertex3f(2.6,1.05,.85);
  glTexCoord2f(1,1);glVertex3f(2.6,1.0,.85);
  glEnd();
  //backPillar Support
  glBegin(GL_POLYGON);
  glNormal3f(0,1,0);
  glTexCoord2f(0,0);glVertex3f(-2.6,1,-2.15);
  glTexCoord2f(1,0);glVertex3f(-2.6,1,-1.95);
  glTexCoord2f(0,1);glVertex3f(2.6,1,-1.95);
  glTexCoord2f(1,1);glVertex3f(2.6,1,-2.15);
  glEnd();
  glBegin(GL_POLYGON);
  glNormal3f(0,-1,0);
  glTexCoord2f(0,0);glVertex3f(-2.6,1.05,-2.15);
  glTexCoord2f(1,0);glVertex3f(-2.6,1.05,-1.95);
  glTexCoord2f(0,1);glVertex3f(2.6,1.05,-1.95);
  glTexCoord2f(1,1);glVertex3f(2.6,1.05,-2.15);
  glEnd();
  glBegin(GL_POLYGON);
  glNormal3f(0,0,1);
  glTexCoord2f(0,0);glVertex3f(-2.6,1.0,-1.95);
  glTexCoord2f(1,0);glVertex3f(-2.6,1.05,-1.95);
  glTexCoord2f(0,1);glVertex3f(2.6,1.05,-1.95);
  glTexCoord2f(1,1);glVertex3f(2.6,1.0,-1.95);
  glEnd();
  glBegin(GL_POLYGON);
  glNormal3f(0,0,-1);
  glTexCoord2f(0,0);glVertex3f(-2.6,1.0,-2.15);
  glTexCoord2f(1,0);glVertex3f(-2.6,1.05,-2.15);
  glTexCoord2f(0,1);glVertex3f(2.6,1.05,-2.15);
  glTexCoord2f(1,1);glVertex3f(2.6,1.0,-2.15);
  glEnd();


  //backPillar Support
  glBegin(GL_POLYGON);
  glNormal3f(0,1,0);
  glTexCoord2f(0,0);glVertex3f(-2.6,1,-2.15);
  glTexCoord2f(1,0);glVertex3f(-2.4,1,-2.15);
  glTexCoord2f(0,1);glVertex3f(-2.4,1,1.05);
  glTexCoord2f(1,1);glVertex3f(-2.6,1,1.05);
  glEnd();
  glBegin(GL_POLYGON);
  glNormal3f(0,-1,0);
  glTexCoord2f(0,0);glVertex3f(-2.6,1.05,-2.15);
  glTexCoord2f(1,0);glVertex3f(-2.4,1.05,-2.15);
  glTexCoord2f(0,1);glVertex3f(-2.4,1.05,1.05);
  glTexCoord2f(1,1);glVertex3f(-2.6,1.05,1.05);
  glEnd();
  glBegin(GL_POLYGON);
  glNormal3f(-1,0,0);
  glTexCoord2f(0,0);glVertex3f(-2.6,1.0,1.05);
  glTexCoord2f(0,1);glVertex3f(-2.6,1.05,1.05);
  glTexCoord2f(1,0);glVertex3f(-2.6,1.05,-2.15);
  glTexCoord2f(1,1);glVertex3f(-2.6,1.0,-2.15);
  glEnd();
  glBegin(GL_POLYGON);
  glNormal3f(1,0,0);
  glTexCoord2f(0,0);glVertex3f(-2.4,1.0,1.05);
  glTexCoord2f(1,0);glVertex3f(-2.4,1.05,1.05);
  glTexCoord2f(0,1);glVertex3f(-2.4,1.05,-2.15);
  glTexCoord2f(1,1);glVertex3f(-2.4,1.0,-2.15);
  glEnd();

   //backPillar Support
  glBegin(GL_POLYGON);
  glNormal3f(0,1,0);
  glTexCoord2f(0,0);glVertex3f(2.6,1,-2.15);
  glTexCoord2f(1,0);glVertex3f(2.4,1,-2.15);
  glTexCoord2f(0,1);glVertex3f(2.4,1,1.05);
  glTexCoord2f(1,1);glVertex3f(2.6,1,1.05);
  glEnd();
  glBegin(GL_POLYGON);
  glNormal3f(0,-1,0);
  glTexCoord2f(0,0);glVertex3f(2.6,1.05,-2.15);
  glTexCoord2f(1,0);glVertex3f(2.4,1.05,-2.15);
  glTexCoord2f(0,1);glVertex3f(2.4,1.05,1.05);
  glTexCoord2f(1,1);glVertex3f(2.6,1.05,1.05);
  glEnd();
  glBegin(GL_POLYGON);
  glNormal3f(1,0,0);
  glTexCoord2f(0,0);glVertex3f(2.6,1.0,1.05);
  glTexCoord2f(1,0);glVertex3f(2.6,1.05,1.05);
  glTexCoord2f(0,1);glVertex3f(2.6,1.05,-2.15);
  glTexCoord2f(1,1);glVertex3f(2.6,1.0,-2.15);
  glEnd();
  glBegin(GL_POLYGON);
  glNormal3f(-1,0,0);
  glTexCoord2f(0,0);glVertex3f(2.4,1.0,1.05);
  glTexCoord2f(1,0);glVertex3f(2.4,1.05,1.05);
  glTexCoord2f(0,1);glVertex3f(2.4,1.05,-2.15);
  glTexCoord2f(1,1);glVertex3f(2.4,1.0,-2.15);
  glEnd();

  //secondPillar supports for second floor

  //pillar Support
  glBegin(GL_POLYGON);
  glNormal3f(0,1,0);
  glTexCoord2f(0,0);glVertex3f(-2.6,2,.85);
  glTexCoord2f(1,0);glVertex3f(-2.6,2,1.05);
  glTexCoord2f(0,1);glVertex3f(2.6,2,1.05);
  glTexCoord2f(1,1);glVertex3f(2.6,2,.85);
  glEnd();
  glBegin(GL_POLYGON);
  glNormal3f(0,-1,0);
  glTexCoord2f(0,0);glVertex3f(-2.6,2.05,.85);
  glTexCoord2f(1,0);glVertex3f(-2.6,2.05,1.05);
  glTexCoord2f(0,1);glVertex3f(2.6,2.05,1.05);
  glTexCoord2f(1,1);glVertex3f(2.6,2.05,.85);
  glEnd();
  glBegin(GL_POLYGON);
  glNormal3f(0,0,1);
  glTexCoord2f(0,0);glVertex3f(-2.6,2.0,1.05);
  glTexCoord2f(1,0);glVertex3f(-2.6,2.05,1.05);
  glTexCoord2f(0,1);glVertex3f(2.6,2.05,1.05);
  glTexCoord2f(1,1);glVertex3f(2.6,2.0,1.05);
  glEnd();
  glBegin(GL_POLYGON);
  glNormal3f(0,0,-1);
  glTexCoord2f(0,0);glVertex3f(-2.6,2.0,.85);
  glTexCoord2f(1,0);glVertex3f(-2.6,2.05,.85);
  glTexCoord2f(0,1);glVertex3f(2.6,2.05,.85);
  glTexCoord2f(1,1);glVertex3f(2.6,2.0,.85);
  glEnd();
  //backPillar Support
  glBegin(GL_POLYGON);
  glNormal3f(0,1,0);
  glTexCoord2f(0,0);glVertex3f(-2.6,2,-2.15);
  glTexCoord2f(1,0);glVertex3f(-2.6,2,-1.95);
  glTexCoord2f(0,1);glVertex3f(2.6,2,-1.95);
  glTexCoord2f(1,1);glVertex3f(2.6,2,-2.15);
  glEnd();
  glBegin(GL_POLYGON);
  glNormal3f(0,-1,0);
  glTexCoord2f(0,0);glVertex3f(-2.6,2.05,-2.15);
  glTexCoord2f(1,0);glVertex3f(-2.6,2.05,-1.95);
  glTexCoord2f(0,1);glVertex3f(2.6,2.05,-1.95);
  glTexCoord2f(1,1);glVertex3f(2.6,2.05,-2.15);
  glEnd();
  glBegin(GL_POLYGON);
  glNormal3f(0,0,1);
  glTexCoord2f(0,0);glVertex3f(-2.6,2.0,-1.95);
  glTexCoord2f(1,0);glVertex3f(-2.6,2.05,-1.95);
  glTexCoord2f(0,1);glVertex3f(2.6,2.05,-1.95);
  glTexCoord2f(1,1);glVertex3f(2.6,2.0,-1.95);
  glEnd();
  glBegin(GL_POLYGON);
  glNormal3f(0,0,-1);
  glTexCoord2f(0,0);glVertex3f(-2.6,2.0,-2.15);
  glTexCoord2f(1,0);glVertex3f(-2.6,2.05,-2.15);
  glTexCoord2f(0,1);glVertex3f(2.6,2.05,-2.15);
  glTexCoord2f(1,1);glVertex3f(2.6,2.0,-2.15);
  glEnd();


    //backPillar Support
  glBegin(GL_POLYGON);
  glNormal3f(0,1,0);
  glTexCoord2f(0,0);glVertex3f(-2.6,2,-2.15);
  glTexCoord2f(1,0);glVertex3f(-2.4,2,-2.15);
  glTexCoord2f(0,1);glVertex3f(-2.4,2,1.05);
  glTexCoord2f(1,1);glVertex3f(-2.6,2,1.05);
  glEnd();
  glBegin(GL_POLYGON);
  glNormal3f(0,-1,0);
  glTexCoord2f(0,0);glVertex3f(-2.6,2.05,-2.15);
  glTexCoord2f(1,0);glVertex3f(-2.4,2.05,-2.15);
  glTexCoord2f(0,1);glVertex3f(-2.4,2.05,1.05);
  glTexCoord2f(1,1);glVertex3f(-2.6,2.05,1.05);
  glEnd();
  glBegin(GL_POLYGON);
  glNormal3f(-1,0,0);
  glTexCoord2f(0,0);glVertex3f(-2.6,2.0,1.05);
  glTexCoord2f(1,0);glVertex3f(-2.6,2.05,1.05);
  glTexCoord2f(0,1);glVertex3f(-2.6,2.05,-2.15);
  glTexCoord2f(1,1);glVertex3f(-2.6,2.0,-2.15);
  glEnd();
  glBegin(GL_POLYGON);
  glNormal3f(1,0,0);
  glTexCoord2f(0,0);glVertex3f(-2.4,2.0,1.05);
  glTexCoord2f(1,0);glVertex3f(-2.4,2.05,1.05);
  glTexCoord2f(0,1);glVertex3f(-2.4,2.05,-2.15);
  glTexCoord2f(1,1);glVertex3f(-2.4,2.0,-2.15);
  glEnd();

   //backPillar Support
  glBegin(GL_POLYGON);
  glNormal3f(0,1,0);
  glTexCoord2f(0,0);glVertex3f(2.6,2,-2.15);
  glTexCoord2f(1,0);glVertex3f(2.4,2,-2.15);
  glTexCoord2f(0,1);glVertex3f(2.4,2,1.05);
  glTexCoord2f(1,1);glVertex3f(2.6,2,1.05);
  glEnd();
  glBegin(GL_POLYGON);
  glNormal3f(0,-1,0);
  glTexCoord2f(0,0);glVertex3f(2.6,2.05,-2.15);
  glTexCoord2f(1,0);glVertex3f(2.4,2.05,-2.15);
  glTexCoord2f(0,1);glVertex3f(2.4,2.05,1.05);
  glTexCoord2f(1,1);glVertex3f(2.6,2.05,1.05);
  glEnd();
  glBegin(GL_POLYGON);
  glNormal3f(1,0,0);
  glTexCoord2f(0,0);glVertex3f(2.6,2.0,1.05);
  glTexCoord2f(1,0);glVertex3f(2.6,2.05,1.05);
  glTexCoord2f(0,1);glVertex3f(2.6,2.05,-2.15);
  glTexCoord2f(1,1);glVertex3f(2.6,2.0,-2.15);
  glEnd();
  glBegin(GL_POLYGON);
  glNormal3f(-1,0,0);
  glTexCoord2f(0,0);glVertex3f(2.4,2.0,1.05);
  glTexCoord2f(1,0);glVertex3f(2.4,2.05,1.05);
  glTexCoord2f(0,1);glVertex3f(2.4,2.05,-2.15);
  glTexCoord2f(1,1);glVertex3f(2.4,2.0,-2.15);
  glEnd();
  
   //  Save transformation
   glPushMatrix();
   glTranslated(x,y,z);
   glScaled(a,b,c);

   //  Undo transformations
   glPopMatrix();
   glDisable(GL_TEXTURE_2D);  
}



/*
 *  Display the scene
 */
void display()
{
  //  Erase the window and the depth buffer
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  glClearColor(.19, .27, .5,.5);
  //  Enable Z-buffering in OpenGL
  glEnable(GL_DEPTH_TEST);
  //  Undo previous transformations
  glLoadIdentity();
  Sky(3*dim);
  //  Perspective - set eye position
  if (mode == 1)
  {
     double Ex = -2*dim*Sin(th)*Cos(ph);
     double Ey = +2*dim*Sin(ph);
     double Ez = +2*dim*Cos(th)*Cos(ph);
     gluLookAt(Ex,Ey,Ez,0,0,0,0,Cos(ph),0);
  }

  else if (mode == 2)
  {
    gluLookAt(x1,0.5,z,x1+lx,0.5,z+lz,0,10,0);
    //  gluLookAt(0,0,zval,xval,0,0,0,Cos(ph),0);
  }

  //  Orthogonal - set world orientation
  else if (mode == 0)
  {
     glRotatef(ph,1,0,0);
     glRotatef(th,0,1,0);
  }

   //  Display parameters
   glColor3d(.1,.1,.1);
   glWindowPos2i(5,5);
   Print("Dim=%.1f FOV=%d Projection=%s",dim,fov,text[mode]);

  //    Flat or smooth shading
   glShadeModel(smooth ? GL_SMOOTH : GL_FLAT);

   //  Light switch
   if (light)
   {
        //  Translate intensity to color vectors
        float Ambient[]   = {0.01*ambient ,0.01*ambient ,0.01*ambient ,1.0};
        float Diffuse[]   = {0.01*diffuse ,0.01*diffuse ,0.01*diffuse ,1.0};
        float Specular[]  = {0.01*specular,0.01*specular,0.01*specular,1.0};
        //  Light position
        float Position[]  = {distance*Cos(zh),ylight,distance*Sin(zh),1.0};
        //  Draw light position as ball (still no lighting here)
        glColor3f(1,1,1);
        ball(Position[0],Position[1],Position[2] , 0.1);
        //  OpenGL should normalize normal vectors
        glEnable(GL_NORMALIZE);
        //  Enable lighting
        glEnable(GL_LIGHTING);
        //  Location of viewer for specular calculations
        glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,local);
        //  glColor sets ambient and diffuse color materials
        glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
        glEnable(GL_COLOR_MATERIAL);
        //  Enable light 0
        glEnable(GL_LIGHT0);
        //  Set ambient, diffuse, specular components and position of light 0
        glLightfv(GL_LIGHT0,GL_AMBIENT ,Ambient);
        glLightfv(GL_LIGHT0,GL_DIFFUSE ,Diffuse);
        glLightfv(GL_LIGHT0,GL_SPECULAR,Specular);
        glLightfv(GL_LIGHT0,GL_POSITION,Position);
   }
   else
     glDisable(GL_LIGHTING);
   temple(0,0,0,1,1,1,.9,.2,.2);
   glPushMatrix();
   glTranslated(0,2,0);
  //  shrine(1,1,1,1);
   glPopMatrix();
   //  Flush and swap
   glFlush();
   glutSwapBuffers();
}


/*
 *  GLUT calls this routine when the window is resized
 */
void idle()
{
   //  Elapsed time in seconds
   double t = glutGet(GLUT_ELAPSED_TIME)/1000.0;
   zh = fmod(90*t,360.0);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}


/*
 *  GLUT calls this routine when a key is pressed
 */
void key(unsigned char ch,int x,int y)
{
   //  Exit on ESC
   if (ch == 27)
      exit(0);
   //  Reset view angle
   else if (ch == '0')
      th = ph = 0;
   //  Switch display mode
   else if (ch == 'm' || ch == 'M')
   {
      mode += 1;
      if(mode>2){
        mode = 1;
      }
   }
   //  Change field of view angle
   else if (ch == '1' && ch>1)
      fov--;
   else if (ch == '2' && ch<179)
      fov++;
  //  PageUp key - increase dim
   else if (ch == '3')
     dim += 0.1;
  //  PageDown key - decrease dim
  else if (ch == '4' && dim>1)
     dim -= 0.1;

  else if(ch == 'd' || ch == 'D'){
    angle += 3.0f;
    lx = Sin(angle);
    lz = -Cos(angle);
  }
  else if(ch == 'a' || ch == 'A'){
    if(mode == 2){
      angle -= 3.0f;
			lx = Sin(angle);
			lz = -Cos(angle);
    }
  }
  else if(ch == 'w' || ch == 'W'){
    x1 += lx * 0.1;
    z += lz * 0.1;
  }
  else if(ch == 's' || ch == 'S'){
    x1 -= lx * 0.1;
    z -= lz * 0.1;
  }
  else if (ch == 'l' || ch == 'L')
    light = 1-light;


 //  Light elevation
  else if (ch=='[')
    ylight -= 0.1;
  else if (ch==']')
     ylight += 0.1;
  //  Ambient level
  else if (ch=='j' && ambient>0)
     ambient -= 5;
  else if (ch=='J' && ambient<100)
     ambient += 5;
  //  Diffuse level
  else if (ch=='k' && diffuse>0)
     diffuse -= 5;
  else if (ch=='K' && diffuse<100)
     diffuse += 5;
  //  Specular level
  else if (ch=='f' && specular>0)
     specular -= 5;
  else if (ch=='F' && specular<100)
     specular += 5;
  //  Emission level
  else if (ch=='h' && emission>0)
     emission -= 5;
  else if (ch=='H' && emission<100)
     emission += 5;
  //  Shininess level
  else if (ch=='g' && shininess>-1)
     shininess -= 1;
  else if (ch=='G' && shininess<7)
     shininess += 1;
  //  Toggle light movement
  else if (ch == 'n' || ch == 'N')
    move = 1-move;
 //  Move light
  else if (ch == '<')
    zh += 1;
  else if (ch == '>')
    zh -= 1;

    //  Translate shininess power to value (-1 => 0)
    shiny = shininess<0 ? 0 : pow(2.0,shininess);
    //  Reproject
    Project(mode?fov:0,asp,dim);
    //  Animate if requested
    glutIdleFunc(move?idle:NULL);
    //  Tell GLUT it is necessary to redisplay the scene
    glutPostRedisplay();
}

/*
 *  GLUT calls this routine when an arrow key is pressed
 */
void special(int key,int x,int y)
{
   //  Right arrow key - increase angle by 5 degrees
   if (key == GLUT_KEY_RIGHT)
      th += 5;
   //  Left arrow key - decrease angle by 5 degrees
   else if (key == GLUT_KEY_LEFT)
      th -= 5;
   //  Up arrow key - increase elevation by 5 degrees
   else if (key == GLUT_KEY_UP)
      ph += 5;
   //  Down arrow key - decrease elevation by 5 degrees
   else if (key == GLUT_KEY_DOWN)
      ph -= 5;
   //  PageUp key - increase dim
   else if (key == GLUT_KEY_PAGE_UP)
      dim += 0.1;
   //  PageDown key - decrease dim
   else if (key == GLUT_KEY_PAGE_DOWN && dim>1)
      dim -= 0.1;
  else if (key == GLUT_KEY_F3)
     distance = (distance==1) ? 3 : 2;
   //  Keep angles to +/-360 degrees
   th %= 360;
   ph %= 360;
   //  Update projection
   Project(fov,asp,dim);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}
/*
 *  GLUT calls this routine when the window is resized
 */
void reshape(int width,int height)
{
   //  Ratio of the width to the height of the window
   double w2h = (height>0) ? (double)width/height : 1;
   //  Set the viewport to the entire window
   glViewport(0,0, width,height);
   //  Tell OpenGL we want to manipulate the projection matrix
   glMatrixMode(GL_PROJECTION);
   //  Undo previous transformations
   glLoadIdentity();
   //  Orthogonal projection box adjusted for the
   //  aspect ratio of the window
   glOrtho(-dim*w2h,+dim*w2h, -dim,+dim, -dim,+dim);
   //  Switch to manipulating the model matrix
  //  glMatrixMode(GL_MODELVIEW);
   //  Undo previous transformations
   glLoadIdentity();

   //  Ratio of the width to the height of the window
   asp = (height>0) ? (double)width/height : 1;
   //  Set the viewport to the entire window
   glViewport(0,0, width,height);
   //  Set projection
   Project(fov,asp,dim);

}
/*
 *  Start up GLUT and tell it what to do
 */
int main(int argc,char* argv[])
{
  //  Initialize GLUT and process user parameters
   glutInit(&argc,argv);
   //  Request double buffered, true color window
   glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
   //  Request 1000 x 500 pixel window
   glutInitWindowSize(1000,800);
   //  Create the window
   glutCreateWindow("Final Project: Pujan Tandukar");
   //  Tell GLUT to call "display" when the scene should be drawn
   glutDisplayFunc(display);
  //  Tell GLUT to call "reshape" when the window is resized
   glutReshapeFunc(reshape);
   //  Tell GLUT to call "special" when an arrow key is pressed
   glutSpecialFunc(special);
   //  Tell GLUT to call "key" when a key is pressed
   glutKeyboardFunc(key);
   glutIdleFunc(idle);
   // Loading Textures
   //  texture[0] = LoadTexBMP("wood.bmp");
   texture[0] = LoadTexBMP("oldwood.bmp");
   texture[1] = LoadTexBMP("white.bmp");
   texture[2] = LoadTexBMP("floor.bmp");
   texture[3] = LoadTexBMP("roof.bmp");
   texture[4] = LoadTexBMP("sky1.bmp");
   texture[5] = LoadTexBMP("shrineTex.bmp");
   texture[6] = LoadTexBMP("japan.bmp");
   texture[7] = LoadTexBMP("rug.bmp");
   texture[8] = LoadTexBMP("whiteBrick.bmp");
   texture[9] = LoadTexBMP("jpbrick.bmp");
   texture[10] = LoadTexBMP("floortex.bmp");
   texture[11] = LoadTexBMP("lightwood.bmp");
   texture[12] = LoadTexBMP("steel.bmp");
   texture[13] = LoadTexBMP("hilt.bmp");
   texture[14] = LoadTexBMP("gold.bmp");
   texture[15] = LoadTexBMP("pot.bmp");
   texture[16] = LoadTexBMP("woodbark.bmp");
   texture[17] = LoadTexBMP("grass.bmp");
   texture[18] = LoadTexBMP("con.bmp");
   ErrCheck("init");
   //  Pass control to GLUT so it can interact with the user
   glutMainLoop();
   //  Return code
   return 0;
}