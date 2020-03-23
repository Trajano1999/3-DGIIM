/**
 * Solución al problema de los productores  y consumidores
 * - MPI
 * - Buffer de almacenamiento intermedio
 * - Múltiples productores y consumidores
 */

#include <iostream>
#include <thread>
#include <random>
#include <chrono>
#include <mpi.h>

using namespace std;
using namespace std::this_thread ;
using namespace std::chrono ;

// ---------------------------------------------------------------------
// variables globales
const int num_productores = 3,
          num_consumidores = 2,
          num_procesos_esperado = num_productores + num_consumidores + 1,
          num_items = num_productores * num_consumidores * 2,
          tam_vec = 10,
          bloque_productor = num_items / num_productores,
          bloque_consumidor = num_items / num_consumidores;

const int etiq_productor = 0,
          etiq_consumidor = 1;

//-----------------------------------------------------------------------
// funciones auxiliares

template< int min, int max > int aleatorio()
{
  static default_random_engine generador( (random_device())() );
  static uniform_int_distribution<int> distribucion_uniforme( min, max ) ;
  return distribucion_uniforme( generador );
}

int producir(int id_productor)
{
   static int contador = id_productor * bloque_productor;
   sleep_for( milliseconds( aleatorio<10,200>()) );
   contador++ ;
   cout << "Productor " << id_productor << ": ha producido valor " << contador << endl << flush;
   return contador ;
}

void consumir (int valor_cons, int id_consumidor)
{
   // espera bloqueada
   sleep_for( milliseconds( aleatorio<10,200>()) );
   cout << "Consumidor " << id_consumidor << ": ha consumido valor " << valor_cons << endl << flush ;
}


//-------------------------------------------------------------------------
// Funciones de procesos

void funcion_productor(int id_productor) {
    int valor;
    for (int i = 0; i < bloque_productor; ++i) {
        // producir valor:
        valor = producir(id_productor);

        // enviar valor:
        cout << "Productor " << id_productor << ": va a enviar valor " << valor << endl;
        MPI_Ssend(&valor, 1, MPI_INT, num_productores, etiq_productor, MPI_COMM_WORLD); // el destinatario es el proceso buffer, cuyo id es num_productores
    }
}

void funcion_buffer () {
    int buffer[tam_vec], // buffer de almacenamiento
        valor,
        ocupadas = 0,
        write_index = 0, // índice de escritura en el buffer
        read_index = 0, // índice de lectura en el buffer
        etiq_emisor; // etiqueta de los emisores acceptables
    MPI_Status estado;
    int id_fuente;

    for (int i = 0; i < num_items*2; ++i) {
        // 1. Determinar si se puede enviar sólo a productor, sólo a consumidorue o a ambos
        if (ocupadas == 0)
            etiq_emisor = etiq_productor;
        else if (ocupadas == tam_vec)
            etiq_emisor = etiq_consumidor;
        else
            etiq_emisor = MPI_ANY_TAG;
        
        // 2. Recibir un mensaje del emisor aceptable
        MPI_Recv(& valor, 1, MPI_INT, MPI_ANY_SOURCE, etiq_emisor, MPI_COMM_WORLD, &estado);

        // 3. Procesar el mensaje recibido
        id_fuente = estado.MPI_SOURCE;
        if (id_fuente < num_productores) { // un productor ha enviado un valor, insertar en el buffer
            buffer[write_index] = valor;
            write_index = (write_index + 1) % tam_vec;
            ++ocupadas;
               
            cout << "Buffer: recibido el valor " << valor << " del productor " << id_fuente << endl;
        }
        else { // un consumidor ha hecho una petición de extracción de un valor del buffer
            valor = buffer[read_index];  
            read_index = (read_index + 1) % tam_vec;
            --ocupadas;

            cout << "Buffer: va a enviar valor " << valor << " al consumidor " << id_fuente - num_productores - 1 << endl;
            MPI_Ssend(&valor, 1, MPI_INT, id_fuente, etiq_consumidor, MPI_COMM_WORLD);
        }
    }
}

void funcion_consumidor (int id_consumidor) {
    int peticion,
        valor_recibido = -1;
    MPI_Status estado;

    for (int i = 0; i < bloque_consumidor; ++i) {
        // Hacer petición de lectura de un dato:
        cout << "Consumidor " << id_consumidor << ": realiza una petición de valor al proceso buffer" << endl;
        MPI_Ssend(&peticion, 1, MPI_INT, num_productores, etiq_consumidor, MPI_COMM_WORLD);

        // Recibir el dato:
        MPI_Recv(&valor_recibido, 1, MPI_INT, num_productores, etiq_consumidor, MPI_COMM_WORLD, &estado);
        cout << "Consumidor " << id_consumidor << ": ha recibido valor " << valor_recibido << endl;
        
        // Consumir el dato:
        consumir(valor_recibido, id_consumidor);
    }
}

int main (int argc, char* argv[]) {
    int id_propio, num_procesos_actual;

    MPI_Init(&argc, &argv); // inicializar MPI
    MPI_Comm_rank(MPI_COMM_WORLD, &id_propio); // obtener id_propio del proceso
    MPI_Comm_size(MPI_COMM_WORLD, &num_procesos_actual); // obtener el número total de procesos lanzados


    if (num_procesos_esperado == num_procesos_actual) { // comprobar que se ha lanzado la cantidad correcta de procesos
        if (id_propio < num_productores)
            funcion_productor(id_propio);
        else if (id_propio == num_productores)
            funcion_buffer();
        else
            funcion_consumidor(id_propio-num_productores-1);
    }
    else if (id_propio == 0) // el primer proceso informa del error
        cout << "el número de procesos esperados es:    " << num_procesos_esperado << endl
             << "el número de procesos en ejecución es: " << num_procesos_actual << endl
             << "(programa abortado)" << endl;

    MPI_Finalize(); // Finalizar MPI
    
    return 0;
}