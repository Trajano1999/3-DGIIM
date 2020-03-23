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
   num_filosofos = 5,
   num_procesos  = 2*num_filosofos+1,
   id_camarero   = num_procesos-1,
   etiq_sentar   = 0,
   etiq_levantar = 1;

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
   int peticion,
       id_ten_izq = (id+1)              % (num_procesos-1),     //id. tenedor izq.
       id_ten_der = (id+num_procesos-2) % (num_procesos-1);     //id. tenedor der.

   while (true) {
      cout << "Filósofo " << id << ": solicita sentarse en la mesa" << endl;
      MPI_Ssend(&peticion, 1, MPI_INT, id_camarero, etiq_sentar, MPI_COMM_WORLD);

      cout << "Filósofo " << id << ": solicita el tenedor izq. " << id_ten_izq << endl;
      MPI_Ssend(&peticion, 1, MPI_INT, id_ten_izq, 0, MPI_COMM_WORLD);
      cout << "Filósofo " << id << ": solicita el tenedor der. " << id_ten_der << endl;
      MPI_Ssend(&peticion, 1, MPI_INT, id_ten_der, 0, MPI_COMM_WORLD);

      cout << "Filósofo " << id << ": comienza a comer" << endl;
      sleep_for( milliseconds( aleatorio<100,500>() ) );

      cout << "Filósofo " << id << ": suelta el tenedor izq. " << id_ten_izq << endl;
      MPI_Ssend(&peticion, 1, MPI_INT, id_ten_izq, 0, MPI_COMM_WORLD);
      cout << "Filósofo " << id << ": suelta el tenedor der. " << id_ten_der << endl;
      MPI_Ssend(&peticion, 1, MPI_INT, id_ten_der, 0, MPI_COMM_WORLD);

      cout << "Filósofo " << id << ": solicita levantarse de la mesa" << endl;
      MPI_Ssend(&peticion, 1, MPI_INT, id_camarero, etiq_levantar, MPI_COMM_WORLD);

      cout << "Filosofo " << id << ": comienza a pensar" << endl;
      sleep_for( milliseconds( aleatorio<100, 500>() ) );
   }
}

// ---------------------------------------------------------------------
//  Función Tenedores
// ---------------------------------------------------------------------

void funcion_tenedores(int id)
{
   int valor,           // valor recibido
       id_filosofo;     // identificador del filósofo
   
   MPI_Status estado;   // metadatos de las dos recepciones

   while(true){
      // Recibir petición de cualquier filósofo
      MPI_Recv(&valor, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &estado);

      // Guardar en 'id_filosofo' el id. del emisor
      id_filosofo = estado.MPI_SOURCE;
      cout << "El tenedor " << id << " ha sido cogido por el filósofo " << id_filosofo << endl;

      // Recibir liberación de filósofo 'id_filosofo'
      MPI_Recv(&valor, 1, MPI_INT, id_filosofo, 0, MPI_COMM_WORLD, &estado);
      cout << "El tenedor " << id << " ha sido liberado por el filósofo " << id_filosofo << endl;
   }
}

// ---------------------------------------------------------------------
//  Función Camarero
// ---------------------------------------------------------------------

void funcion_camarero(){
   int sentados = 0, 
       max_sentados = num_filosofos-1,
       etiq_valida,
       valor;

   MPI_Status estado;

   while(true){
      if (sentados >= max_sentados)
         etiq_valida = etiq_levantar;
      else
         etiq_valida = MPI_ANY_TAG;
   
      MPI_Recv(&valor, 1, MPI_INT, MPI_ANY_SOURCE, etiq_valida, MPI_COMM_WORLD, &estado);

      if (estado.MPI_TAG == etiq_sentar)
         ++sentados;
      else
         --sentados;
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
      if(id_propio == num_procesos-1)
         funcion_camarero();
      else if(id_propio % 2 == 0)
         funcion_filosofos(id_propio); 
      else
         funcion_tenedores(id_propio);
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
