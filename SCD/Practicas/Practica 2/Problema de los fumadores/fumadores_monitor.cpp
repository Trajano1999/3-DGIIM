#include <iostream>
#include <cassert>
#include <thread>
#include <mutex>
#include <random> // dispositivos, generadores y distribuciones aleatorias
#include <chrono> // duraciones (duration), unidades de tiempo
#include "HoareMonitor.h"

using namespace std;
using namespace HM;

//----------------------------------------------------------------------
//--------------------- VARIABLES GLOBALES -----------------------------
//----------------------------------------------------------------------

const int num_fumadores = 3;
mutex mtx_cout;

//------------------------------------------------------------------------------

template <int min, int max>
int aleatorio()
{
    static default_random_engine generador((random_device())());
    static uniform_int_distribution<int> distribucion_uniforme(min, max);
    return distribucion_uniforme(generador);
}

void fumar(int num_fumador)
{
    // calcular milisegundos aleatorios de duración de la acción de fumar)
    chrono::milliseconds duracion_fumar(aleatorio<20, 200>());

    // informa de que comienza a fumar
    mtx_cout.lock();
    cout << "Fumador " << num_fumador << "  :"
         << " empieza a fumar (" << duracion_fumar.count() << " milisegundos)" << endl;
    mtx_cout.unlock();
    // espera bloqueada un tiempo igual a ''duracion_fumar' milisegundos
    this_thread::sleep_for(duracion_fumar);

    // informa de que ha terminado de fumar
    mtx_cout.lock();
    cout << "Fumador " << num_fumador << "  : termina de fumar, comienza espera de ingrediente." << endl << endl;
    mtx_cout.unlock();
}

//------------------------------------------------------------------------------

class MonitorFumadores : public HoareMonitor
{
    private:
        int ingrediente;    // -1: vacio, 0: ingr 0, 1: ingr 1, 2: ingr 2
        CondVar fumador[num_fumadores],
                estanquero;
    public:  
        MonitorFumadores();
        void ponerIngrediente(int i);
        void retirarIngrediente(int i);
        void esperarRecogidaIngrediente();
};

MonitorFumadores::MonitorFumadores(){
    ingrediente = -1;
    for(int i=0; i<num_fumadores; ++i)
        fumador[i] = newCondVar();
    estanquero = newCondVar();
}

void MonitorFumadores::ponerIngrediente(int i){
    ingrediente = i;
    mtx_cout.lock();
        cout << "\tEl estanquero ha depositado el ingrediente: " << ingrediente << endl;
    mtx_cout.unlock();
    fumador[i].signal();
}

void MonitorFumadores::retirarIngrediente(int i){
    if(ingrediente != i)
        fumador[i].wait();
    
    mtx_cout.lock();
        cout << "El fumador " << i << " ha retirado el ingrediente: " << ingrediente << endl;
    mtx_cout.unlock();

    ingrediente = -1;
    estanquero.signal();
}

void MonitorFumadores::esperarRecogidaIngrediente(){
    if(ingrediente >= 0)
        estanquero.wait();
}

//------------------------------------------------------------------------------

void funcion_hebra_estanquero(MRef<MonitorFumadores> monitor)
{
    while(true){
        int ing = aleatorio<0,2>();
        monitor->ponerIngrediente(ing);
        monitor->esperarRecogidaIngrediente();
    }
}

void funcion_hebra_fumador(int num_fumador, MRef<MonitorFumadores> monitor)
{
    while (true){
        monitor->retirarIngrediente(num_fumador);
        fumar(num_fumador);
    }
}

//------------------------------------------------------------------------------

int main()
{
    MRef<MonitorFumadores> monitor = Create<MonitorFumadores>();

    thread hebra_estanquero(funcion_hebra_estanquero, monitor),
           hebra_fumador1(funcion_hebra_fumador, 0, monitor),
           hebra_fumador2(funcion_hebra_fumador, 1, monitor),
           hebra_fumador3(funcion_hebra_fumador, 2, monitor);
   
    hebra_estanquero.join();
    hebra_fumador1.join();
    hebra_fumador2.join();
    hebra_fumador3.join();
    return 0;
}