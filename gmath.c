#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "gmath.h"
#include "matrix.h"
#include "ml6.h"

//lighting functions
color get_lighting( double *normal, double *view, color alight, double light[2][3], double *areflect, double *dreflect, double *sreflect) {
  normalize(normal);
  normalize(light[LOCATION]);
  normalize(view);
  
  color ambient=calculate_ambient(alight, areflect);
  color diffuse=calculate_diffuse(light, dreflect, normal);
  color specular=calculate_specular(light, sreflect, view, normal);
  //printf("red: %d, green: %d, blue: %d\n",i.red,i.green,i.blue);
  color i;
  i.red=ambient.red+diffuse.red+specular.red;
  i.green=ambient.green+diffuse.green+specular.green;
  i.blue=ambient.blue+diffuse.blue+specular.blue;
  limit_color(&i);
  return i;
}

color calculate_ambient(color alight, double *areflect ) {
  color a;
  a.red=alight.red*areflect[RED];
  a.green=alight.green*areflect[GREEN];
  a.blue=alight.blue*areflect[BLUE];
  limit_color(&a);
  
  return a;
}

color calculate_diffuse(double light[2][3], double *dreflect, double *normal ) {
  color d;
  //P*Kd*(N dot L)
  
  double dot=dot_product(normal, light[LOCATION]);//how calculate light vector
  
  d.red=dreflect[RED]*light[COLOR][RED]*(dot);
  d.green=dreflect[GREEN]*light[COLOR][GREEN]*(dot);
  d.blue=dreflect[BLUE]*light[COLOR][BLUE]*(dot);
  
  limit_color(&d);
  return d;
}

//P*Ks*[2*(N dot L)*N-L)dotV]^x
color calculate_specular(double light[2][3], double *sreflect, double *view, double *normal ) {
  color s;

  s.red=0;
  s.green=0;
  s.blue=0;

  if (dot_product(normal, light[LOCATION])<=0)
    return s;

  double vals[3];
  vals[RED]=2*dot_product(normal,light[LOCATION])*normal[RED]-light[LOCATION][RED];
  vals[GREEN]=2*dot_product(normal,light[LOCATION])*normal[GREEN]-light[LOCATION][GREEN];
  vals[BLUE]=2*dot_product(normal,light[LOCATION])*normal[BLUE]-light[LOCATION][BLUE];

  //normalize(vals);



  double dot=pow(dot_product(vals,view),15);
  //vals[GREEN]=pow(dot_product(vals[GREEN],view),16);
  //vals[BLUE]=pow(dot_product(vals[BLUE],view),16);

  s.red=light[COLOR][RED]*sreflect[RED]*dot;
  s.green=light[COLOR][GREEN]*sreflect[GREEN]*dot;
  s.blue=light[COLOR][BLUE]*sreflect[BLUE]*dot;

  limit_color(&s);
  
  return s;
}


//limit each component of c to a max of 255
void limit_color( color * c ) {
  if (c->red>255)
    c->red=255;
  if (c->red<0)
    c->red=0;
  if (c->green>255)
    c->green=255;
  if (c->green<0)
    c->green=0;
  if (c->blue>255)
    c->blue=255;
  if (c->blue<0)
    c->blue=0;
}

//vector functions
//normalize vetor, should modify the parameter
void normalize( double *vector ) {
  //divide all components by magnitude
  double magnitude=sqrt(vector[0]*vector[0]+vector[1]*vector[1]+vector[2]*vector[2]);
  vector[0]=vector[0]/magnitude;
  vector[1]=vector[1]/magnitude;
  vector[2]=vector[2]/magnitude;
  
}

//Return the dot porduct of a . b
double dot_product( double *a, double *b ) {
  //AxBx+AyBy+AzBz;  
  return a[0]*b[0]+a[1]*b[1]+a[2]*b[2];
}

double *calculate_normal(struct matrix *polygons, int i) {

  double A[3];
  double B[3];
  double *N = (double *)malloc(3 * sizeof(double));

  A[0] = polygons->m[0][i+1] - polygons->m[0][i];
  A[1] = polygons->m[1][i+1] - polygons->m[1][i];
  A[2] = polygons->m[2][i+1] - polygons->m[2][i];

  B[0] = polygons->m[0][i+2] - polygons->m[0][i];
  B[1] = polygons->m[1][i+2] - polygons->m[1][i];
  B[2] = polygons->m[2][i+2] - polygons->m[2][i];

  N[0] = A[1] * B[2] - A[2] * B[1];
  N[1] = A[2] * B[0] - A[0] * B[2];
  N[2] = A[0] * B[1] - A[1] * B[0];

  return N;
}
