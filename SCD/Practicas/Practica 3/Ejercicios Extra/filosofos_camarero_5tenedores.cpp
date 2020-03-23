// -----------------------------------------------------------------------------
//
// Sistemas concurrentes y Distribuidos.
// Práctica 3. Implementación de algoritmos distribuidos con MPI
//
// Archivo: filosofos-plantilla.cpp
// Implementación del problema de los filósofos (sin camarero).
// Plantilla para completar.
//
// Historial:
// Actualizado a C++11 en Diciembre de 2019
// Juan Manuel Mateos Pérez
// -----------------------------------------------------------------------------

#include <mpi.h>
#include <thread> // this_thread::sleep_for
#include <random> // dispositivos, generadores y distribuciones aleatorias
#include <chrono> // duraciones (duration), unidades de tiempo
#include <iostream>

using namespace std;
using namespace std::this_thread;
using namespace std::chrono;

// ---------------------------------------------------------------------
//  Variables Globales
// ---------------------------------------------------------------------

const int
   num_filosofos = 6,
   num_procesos  = num_filosofos+1,
   id_camarero   = num_procesos-1,
   etiq_empezar  = 0,
   etiq_terminar = 1,
   num_tenedores = 5;

// ---------------------------------------------------------------------
// Funciones Auxiliares
// ---------------------------------------------------------------------

template< int min, int max > int aleatorio()
{
  static default_random_engine generador( (random_device())() );
  static uniform_int_distribution<int> distribucion_uniforme( min, max ) ;
  return distribucion_uniforme( generador );
}

// ---------------------------------------------------------------------
//  Función Filósofos
// ---------------------------------------------------------------------

void funcion_filosofos(int id)
{
   int peticion;

   while (true) {
      cout << "El filósofo " << id << " solicita tenedor(es)" << endl;
      MPI_Ssend(&peticion, 1, MPI_INT, id_camarero, etiq_empezar, MPI_COMM_WORLD);
      
      cout << "El filósofo " << id << " comienza a comer" << endl;
      sleep_for( milliseconds( aleatorio<100,500>() ) );

      cout << "El filósofo " << id << " suelta tenedor(es)" << endl;
      MPI_Ssend(&peticion, 1, MPI_INT, id_camarero, etiq_terminar, MPI_COMM_WORLD);

      cout << "El filósofo " << id << " comienza a pensar" << endl;
      sleep_for( milliseconds( aleatorio<100, 500>() ) );
   }
}

// ---------------------------------------------------------------------
//  Función Camarero
// ---------------------------------------------------------------------

void funcion_camarero(){
   int ten_disponibles = num_tenedores,
       etiq_valida,
       valor;

   MPI_Status estado;

   while(true){
      if (ten_disponibles <= 1)
         etiq_valida = etiq_terminar;
      else
         etiq_valida = MPI_ANY_TAG;
   
      MPI_Recv(&valor, 1, MPI_INT, MPI_ANY_SOURCE, etiq_valida, MPI_COMM_WORLD, &estado);

      if (estado.MPI_TAG == etiq_terminar){
         if(estado.MPI_SOURCE == 1 || estado.MPI_SOURCE == 3)
            ten_disponibles++;
         else
            ten_disponibles += 2;                 
      }else{
         if(estado.MPI_SOURCE == 1 || estado.MPI_SOURCE == 3)
            ten_disponibles--;
         else
            ten_disponibles -= 2;  
      }

      cout << "Hay " << ten_disponibles << " tenedores disponibles " << endl; 
   }
}

// ---------------------------------------------------------------------
//  Main
// ---------------------------------------------------------------------

int main( int argc, char** argv )
{
   int id_propio, num_procesos_actual;

   MPI_Init( &argc, &argv );
   MPI_Comm_rank( MPI_COMM_WORLD, &id_propio );
   MPI_Comm_size( MPI_COMM_WORLD, &num_procesos_actual );

   if (num_procesos == num_procesos_actual){
      if(id_propio == id_camarero)
         funcion_camarero();
      else
         funcion_filosofos(id_propio);
   }
   else{
      if (id_propio == 0){
         cout << "el número de procesos esperados es:    " << num_procesos << endl
              << "el número de procesos en ejecución es: " << num_procesos_actual << endl
              << "(programa abortado)" << endl;
      }
   }

   MPI_Finalize( );
   return 0;
}
