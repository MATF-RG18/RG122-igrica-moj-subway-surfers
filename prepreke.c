#include <GL/glut.h>

#include "prepreke.h"

int brojPrepreka = 30;

void nacrtajPrepreke()
{
    //preuzeta neka boja sa neta
    GLfloat ambijentMaterijala[] = {0.1, 0.18725, 0.1745, 0.8};
    GLfloat difuznoMaterijala[] = {0.396, 0.74151, 0.69102, 0.8};
    GLfloat spekularnoMaterijala[] = {0.297254, 0.30829, 0.306678, 0.8};
    GLfloat emisionoMaterijala[] = {0.0,0.0,0.0,0.0};
    GLfloat sajnes = 12.8;
    
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambijentMaterijala);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, difuznoMaterijala);
    glMaterialfv(GL_FRONT, GL_SPECULAR, spekularnoMaterijala);
    glMaterialf(GL_FRONT, GL_SHININESS, sajnes);
    glMaterialfv(GL_FRONT, GL_EMISSION, emisionoMaterijala);
    
    int i;
    for(i=0; i<brojPrepreka; i++)
    {
        glPushMatrix();
            glTranslatef(0, 0.5, i*35);
            glutSolidCube(1);
        glPopMatrix();
    }
}
