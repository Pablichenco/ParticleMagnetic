
/*
 *=======================================================================*
 *      Modulo de resolucion de ecuaciones diferenciales vectoriales     *
 *      de 2do orden                                                     *
 *                                                                       *
 *      Metodo de Runge Kutta de 4to Orden : RK4                         *
 *                                                                       *
 *=======================================================================*
 *                                                                       *
 *      El modulo resuelve ecuaciones de la forma                        *
 *      d^2 r/dt^2=F(t,r,dr/dt)                                          *
 *                                                                       *
 *      r=(x,y,z)                                                        *
 *      v=(vx,vy,vz)                                                     *
 *      F(t,r,v)=(Fx,Fy,Fz)                                              *
 *                                                                       *
 *=======================================================================*
 *                                                                       *
 *      Se utiliza el cambio de variable                                 *
 *                                                                       *
 *      dr/dt=v    dv/dt=F(t,r,v)                                        *
 *                                                                       *
 *      Se obtiene un sistema de 6 ecuaciones de primer orden            *
 *                                                                       *
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


// Librerias Usadas
#include "modulo.h"
#include <stdio.h>
#include "math.h"
#include "stdlib.h"
#include "string.h"

// Numero de divicion del intervalo
#define N 500

// Vectores y matrices utilizadas
// Matris de parametros de RK4, k1,k2,k3,k4 (4) para x,y,z,vx,vy,vz (6)
double K[4][6];
// Las 6 funciones del sistema de ecuaciones y la suma de las Kaas
double F[6], SumaK[6];


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


//verificar archivo
int Verificar_Archivo(FILE *Arch){
    // Verificar que el archivo fue creado
    if (Arch == NULL){
        printf("No se pudo abrir el archivo, recompile!\n");
        return -1;
    }
    return 1;
}

//Crea un archivo con el nombre que le indiquen
FILE * AbrirArchivo(){
    FILE *Archivo;
    
    char Nombre[30];
    char extension[8]=".dat";
    
    //Dar nombre al archivo
    printf("Escriba el nombre del archivo al que desea guardar los datos (sin espacios, ni extencion): \n");
    scanf("%s",Nombre);
    printf("\n");
    
    // concatena extension a name
    strcat(Nombre, extension);
    
    //Crea el archivo
    Archivo= fopen(Nombre, "w+");
    
    // Verificar que el archivo fue creado
    Verificar_Archivo(Archivo);
    return Archivo;
}


// Inicializar los arreglos con las funciones deferenciales
void InicializarVectorFunciones(double t, Vector posicion, Vector velocidad, VectorF FunDif_1, VectorF FunDif_2){
    
    F[0]=FunDif_1.tipofunciondif_x( t, posicion, velocidad);
    F[1]=FunDif_1.tipofunciondif_y( t, posicion, velocidad);
    F[2]=FunDif_1.tipofunciondif_z( t, posicion, velocidad);
    
    F[3]=FunDif_2.tipofunciondif_x( t, posicion, velocidad);
    F[4]=FunDif_2.tipofunciondif_y( t, posicion, velocidad);
    F[5]=FunDif_2.tipofunciondif_z( t, posicion, velocidad);
}

// Sumar un vector con las K's asociadas a las variables de posicion
Vector SumaVectorPosicion(Vector a, double Constante, int i){
    a.vx=a.vx+Constante*K[i][0];
    a.vy=a.vy+Constante*K[i][1];
    a.vz=a.vz+Constante*K[i][2];
    return a;
}
// Sumar un vector con las K's asociadas a las variables de velocidad
Vector SumaVectorVelocidad(Vector a, double Constante, int i){
    a.vx=a.vx+Constante*K[i][3];
    a.vy=a.vy+Constante*K[i][4];
    a.vz=a.vz+Constante*K[i][5];
    return a;
}

// Ingrezar los datos de las K's: (k1,k2,k3,k4)(x,y,z,vx,vy,vz) = {K[0][j],K[1][j],K[2][j],K[3][j]}
void Inicializar_Kaas(double t, Vector posicion, Vector velocidad, VectorF FunDif_1, VectorF FunDif_2, double paso){
    

    InicializarVectorFunciones(t, posicion, velocidad, FunDif_1, FunDif_2);
    
    for (int j=0; j<6; j++) {
        K[0][j]=paso*F[j]; // K1
    }
    
    InicializarVectorFunciones(t+0.5*paso, SumaVectorPosicion(posicion, 0.5, 0), SumaVectorVelocidad(velocidad, 0.5, 0), FunDif_1, FunDif_2);
    
    for (int j=0; j<6; j++) {
        K[1][j]=paso*F[j]; // K2
    }
    
    InicializarVectorFunciones(t+0.5*paso, SumaVectorPosicion(posicion, 0.5, 1), SumaVectorVelocidad(velocidad, 0.5, 1), FunDif_1, FunDif_2);
    
    for (int j=0; j<6; j++) {
        K[2][j]=paso*F[j]; // K3
    }
    
    InicializarVectorFunciones(t+paso, SumaVectorPosicion(posicion, 1.0, 2), SumaVectorVelocidad(velocidad, 1.0, 2), FunDif_1, FunDif_2);
    
    for (int j=0; j<6; j++) {
        K[3][j]=paso*F[j]; // K4
    }
    
    
    
}

// Promedio de las K's asociadas a cada variable
void Suma_Kaas(){
    
    for (int j=0; j<6; j++) {
        SumaK[j]=( K[0][j]+ 2.0*K[1][j]+ 2.0*K[2][j]+ K[3][j])/6.0;
    }
    
    
}

//solucion de una ecuacion diferencial vectorial de segundo orden, vec(x'')=vec(f(t,vec(x),vec(x')))
double solucion(double liminf ,double limsup,Vector posicion_inicial, Vector velocidad_inicial, VectorF FunDif_1, VectorF FunDif_2, VectorF posicion_exacta, VectorF velocidad_exacta){
    
    // Se abre un archivo con el nombre que se le indique
    printf("\nAbriendo archivo de posiciones\n\n");
    FILE *archivo_posicion;
    archivo_posicion = AbrirArchivo();
    printf("\nAbriendo archivo de velocidades\n\n");
    FILE *archivo_velocidad;
    archivo_velocidad = AbrirArchivo();
    
    //Tamaño del paso
    double paso= (limsup-liminf)/N;
    
    //Variables dinamicas
    double t;
    Vector posicion,velocidad;
    Vector inter_posicion,inter_velocidad;
    
    //Inicializacion de las variales, condicion inicial
    t=liminf;
    
    posicion.vx=posicion_inicial.vx;
    posicion.vy=posicion_inicial.vy;
    posicion.vz=posicion_inicial.vz;
    
    velocidad.vx=velocidad_inicial.vx;
    velocidad.vy=velocidad_inicial.vy;
    velocidad.vz=velocidad_inicial.vz;
    
    //Guardar en archivo valores iniciales
    fprintf(archivo_posicion, "%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\n", t, posicion.vx, posicion.vy, posicion.vz, posicion_exacta.tipofunciondif_x(t,posicion,velocidad), posicion_exacta.tipofunciondif_y(t,posicion,velocidad), posicion_exacta.tipofunciondif_z(t,posicion,velocidad));
    fprintf(archivo_velocidad, "%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\n", t, velocidad.vx, velocidad.vy, velocidad.vz, velocidad_exacta.tipofunciondif_x(t,posicion,velocidad), velocidad_exacta.tipofunciondif_y(t,posicion,velocidad), velocidad_exacta.tipofunciondif_z(t,posicion,velocidad));
    
    
    for (int i=1; i<N; i++) {
        t=liminf+(double)(i)*paso;
        
        Inicializar_Kaas( t, posicion, velocidad, FunDif_1, FunDif_2, paso);
        Suma_Kaas();
        
        inter_posicion.vx=SumaK[0];
        inter_posicion.vy=SumaK[1];
        inter_posicion.vz=SumaK[2];
        
        inter_velocidad.vx=SumaK[3];
        inter_velocidad.vy=SumaK[4];
        inter_velocidad.vz=SumaK[5];
        
        // x vx
        posicion.vx+=inter_posicion.vx;
        velocidad.vx+=inter_velocidad.vx;
        // y vy
        posicion.vy+=inter_posicion.vy;
        velocidad.vy+=inter_velocidad.vy;
        //z vz
        posicion.vz+=inter_posicion.vz;
        velocidad.vz+=inter_velocidad.vz;
        
        
        fprintf(archivo_posicion, "%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\n", t, posicion.vx, posicion.vy, posicion.vz, posicion_exacta.tipofunciondif_x(t,posicion,velocidad), posicion_exacta.tipofunciondif_y(t,posicion,velocidad), posicion_exacta.tipofunciondif_z(t,posicion,velocidad));
        fprintf(archivo_velocidad, "%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\n", t, velocidad.vx, velocidad.vy, velocidad.vz, velocidad_exacta.tipofunciondif_x(t,posicion,velocidad), velocidad_exacta.tipofunciondif_y(t,posicion,velocidad), velocidad_exacta.tipofunciondif_z(t,posicion,velocidad));
        
        
        printf("diferencia %lf\n", posicion.vx-posicion_exacta.tipofunciondif_x(t,posicion,velocidad));
        
    }
    
    fclose(archivo_posicion);
    fclose(archivo_velocidad);
    
    return 0;
}
