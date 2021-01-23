/*
*=======================================================================*
* UNIVERSIDAD DE COSTA RICA                                             *
* ESCUELA DE FISICA                                                     *
* CURSO: FS0733 TOPICOS DE METODOS MATEMATICOS DE LA FISICA             *
* TEMA: FISICA COMPUTACIONAL Y PROGRAMACION EN C/C++                    *
* PROFESOR DAVID SOLANO SOLANO                                          *
*                                                                       *
* Codigo creado por Pablo Balmaceda Rescia                              *
*                                                                       *
* Copyleft (C inverted) 2017 Pablo Balmaceda Rescia                     *
*                                                                       *
* This program is free software: you can redistribute it and/or modify  *
* it under the terms of the GNU General Public License as published by  *
* the Free Software Foundation, either version 3 of the License, or     *
* (at your option) any later version.                                   *
*                                                                       *
* This program is distributed in the hope that it will be useful,       *
* but WITHOUT ANY WARRANTY; without even the implied warranty of        *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
* GNU General Public License for more details.                          *
*                                                                       *
* You should have received a copy of the GNU General Public License     *
* along with this program.  If not, see <http://www.gnu.org/licenses.   *
*                                                                       *
*                                                                       *
*=======================================================================*
*/


#include <stdio.h>
#include "math.h"
#include "modulo.h"


// constantes del sistema
// B0: campo magnetico constante, m: masa de la particula, q: carga de la particula
double B0,m,q;

double T0,X0,Y0,Z0,UX0,UY0,UZ0;


// Estructura de vector de doubles
struct vector {
    double vx;
    double vy;
    double vz;
};

typedef struct vector Vector;// redefinicion del tipo de dato vector de double como "Vector"

// Estructura de vector de punteros de funciones con argumentos (double, Vector, Vector)
struct vector_funciones {
    double(*tipofunciondif_x)(double t, Vector posicion, Vector velocidad);
    double(*tipofunciondif_y)(double t, Vector posicion, Vector velocidad);
    double(*tipofunciondif_z)(double t, Vector posicion, Vector velocidad);
};

typedef struct vector_funciones VectorF;// redefinicion del tipo de dato vector de funciones con argumentos (double, Vector, Vector) como "VectorF"

//Funciones diferenciales (el _1 representa el cambio de variable de dx/dt=vx)

// Posicion y velocidad en x
double funciondif_x_1(double t, Vector posicion, Vector velocidad);
double funciondif_x_2(double t, Vector posicion, Vector velocidad);

// Posicion y velocidad en y
double funciondif_y_1(double t, Vector posicion, Vector velocidad);
double funciondif_y_2(double t, Vector posicion, Vector velocidad);

// Posicion y velocidad en z
double funciondif_z_1(double t, Vector posicion, Vector velocidad);
double funciondif_z_2(double t, Vector posicion, Vector velocidad);

//Solucion exacta
double funcion_exacta_x(double t, Vector posicion, Vector velocidad);
double funcion_exacta_y(double t, Vector posicion, Vector velocidad);
double funcion_exacta_z(double t, Vector posicion, Vector velocidad);

double funcion_exacta_dx(double t, Vector posicion, Vector velocidad);
double funcion_exacta_dy(double t, Vector posicion, Vector velocidad);
double funcion_exacta_dz(double t, Vector posicion, Vector velocidad);

// funciones que inicializan las condiciones iniciales
Vector Inicializar_posicion(Vector posicion_inicial, double x0, double y0, double z0);
Vector Inicializar_velocidad(Vector velocidad_inicial, double vx0, double vy0, double vz0);

