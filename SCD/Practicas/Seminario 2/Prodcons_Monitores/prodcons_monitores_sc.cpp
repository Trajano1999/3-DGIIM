#include <iostream>
#include <iomanip>
#include <cassert>
#include <random>

#include <thread>
#include <mutex>
#include <condition_variable>

using namespace std;

//----------------------------------------------------------------------
//--------------------- VARIABLES GLOBALES -----------------------------
//----------------------------------------------------------------------

const int num_productores = 4,       // num_items % num_productores == 0
          num_consumidores = 4,      // num_items % num_consumidores == 0
          num_items = 40,
          tam_vector = 10;

mutex mtx;

unsigned cont_prod[num_items] = {0}, // contadores de verificación: producidos
         cont_cons[num_items] = {0}, // contadores de verificación: consumidos
         cont_nprod[num_productores] = {0};

//----------------------------------------------------------------------

template< int min, int max > int aleatorio()
{
  static default_random_engine generador((random_device())());
  static uniform_int_distribution<int> distribucion_uniforme( min, max ) ;
  return distribucion_uniforme( generador );
}

int producir_dato (int ih){
   this_thread::sleep_for(chrono::milliseconds(aleatorio<20,100>()));

   mtx.lock();
   cout << "producido: " << ih*num_items/num_productores + cont_nprod[ih] << endl << flush;
   mtx.unlock();

   cont_prod[ih*num_items/num_productores + cont_nprod[ih]]++;
   return ih*num_items/num_productores + cont_nprod[ih]++;
}

void consumir_dato (unsigned dato){
   if (num_items <= dato){
      cout << " dato === " << dato << ", num_items == " << num_items << endl ;
      assert(dato < num_items);
   }

   cont_cons[dato]++;

   this_thread::sleep_for( chrono::milliseconds( aleatorio<20,100>() ));

   mtx.lock();
   cout << "                  consumido: " << dato << endl;
   mtx.unlock();
}


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

class MonitorProdCons
{
    private:
        int datos[tam_vector],
            indice;

        condition_variable prod,
                           cons;
        mutex cerrojo_mon;
    public:
        MonitorProdCons();
        void escribir(int d);
        int leer();
};

MonitorProdCons::MonitorProdCons()
{
    indice = 0;
}

void MonitorProdCons::escribir(int d)
{
    unique_lock<mutex> guardia(cerrojo_mon);
    while(indice >= tam_vector)                    
        prod.wait(guardia);

    datos[indice] = d;
    indice++;

    cons.notify_one();
}

int MonitorProdCons::leer()
{
    unique_lock<mutex> guardia(cerrojo_mon);
    while(indice <= 0)                             
        cons.wait(guardia);

    assert(indice > 0);
    indice--;
    int d = datos[indice];
    
    prod.notify_one();

    return d;
}

//----------------------------------------------------------------------

void funcion_productores(MonitorProdCons * monitor, int ih)
{
    for(int i=0; i<num_items/num_productores; ++i){
        int dato = producir_dato(ih);
        monitor->escribir(dato);
    }
}

void funcion_consumidores(MonitorProdCons * monitor)
{
    for(int i=0; i<num_items/num_consumidores; ++i){
        int dato = monitor->leer();
        consumir_dato(dato);
    }
}

//----------------------------------------------------------------------

int main()
{
    cout << "--------------------------------------------------------" << endl
         << "Problema de los productores-consumidores con monitor SC:" << endl
         << "Numero productores : " << num_productores << endl
         << "Numero consumidores: " << num_consumidores << endl
         << "Numero elementos   : " << num_items << endl
         << "Tamaño vector      : " << tam_vector << endl
         << "--------------------------------------------------------" << endl
         << flush ;

    MonitorProdCons monitor;
    thread productores[num_productores],
           consumidores[num_consumidores];

    for(int i=0; i<num_productores; ++i)
        productores[i] = thread(funcion_productores, &monitor, i);
    for(int i=0; i<num_consumidores; ++i)
        consumidores[i] = thread(funcion_consumidores, &monitor);

    for(int i=0; i<num_productores; ++i)
        productores[i].join();
    for(int i=0; i<num_consumidores; ++i)
        consumidores[i].join();

    // comprobar que cada item se ha producido y consumido exactamente una vez
    test_contadores();

    return 0;
}