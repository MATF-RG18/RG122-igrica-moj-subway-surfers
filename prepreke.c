#include <GL/glut.h>
#include <stdlib.h>

#include "prepreke.h"
#include "ogradjenPut.h"

int brVrstaPrepreka = 9;
Prepreka prvaPolovina[10];
Prepreka drugaPolovina[10];
int brPreprekaNaPrvojPolovini = 0;
int brPreprekaNaDrugojPolovini = 0;

static const int minRazmakIzmedjuPrepreka = 60;
static const int maxRazmakIzmedjuPrepreka = 80;
static int zPozicijaPravljenjaSledecePrepreke;
static const double maxBrzinaKretanja = 0.2;

static Prepreka napraviSledecuPrepreku(int zPozicijaPrepreke);

static Prepreka napraviObicnuPrepreku(void);
static Prepreka napraviPreskakajucuPrepreku(void);
static Prepreka napraviObicnuPlusPreskakajucuPrepreku(void);
static Prepreka napraviDvostrukuPrepreku(void);
static Prepreka napraviDvostrukuPlusPreskakajucuPrepreku(void);
static Prepreka napraviPomerajucuObicnuPrepreku(void);
static Prepreka napraviPomerajucuObicnuPlusPreskakajucuPrepreku(void);
static Prepreka napraviPomerajucuDvostrukuPrepreku(void);
static Prepreka napraviPomerajucuDvostrukuPlusPreskakajucuPrepreku(void);

static DeoPrepreke napraviDeoPreprekeObicna(double x, double y, double minVisina);
static DeoPrepreke napraviDeoPreprekePreskakajuca();

static void nacrtajPrepreku(Prepreka p);
static void nacrtajDeoPrepreke(DeoPrepreke dp);

static int randomCelobrojnaVrednost(int n, int m) //[n, m]
{
    return n + rand()%(m-n+1);
}
static double randomRealnaVrednost(double n, double m) //[n, m]
{
    return n+(m-n)*((double) rand() / RAND_MAX);
}
static void kretanjePrepreke(Prepreka *p, int i);
static int sudarDelaPreprekeSaZidom(DeoPrepreke pa);

void inicijalizujPrepreke(void)
{
    brVrstaPrepreka = 9;
    zPozicijaPravljenjaSledecePrepreke = duzinaPuta/2 + minRazmakIzmedjuPrepreka;
}

void nacrtajPrepreke(void)
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
    for(i=0; i<brPreprekaNaPrvojPolovini; i++)
        nacrtajPrepreku(prvaPolovina[i]);
    
    for(i=0; i<brPreprekaNaDrugojPolovini; i++)
        nacrtajPrepreku(drugaPolovina[i]);
}

void pomeriPrepreke(void)
{
    int i;
    for(i=0; i<brPreprekaNaDrugojPolovini; i++)
    {
        //pomerimo sve bez obzira da li se prepreka sastoji iz 3 dela ili manje delova
        drugaPolovina[i].a.z -= duzinaPuta/2;
        drugaPolovina[i].b.z -= duzinaPuta/2;
        drugaPolovina[i].c.z -= duzinaPuta/2;
        prvaPolovina[i] = drugaPolovina[i];
    }
    
    brPreprekaNaPrvojPolovini = brPreprekaNaDrugojPolovini;
}

/*Prepreke se uvek prave na drugoj polovini duzinaPuta
 * A na prvoj se dobijaju pomeranjem*/
void napraviNovePrepreke(void)
{
    int i;
    
    for(i=0; zPozicijaPravljenjaSledecePrepreke <= duzinaPuta; i++)
    {
        drugaPolovina[i] = napraviSledecuPrepreku(zPozicijaPravljenjaSledecePrepreke);
        zPozicijaPravljenjaSledecePrepreke += randomCelobrojnaVrednost(minRazmakIzmedjuPrepreka, maxRazmakIzmedjuPrepreka);
    }
    
    brPreprekaNaDrugojPolovini = i;
    zPozicijaPravljenjaSledecePrepreke -= duzinaPuta/2;
}

