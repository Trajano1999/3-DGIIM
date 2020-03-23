#include <iostream>
#include <cassert>
#include <thread>
#include <mutex>
#include <random> // dispositivos, generadores y distribuciones aleatorias
#include <chrono> // duraciones (duration), unidades de tiempo
#include "Semaphore.h"

using namespace std ;
using namespace SEM ;

Semaphore puede_gestionar   = 1,
          puede_mostrar     = 1,
          puede_estancar[2] = {0,0},
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

void funcion_hebra_estanquero(int estanquero)
{  
   while(true){
      sem_wait(puede_estancar[estanquero]);
      for(int i=0; i<5; ++i){
         sem_wait(puede_mostrar);
         mostrado = aleatorio<0,2>();
         mtx_cout.lock();
            cout << "El estanquero " << estanquero << " ha depositado el ingrediente: " << mostrado << endl;
         mtx_cout.unlock();
         puede_fumar[mostrado].sem_signal();
      }
      sem_signal(puede_gestionar);
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
      sem_signal(puede_mostrar);
   }
}

//----------------------------------------------------------------------

void funcion_gestor(){
   while(true){
      sem_wait(puede_gestionar);
      int estanquero_elegido = aleatorio<0,1>();
      mtx_cout.lock();
         cout << "\tEl gestor ha elegido al estanquero " << estanquero_elegido << endl;
      mtx_cout.unlock();
      sem_signal(puede_estancar[estanquero_elegido]);
   }
}

//----------------------------------------------------------------------

int main(){
   thread hebra_estanquero1(funcion_hebra_estanquero,0),
          hebra_estanquero2(funcion_hebra_estanquero,1),
          hebra_fumador1(funcion_hebra_fumador,0),
          hebra_fumador2(funcion_hebra_fumador,1),
          hebra_fumador3(funcion_hebra_fumador,2),
          gestor(funcion_gestor);
   
   hebra_estanquero1.join();
   hebra_estanquero2.join();
   hebra_fumador1.join();
   hebra_fumador2.join();
   hebra_fumador3.join();
   gestor.join();
}