int main() {
    
    //double solucion(double liminf ,double limsup,Vector posicion_inicial, Vector velocidad_inicial, VectorF FunDif_1, VectorF FunDif_2, VectorF posicion_exacta, VectorF velocidad_exacta)
    
    T0=0.0;
    // Tiempo final
    double Tf=20.0;
    
    //constantes del sistema
    m=1.0;
    q=1.0;
    B0=1.0*q/m;
    
    Vector posicion_inicial;
    posicion_inicial.vx=0.0;
    posicion_inicial.vy=0.0;
    posicion_inicial.vz=0.0;
    
    Vector velocidad_inicial;
    velocidad_inicial.vx=0.0;
    velocidad_inicial.vy=0.0;
    velocidad_inicial.vz=0.0;
    
    VectorF FunDif_1;
    FunDif_1.tipofunciondif_x=funciondif_x_1;
    FunDif_1.tipofunciondif_y=funciondif_y_1;
    FunDif_1.tipofunciondif_z=funciondif_z_1;
    
    VectorF FunDif_2;
    FunDif_2.tipofunciondif_x=funciondif_x_2;
    FunDif_2.tipofunciondif_y=funciondif_y_2;
    FunDif_2.tipofunciondif_z=funciondif_z_2;
    
    VectorF posicion_exacta;
    posicion_exacta.tipofunciondif_x=funcion_exacta_x;
    posicion_exacta.tipofunciondif_y=funcion_exacta_y;
    posicion_exacta.tipofunciondif_z=funcion_exacta_z;
    
    VectorF velocidad_exacta;
    velocidad_exacta.tipofunciondif_x=funcion_exacta_dx;
    velocidad_exacta.tipofunciondif_y=funcion_exacta_dy;
    velocidad_exacta.tipofunciondif_z=funcion_exacta_dz;
    
    
    /*
    for (int n=0; n<3; n++) {
        posicion_inicial=Inicializar_posicion(posicion_inicial, 0.0, 1.0, 0.0);
        switch (n) {
            case 0:
                velocidad_inicial=Inicializar_velocidad(velocidad_inicial, 0.0, 1.0, 0.0);
                break;
            case 1:
                velocidad_inicial=Inicializar_velocidad(velocidad_inicial, 1.0, 0.0, 0.0);
                break;
            case 2:
                velocidad_inicial=Inicializar_velocidad(velocidad_inicial, 0.0, -1.0, 0.0);
                break;
        }
        printf("%i_Pos_x=%2.1lf_y=%2.1lf_z=%2.1lf_B0=%2.1lf\n", n, posicion_inicial.vx, posicion_inicial.vy, posicion_inicial.vz, B0);
        printf("%i_Vel_vx=%2.1lf_vy=%2.1lf_vz=%2.1lf_B0=%2.1lf\n", n, velocidad_inicial.vx, velocidad_inicial.vy, velocidad_inicial.vz, B0);
        solucion(T0, Tf, posicion_inicial, velocidad_inicial, FunDif_1, FunDif_2, posicion_exacta, velocidad_exacta);
    }
    
    for (int n=0; n<3; n++) {
        posicion_inicial=Inicializar_posicion(posicion_inicial, 0.0, 1.0, 0.0);
        switch (n) {
            case 0:
                velocidad_inicial=Inicializar_velocidad(velocidad_inicial, 0.0, 0.0, 1.0);
                break;
            case 1:
                velocidad_inicial=Inicializar_velocidad(velocidad_inicial, 0.0, -1.0, 1.0);
                break;
            case 2:
                velocidad_inicial=Inicializar_velocidad(velocidad_inicial, 0.0, 1.0, 1.0);
                break;
        }
        printf("%i_Pos_x=%2.1lf_y=%2.1lf_z=%2.1lf_B0=%2.1lf\n", n+3, posicion_inicial.vx, posicion_inicial.vy, posicion_inicial.vz, B0);
        printf("%i_Vel_vx=%2.1lf_vy=%2.1lf_vz=%2.1lf_B0=%2.1lf\n", n+3, velocidad_inicial.vx, velocidad_inicial.vy, velocidad_inicial.vz, B0);
        solucion(T0, Tf, posicion_inicial, velocidad_inicial, FunDif_1, FunDif_2, posicion_exacta, velocidad_exacta);
    }
    */
    
    for (int n=0; n<3; n++) {
        posicion_inicial=Inicializar_posicion(posicion_inicial, 0.0, 1.0, 0.0);
        velocidad_inicial=Inicializar_velocidad(velocidad_inicial, 0.0, 1.0, 1.0);
        switch (n) {
            case 0:
                B0=0.1*q/m;
                break;
            case 1:
                B0=0.5*q/m;
                break;
            case 2:
                B0=2.0*q/m;
                break;
                
            default:
                break;
        }
        printf("Pos_x=%2.1lf_y=%2.1lf_z=%2.1lf_B0=%2.1lf\n", posicion_inicial.vx, posicion_inicial.vy, posicion_inicial.vz, B0);
        printf("Vel_vx=%2.1lf_vy=%2.1lf_vz=%2.1lf_B0=%2.1lf\n", velocidad_inicial.vx, velocidad_inicial.vy, velocidad_inicial.vz, B0);
        solucion(T0, Tf, posicion_inicial, velocidad_inicial, FunDif_1, FunDif_2, posicion_exacta, velocidad_exacta);
    }
    
    
    
    return 0;
}