static Prepreka napraviSledecuPrepreku(int zPozicijaPrepreke)
{
    enum prepreke vrstaSledecePrepreke = randomCelobrojnaVrednost(0, brVrstaPrepreka-1);
    Prepreka sledecaPrereka;
    
    if(vrstaSledecePrepreke == Obicna)
        sledecaPrereka = napraviObicnuPrepreku();
    if(vrstaSledecePrepreke == Preskakajuca)
        sledecaPrereka = napraviPreskakajucuPrepreku();
    else if(vrstaSledecePrepreke == ObicnaPlusPreskakajuca)
        sledecaPrereka = napraviObicnuPlusPreskakajucuPrepreku();
    else if(vrstaSledecePrepreke == Dvostruka)
        sledecaPrereka = napraviDvostrukuPrepreku();
    else if(vrstaSledecePrepreke == DvostrukaPlusPreskakajuca)
        sledecaPrereka = napraviDvostrukuPlusPreskakajucuPrepreku();
    else if(vrstaSledecePrepreke == PomerajucaObicna)
        sledecaPrereka = napraviPomerajucuObicnuPrepreku();
    else if(vrstaSledecePrepreke == PomerajucaObicnaPlusPreskakajuca)
        sledecaPrereka = napraviPomerajucuObicnuPlusPreskakajucuPrepreku();
    else if(vrstaSledecePrepreke == PomerajucaDvostruka)
        sledecaPrereka = napraviPomerajucuDvostrukuPrepreku();
    else if(vrstaSledecePrepreke == PomerajucaDvostrukaPlusPreskakajuca)
        sledecaPrereka = napraviPomerajucuDvostrukuPlusPreskakajucuPrepreku();
    
    sledecaPrereka.a.z = zPozicijaPrepreke;
    sledecaPrereka.b.z = zPozicijaPrepreke;
    sledecaPrereka.c.z = zPozicijaPrepreke;
    sledecaPrereka.vrsta = vrstaSledecePrepreke;
    
    return sledecaPrereka;
}
//1
static Prepreka napraviObicnuPrepreku(void)
{
    Prepreka x;
    x.a = napraviDeoPreprekeObicna(-sirinaPuta/2, sirinaPuta/2, 2);
    return x;
}
//2
static Prepreka napraviPreskakajucuPrepreku(void)
{
    Prepreka x;
    x.a = napraviDeoPreprekePreskakajuca();
    return x;
}
//3
static Prepreka napraviObicnuPlusPreskakajucuPrepreku(void)
{
    Prepreka x;
    x.a = napraviDeoPreprekeObicna(-sirinaPuta/2, sirinaPuta/2, 3);
    x.b = napraviDeoPreprekePreskakajuca();
    return x;
}
//4
static Prepreka napraviDvostrukuPrepreku(void)
{
    Prepreka x;
    x.a = napraviDeoPreprekeObicna(-sirinaPuta/2, 0, 2);
    x.b = napraviDeoPreprekeObicna(0, sirinaPuta/2, 2);
    return x;
}
//5
static Prepreka napraviDvostrukuPlusPreskakajucuPrepreku(void)
{
    Prepreka x;
    x.a = napraviDeoPreprekeObicna(-sirinaPuta/2, 0, 3);
    x.b = napraviDeoPreprekeObicna(0, sirinaPuta/2, 3);
    x.c = napraviDeoPreprekePreskakajuca();
    return x;
}
//6
static Prepreka napraviPomerajucuObicnuPrepreku(void)
{
    Prepreka x = napraviObicnuPrepreku();
    x.a.brzinaKretanja = randomRealnaVrednost(-maxBrzinaKretanja, maxBrzinaKretanja);
    return x;
}
//7
static Prepreka napraviPomerajucuObicnuPlusPreskakajucuPrepreku(void)
{
    Prepreka x = napraviObicnuPlusPreskakajucuPrepreku();
    x.a.brzinaKretanja = randomRealnaVrednost(-maxBrzinaKretanja, maxBrzinaKretanja);
    return x;
}
//8
static Prepreka napraviPomerajucuDvostrukuPrepreku(void)
{
    Prepreka x = napraviDvostrukuPrepreku();
    x.a.brzinaKretanja = randomRealnaVrednost(-maxBrzinaKretanja, maxBrzinaKretanja);
    x.b.brzinaKretanja = x.a.brzinaKretanja;
    return x;
}
//9
static Prepreka napraviPomerajucuDvostrukuPlusPreskakajucuPrepreku(void)
{
    Prepreka x = napraviDvostrukuPlusPreskakajucuPrepreku();
    x.a.brzinaKretanja = randomRealnaVrednost(-maxBrzinaKretanja, maxBrzinaKretanja);
    x.b.brzinaKretanja = x.a.brzinaKretanja;
    return x;
}

