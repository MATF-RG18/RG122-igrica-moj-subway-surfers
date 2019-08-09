#include <GL/glut.h>

#include "kuglaIKamera.h"

#define poluprecnikSfere 0.7

Sfera kugla = {0, poluprecnikSfere, 0, poluprecnikSfere};

void nacrtajLoptu()
{
    //preuzeo sa neta neku boju
    GLfloat ambijentMaterijala[] = {0.105882, 0.058824, 0.113725, 1.0};
    GLfloat difuznoMaterijala[] = {0.427451, 0.470588, 0.541176, 1.0};
    GLfloat spekularnoMaterijala[] = {0.333333, 0.333333, 0.521569, 1.0};
    GLfloat emisionoMaterijala[] = {0, 0, 0, 0};
    GLfloat sajnes = 9.84615;
    
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambijentMaterijala);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, difuznoMaterijala);
    glMaterialfv(GL_FRONT, GL_SPECULAR, spekularnoMaterijala);
    glMaterialf(GL_FRONT, GL_SHININESS, sajnes);
    glMaterialfv(GL_FRONT, GL_EMISSION, emisionoMaterijala);
    
    glPushMatrix();
        glTranslatef(kugla.x, kugla.y, kugla.z);
        glutSolidSphere(kugla.r, 40, 40);
    glPopMatrix();
}

void postaviKameru()
{
    //ove cetiri double vrednosti predstavljaju pocetnu poziciju kamere
    //i ono u sta ona gleda nisu nikakve pametne formule samo nastelovano
    //da lepo izgleda
    double pocetnoY = 4*kugla.r + 2;
    double pocetnoZ = 5*kugla.r + 3;
    
    double pocetnoGledanjeY =  3*kugla.r;
    double pocetnoGledanjeZ = -5*kugla.r;
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    //kamera ce se kretati po x i z osi sa kuglicom
    gluLookAt(kugla.x, pocetnoY, pocetnoZ + kugla.z, 
              kugla.x, pocetnoGledanjeY, pocetnoGledanjeZ + kugla.z, 
              0, 1, 0);
}
