#include "CSCIx229.h"

void shrine(double sx, double sy, double sz, double h) {
    glPushMatrix();
    glScaled(sx,sy,sz);
    glColor3f(1,1,1);
    float radius = 1.5;
    float height = h;
    //top and bottom of main column
    circleFace(0, radius);
    circleFace(height, radius);
    //cylinder of main columns
    drawCylinder(0, height, radius);
    circleFace(0.0, .5);
    glPopMatrix();

}
