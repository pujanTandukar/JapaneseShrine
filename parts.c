#include "CSCIx229.h"
#include "texture.h"

void drawCylinder(double base, double height, double radius) {
        //column cylinder
        glPushMatrix();
        
        int inc = 30;
        int max = 360; 

        glEnable(GL_TEXTURE_2D);
        glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE, GL_MODULATE);
        glBindTexture(GL_TEXTURE_2D,texture[0]);

        glColor4f(0.69, 0.31, 0.42, 0.0);

        glBegin(GL_TRIANGLE_STRIP);
        for (int i=0; i <  max; i+= inc)
        {
                //bottom left
                glTexCoord2f((((double) i)/max), 0.0);
                glNormal3f(Cos(i)*radius,base,Sin(i)*radius);
                glVertex3f(Cos(i)*radius,base,Sin(i)*radius);

                //top left
                glTexCoord2f((((double) i)/max), 1.0);
                glNormal3f(Cos(i)*radius,base + height,Sin(i)*radius);
                glVertex3f(Cos(i)*radius,base + height,Sin(i)*radius);

                //bottom right
                glTexCoord2f((((double) i + (double) inc)/max), 0.0);
                glNormal3f(Cos(i+inc)*radius,base,Sin(i+inc)*radius);
                glVertex3f(Cos(i+inc)*radius,base,Sin(i+inc)*radius);

                //top right
                glTexCoord2f((((double) i + (double) inc)/max), 1.0);
                glNormal3f(Cos(i+inc)*radius,base + height,Sin(i+inc)*radius);
                glVertex3f(Cos(i+inc)*radius,base + height,Sin(i+inc)*radius);
        }

        glEnd();
        glPopMatrix();

        glDisable(GL_TEXTURE_2D);
}
void circleFace(double height, double radius) {
        glPushMatrix();

        int inc = 30;
        int max = 390;

        glEnable(GL_TEXTURE_2D);
        glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE, GL_MODULATE);
        glBindTexture(GL_TEXTURE_2D,texture[0]);
        glBegin(GL_TRIANGLE_FAN);
        glTexCoord2f(0.5, 0.5);
        glNormal3f(0,height,0);
        glVertex3f(0.0, height, 0.0);

        for (int i=0; i < max; i+= inc)
        {
                glTexCoord2f(0.5*Cos(i)+0.5,0.5*Sin(i)+0.5);
                glVertex3f(Cos(i)*radius,height,Sin(i)*radius);
        }
        glEnd();
        glPopMatrix();

        glDisable(GL_TEXTURE_2D);
}