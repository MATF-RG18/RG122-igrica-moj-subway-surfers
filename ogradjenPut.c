#include <GL/glut.h>

#include "ogradjenPut.h"

const double sirinaPuta = 22;
const double debljinaPuta = 1;
const double duzinaPuta = 300;
const double visinaZida = 3;
const double debljinaZida = 1;

static void nacrtajPut(void);
static void nacrtajZidove(void);

void nacrtajOgradjenPut(void)
{
    nacrtajPut();
    nacrtajZidove();
}

/*
 * vrh puta je na y = 0 
 * bitno zbog crtanja ostalih objekata na putu
 */
static void nacrtajPut(void)
{
    //sa neta uzeo boju zlata
    GLfloat ambijentMaterijala[] = {0.24725, 0.1995, 0.0745, 1.0 };
    GLfloat difuznoMaterijala[] = {0.75164, 0.60648, 0.22648, 1.0 };
    GLfloat spekularnoMaterijala[] = {0.628281, 0.555802, 0.366065, 1.0 };
    GLfloat emisionoMaterijala[] = {0, 0, 0, 0};
    GLfloat sajnes = 51.2;
    
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambijentMaterijala);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, difuznoMaterijala);
    glMaterialfv(GL_FRONT, GL_SPECULAR, spekularnoMaterijala);
    glMaterialf(GL_FRONT, GL_SHININESS, sajnes);
    glMaterialfv(GL_FRONT, GL_EMISSION, emisionoMaterijala);
    
    glPushMatrix();
        glTranslatef(0, -debljinaPuta/2, duzinaPuta/2);
        glScalef(sirinaPuta, debljinaPuta, duzinaPuta+20);//duzinaPuta+20 -> dodajemo 20 jer pri vracanju kamere i kugle u nazad da ne bi opet videli pocetak staze
        glutSolidCube(1);
    glPopMatrix();
}

static void nacrtajZidove(void)
{
    //preuzeo sa neta neku boju sto lici na zelenu
    GLfloat ambijentMaterijala[] = {0.135, 0.2225, 0.1575, 0.95};
    GLfloat difuznoMaterijala[] = {0.54, 0.89, 0.63, 0.95};
    GLfloat spekularnoMaterijala[] = {0.316228, 0.316228, 0.316228, 0.95};
    GLfloat emisionoMaterijala[] = {0, 0, 0, 0};
    GLfloat sajnes = 12.8;
    
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambijentMaterijala);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, difuznoMaterijala);
    glMaterialfv(GL_FRONT, GL_SPECULAR, spekularnoMaterijala);
    glMaterialf(GL_FRONT, GL_SHININESS, sajnes);
    glMaterialfv(GL_FRONT, GL_EMISSION, emisionoMaterijala);
    
    //desni zid
    glPushMatrix();
        glTranslatef(debljinaZida/2 + sirinaPuta/2, visinaZida/2, duzinaPuta/2);
        glScalef(debljinaZida, visinaZida, duzinaPuta+20);
        glutSolidCube(1);
    glPopMatrix();
    
    //levi zid
    glPushMatrix();
        glTranslatef(-(debljinaZida/2 + sirinaPuta/2), visinaZida/2, duzinaPuta/2);
        glScalef(debljinaZida, visinaZida, duzinaPuta+20);
        glutSolidCube(1);
    glPopMatrix();
}
