#include <iostream>
#include <cassert>
#include <thread>
#include <mutex>
#include <random>
#include "Semaphore.h"

using namespace std ;
using namespace SEM ;

//**********************************************************************
// variables compartidas

const int num_items = 40 ,   // número de items
	       tam_vec   = 10 ;   // tamaño del buffer

unsigned  cont_prod[num_items] = {0}, // contadores de verificación: producidos
          cont_cons[num_items] = {0}; // contadores de verificación: consumidos

Semaphore libres    = tam_vec,
          ocupadas  = 0;

unsigned int primera_libre   = 0,
             n_productores   = 4,
             n_consumidores  = 4,
             n_producidos    = 0,
             n_consumidos    = 0;

int vect[tam_vec];

mutex mtx_primera_libre, 
      mtx_n_producidos, 
      mtx_n_consumidos;

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

//**********************************************************************
// funciones comunes a las dos soluciones (fifo y lifo)
//----------------------------------------------------------------------

int producir_dato()
{
   static int contador = 0 ;
   this_thread::sleep_for( chrono::milliseconds( aleatorio<20,100>() ));

   cout << "producido: " << contador << endl << flush ;

   cont_prod[contador] ++ ;
   return contador++ ;
}
//----------------------------------------------------------------------

void consumir_dato( unsigned dato )
{
   assert( dato < num_items );
   cont_cons[dato] ++ ;
   this_thread::sleep_for( chrono::milliseconds( aleatorio<20,100>() ));

   cout << "                  consumido: " << dato << endl ;

}


//----------------------------------------------------------------------

void test_contadores()
{
   bool ok = true ;
   cout << "comprobando contadores ...." ;
   for( unsigned i = 0 ; i < num_items ; i++ )
   {  if ( cont_prod[i] != 1 )
      {  cout << "error: valor " << i << " producido " << cont_prod[i] << " veces." << endl ;
         ok = false ;
      }
      if ( cont_cons[i] != 1 )
      {  cout << "error: valor " << i << " consumido " << cont_cons[i] << " veces" << endl ;
         ok = false ;
      }
   }
   if (ok)
      cout << endl << flush << "solución (aparentemente) correcta." << endl << flush ;
}

//----------------------------------------------------------------------

void  funcion_hebra_productora(  )
{  
   while(true){
      mtx_n_producidos.lock();
         if(!(n_producidos < num_items)){
            mtx_n_producidos.unlock();
            break;
         }
         n_producidos++;
      mtx_n_producidos.unlock();

      int dato = producir_dato();
      sem_wait(libres);
         mtx_primera_libre.lock();

            vect[primera_libre] = dato;
            primera_libre++;

         mtx_primera_libre.unlock();
      sem_signal(ocupadas);
   }
}

//----------------------------------------------------------------------

void funcion_hebra_consumidora(  )
{
   while(true){
      mtx_n_consumidos.lock();
         if(!(n_consumidos < num_items)){
            mtx_n_consumidos.unlock();
            break;
         }
         n_consumidos++;
      mtx_n_consumidos.unlock();

      int dato;
      sem_wait(ocupadas);
         mtx_primera_libre.lock();

            primera_libre--;
            dato = vect[primera_libre];

         mtx_primera_libre.unlock();
      sem_signal(libres);
      consumir_dato(dato);
   }
}

// Hay que indicar el mtx_primera_libre.lock() and unlock() porque puede haber zonas 
// críticas entre la lectura y escritura ya que usan la misma variable 

//----------------------------------------------------------------------

int main()
{
   cout << "----------------------------------------------------------------" << endl
        << "Problema de los productores-consumidores (solución VARIOS LIFO)." << endl
        << "----------------------------------------------------------------" << endl
        << flush ;

   thread a1 [n_productores],
          a2 [n_consumidores];

   for(int i=0; i<n_productores; ++i)
      a1[i] = thread(funcion_hebra_productora);

   for(int i=0; i<n_consumidores; ++i)   
      a2[i] = thread(funcion_hebra_consumidora);

   for(int i=0; i<n_productores; ++i)
      a1[i].join();

   for(int i=0; i<n_consumidores; ++i)   
      a2[i].join();

   test_contadores();
}
