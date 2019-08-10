#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>

#include "ogradjenPut.h"
#include "kuglaIKamera.h"
#include "prepreke.h"

static int pokrenutaAplikacija = 0;
static int uSkoku = 0;
static const int vremePozivaTajmera = 17;
static const double pi = 3.141592;
static double fi = 0;
static const double pocetnaBrzinaPadanjaPriSkoku = 0.07;
static double brzinaPadanjaPriSkoku = 0.07;
static double brzinaKuglice = 0.8;
static double pomerajKugliceUStranu = 0.5;
static double brzinaRotacijeKuglice = 60;

static void inicijalizacija(int argc, char **argv);
static void postaviSvetlo(void);
static void promenaVelicineProzora(int sirina, int visina);
static void iscrtajNaEkran(void);
static void tastatura(unsigned char pritisnutKarakter, int x, int y);
static void tastaturaSpecijalniTasteri(int pritisnutSpecijalniKarakter, int x, int y);
static void tajmer(int idTajmera);
static void obradiSkok(void);
static void kretanjePoOsiZ(void);
static void rotirajKuglicu(void);

int main(int argc, char **argv)
{
    inicijalizacija(argc, argv);

    glutMainLoop();

    return 0;
}

static void inicijalizacija(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

    glutInitWindowSize(800, 800);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);

    glutKeyboardFunc(tastatura);
    glutSpecialFunc(tastaturaSpecijalniTasteri);
    glutReshapeFunc(promenaVelicineProzora);
    glutDisplayFunc(iscrtajNaEkran);
    
    glClearColor((double)135/255, (double)206/255, (double)235/255, 0); //preuzeo sa neta boju neba
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
}

static void postaviSvetlo()
{
    GLfloat pozicijaSvetla[] = { 0, 1, -0.3, 0 };
    GLfloat ambijentalnoSvetlo[] = { 0.2, 0.2, 0.2, 1.0 };
    GLfloat difuznoSvetlo[] = { 0.9, 0.9, 0.9, 1 };
    GLfloat spekularnoSvetlo[] = { 1, 1, 1, 1 };
    
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, pozicijaSvetla);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambijentalnoSvetlo);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, difuznoSvetlo);
    glLightfv(GL_LIGHT0, GL_SPECULAR, spekularnoSvetlo);
}

static void promenaVelicineProzora(int sirina, int visina)
{
    glViewport(0, 0, sirina, visina);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, (double)sirina / visina, 1, duzinaPuta/2.5);
}

static void iscrtajNaEkran(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    postaviKameru();    
    postaviSvetlo();

    nacrtajOgradjenPut();
    nacrtajLoptu();
    nacrtajPrepreke();
    
    glutSwapBuffers();
}

static void tastatura(unsigned char pritisnutKarakter, int x, int y)
{
    switch(pritisnutKarakter) 
    {
        case 27:
            exit(0);
            break;
            
        case 'g':
        case 'G':
            if(!pokrenutaAplikacija)
            {
                pokrenutaAplikacija = 1;
                glutTimerFunc(vremePozivaTajmera, tajmer, 0);
            }
            break;
            
        case 'p':
        case 'P':
            pokrenutaAplikacija = 0;
            break;
    }
}

static void tastaturaSpecijalniTasteri(int pritisnutSpecijalniKarakter, int x, int y)
{
    switch(pritisnutSpecijalniKarakter)
    {
        case GLUT_KEY_LEFT:
            if(pokrenutaAplikacija)
            {
                kugla.x += pomerajKugliceUStranu;
            }
            break;
            
        case GLUT_KEY_RIGHT:
            if(pokrenutaAplikacija)
            {
                kugla.x += -pomerajKugliceUStranu;
            }
            break;
        case GLUT_KEY_UP:
            if(pokrenutaAplikacija && !uSkoku)
            {
                uSkoku = 1;
            }
            break;
            
        case GLUT_KEY_DOWN:
            //Omogucava kad smo u skoku da brze padnemo na zemlju
            if(pokrenutaAplikacija && uSkoku)
            {                
                brzinaPadanjaPriSkoku += pocetnaBrzinaPadanjaPriSkoku;
            }
            break;
    }
}

static void tajmer(int idTajmera)
{
    kretanjePoOsiZ();
    rotirajKuglicu();
    obradiSkok();
    
    printf("pomeraj: %lf\nfi: %lf\n", kugla.z, fi);
    
    if(pokrenutaAplikacija)
        glutTimerFunc(vremePozivaTajmera, tajmer, 0);
    
    glutPostRedisplay();
}

static void obradiSkok(void)
{
    if(uSkoku)
    {
        fi += brzinaPadanjaPriSkoku;
        if(fi >= pi)
        {
            fi = 0;
            uSkoku = 0;
            brzinaPadanjaPriSkoku = pocetnaBrzinaPadanjaPriSkoku; //opet postavljamo jer se mozda menjalo u GLUT_KEY_DOWN
        }
        kugla.y = kugla.r + 2.5*sin(fi);
    }
}

static void kretanjePoOsiZ(void)
{
    kugla.z += brzinaKuglice;
    
    if(kugla.z >= duzinaPuta/2)
    {
        kugla.z -= duzinaPuta/2;
        //TODO pomeriPrepreke();
        //TODO napraviNovePrepreke();
    }
}

static void rotirajKuglicu(void)
{
    if(rotacijaKugle >= 360)
        rotacijaKugle = brzinaRotacijeKuglice;
    else
        rotacijaKugle += brzinaRotacijeKuglice;
}
