//
//  modulo.h
//

#ifndef modulo_h
#define modulo_h

#include <stdio.h>

//Crear archivo
FILE * AbrirArchivo();
//verificar archivo
int Verificar_Archivo(FILE *Arch);

// Estructura de vector de doubles
struct vector;

typedef struct vector Vector;// redefinicion del tipo de dato vector de double como "Vector"

// Estructura de vector de punteros de funciones con argumentos (double, Vector, Vector)
struct vector_funciones;

typedef struct vector_funciones VectorF;// redefinicion del tipo de dato vector de funciones con argumentos (double, Vector, Vector) como "VectorF"


// Inicializar los arreglos con las funciones deferenciales
void InicializarVectorFunciones(double t, Vector posicion, Vector velocidad, VectorF FunDif_1, VectorF FunDif_2);

// Sumar un vector con las K's asociadas a las variables de posicion
Vector SumaVectorPosicion(Vector a, double Constante, int i);

// Sumar un vector con las K's asociadas a las variables de velocidad
Vector SumaVectorVelocidad(Vector a, double Constante, int i);

// Ingrezar los datos de las K's: (k1,k2,k3,k4)(x,y,z,vx,vy,vz) = {K[0][j],K[1][j],K[2][j],K[3][j]}
void Inicializar_Kaas(double t, Vector posicion, Vector velocidad, VectorF FunDif_1, VectorF FunDif_2, double paso);

//suma de las K's
void Suma_Kaas();

//solucion
double solucion(double liminf ,double limsup,Vector posicion_inicial, Vector velocidad_inicial, VectorF FunDif_1, VectorF FunDif_2, VectorF posicion_exacta, VectorF velocidad_exacta);


#endif /* modulo_h */