//prepreka po x-osi mora biti izmedju prosledjenih vrednosti x i y
static DeoPrepreke napraviDeoPreprekeObicna(double x, double y, double minVisina)
{
    DeoPrepreke a;
    
    a.sirina = randomRealnaVrednost(2, (2.0/3)*(y-x)); //maksimalno 2/3 moze da zauzme onog dela na kom se pravi
    a.visina = randomRealnaVrednost(minVisina, 5);
    a.x = randomRealnaVrednost(x + a.sirina/2, y - a.sirina/2);
    a.y = a.visina/2;
    
    return a;
    
}

static DeoPrepreke napraviDeoPreprekePreskakajuca()
{
    DeoPrepreke a;

    a.visina = randomCelobrojnaVrednost(1,  2);
    a.sirina = sirinaPuta;
    a.x = 0;
    a.y = a.visina/2;
    
    return a;
}

//Pokretne se crtaju kao te istog tipa sto nisu pokretne
//jedino sto im se menja (x) koordinata u tajmeru
static void nacrtajPrepreku(Prepreka p)
{
    switch(p.vrsta)
    {
        case Obicna:
            nacrtajDeoPrepreke(p.a);
            break;
        case Preskakajuca:
            nacrtajDeoPrepreke(p.a);
            break;
        case ObicnaPlusPreskakajuca:
            nacrtajDeoPrepreke(p.a);
            nacrtajDeoPrepreke(p.b);
            break;
        case Dvostruka:
            nacrtajDeoPrepreke(p.a);
            nacrtajDeoPrepreke(p.b);
            break;
        case DvostrukaPlusPreskakajuca:
            nacrtajDeoPrepreke(p.a);
            nacrtajDeoPrepreke(p.b);
            nacrtajDeoPrepreke(p.c);
            break;
        case PomerajucaObicna:
            nacrtajDeoPrepreke(p.a);
            break;
        case PomerajucaObicnaPlusPreskakajuca:
            nacrtajDeoPrepreke(p.a);
            nacrtajDeoPrepreke(p.b);
            break;
        case PomerajucaDvostruka:
            nacrtajDeoPrepreke(p.a);
            nacrtajDeoPrepreke(p.b);
            break;
        case PomerajucaDvostrukaPlusPreskakajuca:
            nacrtajDeoPrepreke(p.a);
            nacrtajDeoPrepreke(p.b);
            nacrtajDeoPrepreke(p.c);
            break;
    }
}

static void nacrtajDeoPrepreke(DeoPrepreke dp)
{
    glPushMatrix();
        glTranslatef(dp.x, dp.y, dp.z);
        glScalef(dp.sirina, dp.visina, 1);
        glutSolidCube(1);
    glPopMatrix();
}

void kretanjePomerajucihPrepreka(void)
{
    int i;
    for(i=0; i<brPreprekaNaPrvojPolovini; i++)
        kretanjePrepreke(prvaPolovina, i);
    for(i=0; i<brPreprekaNaDrugojPolovini; i++)
        kretanjePrepreke(drugaPolovina, i);
}

static void kretanjePrepreke(Prepreka *p, int i)
{
    switch(p[i].vrsta)
    {
        case PomerajucaObicna:
            p[i].a.x += p[i].a.brzinaKretanja;
            if(sudarDelaPreprekeSaZidom(p[i].a))
                p[i].a.brzinaKretanja *= -1;
            break;
        case PomerajucaObicnaPlusPreskakajuca:
            p[i].a.x += p[i].a.brzinaKretanja;
            if(sudarDelaPreprekeSaZidom(p[i].a))
                p[i].a.brzinaKretanja *= -1;
            break;
        case PomerajucaDvostruka:
            p[i].a.x += p[i].a.brzinaKretanja;
            p[i].b.x += p[i].b.brzinaKretanja;
            if(sudarDelaPreprekeSaZidom(p[i].a))
                p[i].a.brzinaKretanja *= -1;
            if(sudarDelaPreprekeSaZidom(p[i].b))
                p[i].b.brzinaKretanja *= -1;
            break;
        case PomerajucaDvostrukaPlusPreskakajuca:
            p[i].a.x += p[i].a.brzinaKretanja;
            p[i].b.x += p[i].b.brzinaKretanja;
            
            if(sudarDelaPreprekeSaZidom(p[i].a))
                p[i].a.brzinaKretanja *= -1;
            if(sudarDelaPreprekeSaZidom(p[i].b))
                p[i].b.brzinaKretanja *= -1;
            break;
    }
}

static int sudarDelaPreprekeSaZidom(DeoPrepreke pa)
{
    if(pa.x-pa.sirina/2 > -sirinaPuta/2 && pa.x+pa.sirina/2 < sirinaPuta/2)
        return 0;
    else return 1;
}



