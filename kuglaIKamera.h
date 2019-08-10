#ifndef __kuglaIKamera__h
#define __kuglaIKamera__h

//(x, y, z) - centar sfere
typedef struct sfera 
{
    double x;
    double y;
    double z;
    
    double r;
    
} Sfera;

extern Sfera kugla;
extern double rotacijaKugle;

extern void nacrtajLoptu();
extern void postaviKameru();


#endif
