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

const int num_clientes = 6;
mutex mtx_cout;

//------------------------------------------------------------------------------

template <int min, int max>
int aleatorio()
{
    static default_random_engine generador((random_device())());
    static uniform_int_distribution<int> distribucion_uniforme(min, max);
    return distribucion_uniforme(generador);
}

void cortarPeloACliente()
{
    chrono::milliseconds duracion(aleatorio<30,300>());

    mtx_cout.lock();
    cout << "El barbero va a cortar el pelo... (" 
         << duracion.count() 
         << " milisegundos)" << endl; 
    mtx_cout.unlock();

    this_thread::sleep_for(duracion);

    mtx_cout.lock();
    cout << "El barbero ha terminado de cortar el pelo" << endl;
    mtx_cout.unlock();
}

void esperarFueraBarberia(int num_cliente)
{
    chrono::milliseconds duracion(aleatorio<30,1000>());

    mtx_cout.lock();
    cout << "El cliente " << num_cliente
         << " va a esperar fuera de la barberia " << duracion.count() 
         << " milisegundos" << endl; 
    mtx_cout.unlock();

    this_thread::sleep_for(duracion);

    mtx_cout.lock();
    cout << "El cliente " << num_cliente << " entra a la barberia" << endl;
    mtx_cout.unlock();
}

//------------------------------------------------------------------------------

class Barberia : public HoareMonitor
{
    private:
        CondVar barbero,
                sala_espera,
                silla;
    public:
        Barberia();
        void cortarPelo(int num_cliente);
        void siguienteCliente();
        void finCliente();
};

Barberia::Barberia()
{
    barbero = newCondVar();
    sala_espera = newCondVar();
    silla = newCondVar();
}

void Barberia::cortarPelo(int num_cliente)
{   
    if(!barbero.empty()){    //si esta vacio es que está trabajando
        mtx_cout.lock();
        cout << "El cliente " << num_cliente << " despierta al barbero" << endl;
        mtx_cout.unlock();

        barbero.signal();
    }else{
                            // if( !sillas.empty() or !sillon.empty() )
        mtx_cout.lock();
        cout << "El cliente " << num_cliente << " se sienta en la sala de espera" << endl;
        mtx_cout.unlock();

        sala_espera.wait();
    }
    
    mtx_cout.lock();
    cout << "El cliente " << num_cliente << " se sienta en la silla" << endl;
    mtx_cout.unlock();
    silla.wait();
}

void Barberia::siguienteCliente()
{
    if(sala_espera.empty() && silla.empty()){
        mtx_cout.lock();
        cout << "No hay clientes esperando, el barbero se va a dormir " << endl << flush;
        mtx_cout.unlock();

        barbero.wait();
    }else if(silla.empty())
        sala_espera.signal();
}

void Barberia::finCliente()
{
    mtx_cout.lock();
    cout << "\tEl cliente sale de la barberia" << endl << flush;
    mtx_cout.unlock();

    silla.signal();
}

//------------------------------------------------------------------------------

void funcion_barbero(MRef<Barberia> monitor)
{
    while(true){
        monitor->siguienteCliente();
        cortarPeloACliente();
        monitor->finCliente();
    }
}

void funcion_cliente(int num_cliente, MRef<Barberia> monitor)
{
    while(true){
        monitor->cortarPelo(num_cliente);
        esperarFueraBarberia(num_cliente);
    }
}

//------------------------------------------------------------------------------

int main()
{
    cout << "-------------------------------------------------------------------------------" << endl
         << "-------------------- Problema de la Barberia: Monitor SU ----------------------" << endl
         << "-------------------------------------------------------------------------------" << endl
         << flush;

    MRef<Barberia> monitor = Create<Barberia>();

    thread hebra_barbero(funcion_barbero, monitor),
           hebra_clientes[num_clientes];
    
    for(int i=0; i<num_clientes; ++i)
        hebra_clientes[i] = thread(funcion_cliente, i, monitor);
    
    hebra_barbero.join();
    for(int i=0; i<num_clientes; ++i)
        hebra_clientes[i].join();

    return 0;
}