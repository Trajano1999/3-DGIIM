#include <iostream>
#include <cassert>
#include <thread>
#include <mutex>
#include <random> // dispositivos, generadores y distribuciones aleatorias
#include <chrono> // duraciones (duration), unidades de tiempo
#include "Semaphore.h"

using namespace std ;
using namespace SEM ;

Semaphore puede_estancar    = 1,
          puede_mostrar     = 1,
          puede_suministrar = 1,
          puede_fumar[3]    = {0,0,0};

int mostrado = 0;

mutex mtx_cout;

//**********************************************************************
// plantilla de función para generar un entero aleatorio uniformemente
// distribuido entre dos valores enteros, ambos incluidos
// (ambos tienen que ser dos constantes, conocidas en tiempo de compilación)
//----------------------------------------------------------------------

template< int min, int max > int aleatorio()
{
  static default_random_engine generador( (random_device())() );
  static uniform_int_distribution<int> distribucion_uniforme( min, max ) ;
  return distribucion_uniforme( generador );
}

//----------------------------------------------------------------------
// función que ejecuta la hebra del estanquero

void funcion_hebra_estanquero()
{  
   while(true){
      sem_wait(puede_estancar);
      for(int i=0; i<10; ++i){
         sem_wait(puede_mostrar);
         mostrado = aleatorio<0,2>();
         mtx_cout.lock();
            cout << "El estanquero ha depositado el ingrediente: " << mostrado << endl << endl
                 << "\t\t\tIteracion: " << i+1 << endl;
         mtx_cout.unlock();
         sem_signal(puede_fumar[mostrado]);
      }
      sem_signal(puede_suministrar);
   }
}

//-------------------------------------------------------------------------
// Función que simula la acción de fumar, como un retardo aleatoria de la hebra

void fumar( int num_fumador )
{
   // calcular milisegundos aleatorios de duración de la acción de fumar)
   chrono::milliseconds duracion_fumar( aleatorio<20,200>() );

   // informa de que comienza a fumar

    cout << "Fumador " << num_fumador << "  :"
          << " empieza a fumar (" << duracion_fumar.count() << " milisegundos)" << endl;

   // espera bloqueada un tiempo igual a ''duracion_fumar' milisegundos
   this_thread::sleep_for( duracion_fumar );

   // informa de que ha terminado de fumar

    cout << "Fumador " << num_fumador << "  : termina de fumar, comienza espera de ingrediente." << endl;
}

//----------------------------------------------------------------------
// función que ejecuta la hebra del fumador
void  funcion_hebra_fumador( int num_fumador ){
   while(true){
      puede_fumar[num_fumador].sem_wait();
      fumar(num_fumador);
      puede_mostrar.sem_signal();
   }
}

//----------------------------------------------------------------------

void funcion_suministrar(){
   while(true){
      sem_wait(puede_suministrar);
      mtx_cout.lock();
         cout << "\tHa llegado el nuevo suministro!" << endl << endl;
      mtx_cout.unlock();
      this_thread::sleep_for(chrono::milliseconds(aleatorio<20, 200>()));
      sem_signal(puede_estancar);
   }
}

//----------------------------------------------------------------------

int main(){
   thread hebra_estanquero(funcion_hebra_estanquero),
          hebra_fumador1(funcion_hebra_fumador,0),
          hebra_fumador2(funcion_hebra_fumador,1),
          hebra_fumador3(funcion_hebra_fumador,2),
          hebra_suministradora(funcion_suministrar);
   
   hebra_estanquero.join();
   hebra_fumador1.join();
   hebra_fumador2.join();
   hebra_fumador3.join();
   hebra_suministradora.join();
}