Vector Inicializar_posicion(Vector posicion_inicial, double x0, double y0, double z0){
    
    X0=x0;
    Y0=y0;
    Z0=z0;
    
    posicion_inicial.vx=X0;
    posicion_inicial.vy=Y0;
    posicion_inicial.vz=Z0;
    
    //printf(" dentro x=%lf\ty=%lf\tz=%lf\t\n",posicion_inicial.vx,posicion_inicial.vy,posicion_inicial.vz);
    return posicion_inicial;
}

Vector Inicializar_velocidad(Vector velocidad_inicial, double vx0, double vy0, double vz0){
    
    UX0=vx0;
    UY0=vy0;
    UZ0=vz0;
    
    velocidad_inicial.vx=UX0;
    velocidad_inicial.vy=UY0;
    velocidad_inicial.vz=UZ0;
    
    //printf("dentro vx=%lf\tvy=%lf\tvz=%lf\t\n",velocidad_inicial.vx,velocidad_inicial.vy,velocidad_inicial.vz);
    return velocidad_inicial;
}

double funciondif_x_1(double t, Vector posicion, Vector velocidad){
    return velocidad.vx;
}
double funciondif_x_2(double t, Vector posicion, Vector velocidad){
    return B0*velocidad.vy;
}

double funciondif_y_1(double t, Vector posicion, Vector velocidad){
    return velocidad.vy;
}
double funciondif_y_2(double t, Vector posicion, Vector velocidad){
    return -B0*velocidad.vx;
}

double funciondif_z_1(double t, Vector posicion, Vector velocidad){
    return velocidad.vz;
}
double funciondif_z_2(double t, Vector posicion, Vector velocidad){
    return 0.0;
}

double funcion_exacta_x(double t, Vector posicion, Vector velocidad){
    return UX0*sin(B0*t)/B0 +UY0*(1-cos(B0*t))/B0 +X0;
}
double funcion_exacta_y(double t, Vector posicion, Vector velocidad){
    return UY0*sin(B0*t)/B0 + UX0*(cos(B0*t)-1)/B0 +Y0;
}
double funcion_exacta_z(double t, Vector posicion, Vector velocidad){
    return UZ0*t+Z0;
}

double funcion_exacta_dx(double t, Vector posicion, Vector velocidad){
    return UX0*cos(B0*t)+UY0*sin(B0*t);
}
double funcion_exacta_dy(double t, Vector posicion, Vector velocidad){
    return UY0*cos(B0*t)-UX0*sin(B0*t);
}
double funcion_exacta_dz(double t, Vector posicion, Vector velocidad){
    return UZ0;
}
