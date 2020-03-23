// -----------------------------------------------------------------------------
// Juan Manuel Mateos Pérez
//
// Sistemas concurrentes y Distribuidos.
// Práctica 4. Implementación de sistemas de tiempo real.
//
// Archivo: ejecutivo1.cpp
// Implementación del primer ejemplo de ejecutivo cíclico:
//
//   Datos de las tareas:
//   ------------
//   Ta.  T    C
//   ------------
//   A  500   100
//   B  500   150
//   C  1000  200
//   D  2000  240
//  
//  -------------
//
//  Planificación (con Ts == 250 ms)
//  *---------*----------*---------*--------*
//  | A B C   | A B D    | A B C   | A B    |
//  *---------*----------*---------*--------*
//
/*

## ¿Cuál es el mínimo tiempo de espera que queda al final de las 
iteraciones del ciclo secundario con tu solución?

Mi ciclo secundario es de 500 milisegundos y el tiempo mínimo de 
espera que queda al final de las iteraciones del ciclo 
secundario es de 10 milisegundos. 
Que se corresponde al ciclo segundo donde se ejecuta la tarea A, B y D, 
con tiempos 100, 150, 240, respectivamente, sumando un total de 490 milisegundos 
frente a los 500 milisegundos del ciclo. Por tanto, la diferencia es 
de 10 milisegundos y es la mínima. 

## ¿Sería planificable si la tarea D tuviese un tiemo de cómputo de 250ms?

Según tengo la política de planificación, sería posible aumentar hasta 
250ms el tiempo de cómputo de la tarea D, permaneciendo invariante la 
planificicación. Aquí el tiempo mínimo de espera sería de 0ms
y correspondería al ciclo 2.

*/
// -----------------------------------------------------------------------------

#include <string>
#include <iostream> // cout, cerr
#include <thread>
#include <chrono>   // utilidades de tiempo
#include <ratio>    // std::ratio_divide

using namespace std ;
using namespace std::chrono ;
using namespace std::this_thread ;

// tipo para duraciones en segundos y milisegundos, en coma flotante:
typedef duration<float,ratio<1,1>>    seconds_f ;
typedef duration<float,ratio<1,1000>> milliseconds_f ;

// -----------------------------------------------------------------------------
// tarea genérica: duerme durante un intervalo de tiempo (de determinada duración)

void Tarea( const std::string & nombre, milliseconds tcomputo )
{
   cout << "   Comienza tarea " << nombre << " (C == " << tcomputo.count() << " ms.) ... " ;
   sleep_for( tcomputo );
   cout << "fin." << endl ;
}

// -----------------------------------------------------------------------------
// tareas concretas del problema:

void TareaA() { Tarea( "A", milliseconds(100) );  }
void TareaB() { Tarea( "B", milliseconds(150) );  }
void TareaC() { Tarea( "C", milliseconds(200) );  }
void TareaD() { Tarea( "D", milliseconds(240) );  }

// -----------------------------------------------------------------------------
// implementación del ejecutivo cíclico:

int main( int argc, char *argv[] )
{
   const milliseconds Ts( 500 );       // Ts = duración del ciclo secundario
   const milliseconds LIMITE(20);      // Tiempo máximo

   // ini_sec = instante de inicio de la iteración actual del ciclo secundario
   time_point<steady_clock> ini_sec = steady_clock::now();

   while(true){
      cout << endl
           << "---------------------------------------" << endl
           << "Comienza iteración del ciclo principal." << endl ;

      for( int i = 1 ; i <= 4 ; i++ ){    // ciclo secundario (4 iteraciones)
         cout << endl << "Comienza iteración " << i << " del ciclo secundario." << endl ;

         switch( i ){
            case 1 : TareaA(); TareaB(); TareaC(); break ;
            case 2 : TareaA(); TareaB(); TareaD(); break ;
            case 3 : TareaA(); TareaB(); TareaC(); break ;
            case 4 : TareaA(); TareaB(); TareaD(); break ;
         }

         // calcular el siguiente instante de inicio del ciclo secundario
         ini_sec += Ts ;
     
         // esperar hasta el inicio de la siguiente iteración del ciclo secundario
         sleep_until( ini_sec );

         time_point<steady_clock> comprobacion = steady_clock::now();
	      time_point<steady_clock>::duration retraso = comprobacion - ini_sec;

         if (retraso > LIMITE){
         	cerr << "Abortado: tiempo maximo superado " << endl;
            exit(EXIT_FAILURE);
         }else
		      cout << "El tiempo de retraso es de " << milliseconds_f(retraso).count() << " ms" << endl;
      }
   }
}
