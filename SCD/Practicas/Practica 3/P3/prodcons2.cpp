#include <iostream>
#include <thread> // this_thread::sleep_for
#include <random> // dispositivos, generadores y distribuciones aleatorias
#include <chrono> // duraciones (duration), unidades de tiempo
#include <mpi.h>

using namespace std;
using namespace std::this_thread ;
using namespace std::chrono ;

// ---------------------------------------------------------------------
//  Variables Globales
// ---------------------------------------------------------------------

const int
   etiq_productor        = 0,
   etiq_consumidor       = 2,

   num_productores       = 2,
   num_consumidores      = 2,

   num_procesos_esperado = num_productores + num_consumidores + 1,
   num_items             = num_productores * num_consumidores * 2,
   tam_vector            = 10,

   proc_productor        = num_items/num_productores,
   proc_consumidor       = num_items/num_consumidores;

// ---------------------------------------------------------------------
// Funciones Auxliares
// ---------------------------------------------------------------------

template< int min, int max > int aleatorio()
{
  static default_random_engine generador( (random_device())() );
  static uniform_int_distribution<int> distribucion_uniforme( min, max ) ;
  return distribucion_uniforme( generador );
}

void presentacion()
{
   cout << "\n<PROGRAMA DE PRODUCTORES Y CONSUMIDORES>" << endl
        << "\tNúmero productores       : " << num_productores << endl
        << "\tNúmero consumidores      : " << num_consumidores << endl
        << "\tTamaño del vector        : " << tam_vector << endl
        << "\tNúmero procesos esperados: " << num_procesos_esperado << endl;
}

// ---------------------------------------------------------------------
// Producir produce los numeros en secuencia (1,2,3,....)
// ---------------------------------------------------------------------

int producir(int id_productor)
{
   static int contador = 0;
   sleep_for( milliseconds( aleatorio<10,100>()) );
   contador++;
   cout << "El productor " << id_productor << " ha producido el valor " << contador << endl << flush;
   return contador;
}

void funcion_productor(int id_productor)
{
   for (unsigned int i=0; i<proc_productor; ++i){
      // producir valor
      int valor = producir(id_productor);

      // enviar valor
      cout << "El productor " << id_productor << " va a enviar el valor " << valor << endl << flush;
      MPI_Ssend(&valor, 1, MPI_INT, num_productores, etiq_productor, MPI_COMM_WORLD);
      //Aqui estoy enviando un valor de tamaño 1 que es entero, al bufer con la etiqueta del productor
   }
}

// ---------------------------------------------------------------------
// Consumir los valores
// ---------------------------------------------------------------------

void consumir(int valor_cons, int id_consumidor)
{
   // espera bloqueada
   sleep_for( milliseconds( aleatorio<110,200>()) );
   cout << "El consumidor " << id_consumidor << " ha recibido el valor " << valor_cons << endl << flush ;
}

void funcion_consumidor(int id_consumidor)
{
   int peticion,
       valor_recibido = 1;
   MPI_Status estado;

   for(unsigned int i=0; i<proc_consumidor; ++i){
      //Hacer peticion de lectura de un dato
      cout << "El consumidor " << id_consumidor << " realiza una peticion al proceso buffer " << endl << flush;
      MPI_Ssend( &peticion,  1, MPI_INT, num_productores, etiq_consumidor, MPI_COMM_WORLD);
      
      //Recibe el valor
      MPI_Recv (&valor_recibido, 1, MPI_INT, num_productores, etiq_consumidor, MPI_COMM_WORLD,&estado);
      consumir(valor_recibido, id_consumidor);
   }
}

// ---------------------------------------------------------------------
// Buffer
// ---------------------------------------------------------------------

void funcion_buffer()
{
   int buffer[tam_vector],      // buffer con celdas ocupadas y vacías
       valor,                   // valor recibido o enviado
       primera_libre       = 0, // índice de primera celda libre
       primera_ocupada     = 0, // índice de primera celda ocupada
       ocupadas            = 0, // número de celdas ocupadas
       etiq_emisor,             // identificador de emisor aceptable
       etiq_fuente;

   MPI_Status estado ;          // metadatos del mensaje recibido

   for(unsigned int i=0; i<num_items*2; ++i){
      // 1. Determinar si puede enviar solo a productor, solo consumidor, o ambos
      if (ocupadas == 0)
         etiq_emisor = etiq_productor;
      else if (ocupadas == tam_vector)
         etiq_emisor = etiq_consumidor;
      else                                          
         etiq_emisor = MPI_ANY_TAG;    // cualquiera

      // 2. Recibir un mensaje del emisor
      MPI_Recv(&valor, 1, MPI_INT, MPI_ANY_SOURCE, etiq_emisor, MPI_COMM_WORLD, &estado);

      // 3. Procesar el mensaje recibido
      etiq_fuente = estado.MPI_SOURCE;

      if(etiq_fuente < num_productores){   // si ha sido el productor: insertar en buffer
         buffer[primera_libre] = valor;
         primera_libre = (primera_libre+1) % tam_vector;
         ocupadas++;
         cout << "El buffer ha recibido el valor " << valor << " del productor " << etiq_fuente << endl;
      }else{   // si ha sido el consumidor: extraer y enviar
         valor = buffer[primera_ocupada];
         primera_ocupada = (primera_ocupada+1) % tam_vector;
         ocupadas--;
         cout << "El buffer va a enviar el valor " << valor << " al consumidor " << etiq_fuente << endl;
         MPI_Ssend( &valor, 1, MPI_INT, etiq_fuente, etiq_consumidor, MPI_COMM_WORLD);
      }
   }
}

// ---------------------------------------------------------------------
// Main
// ---------------------------------------------------------------------

int main( int argc, char *argv[] )
{
   int id_propio, num_procesos_actual;

   // Inicializa las variables id_propio y num_procesos_actual
   MPI_Init( &argc, &argv );
   MPI_Comm_rank( MPI_COMM_WORLD, &id_propio );
   MPI_Comm_size( MPI_COMM_WORLD, &num_procesos_actual );

   //Simplemente muestra información en pantalla
      //presentacion();

   if (num_procesos_esperado == num_procesos_actual){
      if (id_propio < num_productores)
         funcion_productor(id_propio);
      else if (id_propio == num_productores)
         funcion_buffer();
      else
         funcion_consumidor(id_propio-num_productores-1);
   }
   else  if (id_propio == 0){    // solo el primero escribe error
            cerr << "el número de procesos esperados es:    " << num_procesos_esperado << endl
                 << "el número de procesos en ejecución es: " << num_procesos_actual << endl
                 << "(programa abortado)" << endl ;
      }

   // Al terminar el proceso, finalizar MPI
   MPI_Finalize( );
   return 0;
}
