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

const int num_clientes = 6,
          num_barberos = 2;   //Coincide con el numero de sillas y el numero de camas

mutex mtx_cout;

//------------------------------------------------------------------------------

template <int min, int max>
int aleatorio()
{
    static default_random_engine generador((random_device())());
    static uniform_int_distribution<int> distribucion_uniforme(min, max);
    return distribucion_uniforme(generador);
}

void cortarPeloACliente(int num_barbero)
{
    chrono::milliseconds duracion(aleatorio<30,300>());

    mtx_cout.lock();
    cout << "El barbero " << num_barbero
         << " va a cortar el pelo... (" 
         << duracion.count() 
         << " milisegundos)" << endl; 
    mtx_cout.unlock();

    this_thread::sleep_for(duracion);

    mtx_cout.lock();
    cout << "El barbero " << num_barbero << " ha terminado de cortar el pelo" << endl;
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
        void siguienteCliente(int num_barbero);
        void finCliente(int num_barbero);
};

Barberia::Barberia()
{
    barbero = newCondVar();
    sala_espera = newCondVar();
    silla = newCondVar();
}

void Barberia::cortarPelo(int num_cliente)
{
    if(sala_espera.empty() && barbero.get_nwt() > 0){
        mtx_cout.lock();
        cout << "El cliente " << num_cliente << " despierta al barbero " << endl;
        mtx_cout.unlock();

        barbero.signal();
    }else{
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

void Barberia::siguienteCliente(int num_barbero)
{
    if(silla.get_nwt() < num_barberos){
        if(sala_espera.empty()){
            mtx_cout.lock();
            cout << "No hay clientes esperando, el barbero " << num_barbero << " se va a dormir " << endl << flush;
            mtx_cout.unlock();

            barbero.wait();
        }else{
            mtx_cout.lock();
            cout << "Barbero " << num_barbero << ": llama al siguiente cliente\n";
            mtx_cout.unlock();

            sala_espera.signal();
        }
    }
}

void Barberia::finCliente(int num_barbero)
{
    mtx_cout.lock();
    cout << "\tEl barbero " << num_barbero << " ha terminado con el cliente" << endl;
    cout << "\tEl cliente sale de la barberia" << endl << flush;
    mtx_cout.unlock();

    silla.signal();
}

//------------------------------------------------------------------------------

void funcion_barbero(int num_barbero, MRef<Barberia> monitor)
{
    while(true){
        monitor->siguienteCliente(num_barbero);
        cortarPeloACliente(num_barbero);
        monitor->finCliente(num_barbero);
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
         << "------------ Singularización del problema del barbero: Monitor SU -------------" << endl
         << "-------------------------------------------------------------------------------" << endl
         << "Nº barberos : " << num_barberos << endl
         << "Nº clientes : " << num_clientes << endl
         << endl << flush;

    MRef<Barberia> monitor = Create<Barberia>();

    thread hebra_barbero[num_barberos],
           hebra_clientes[num_clientes];
    
    for(int i=0; i<num_barberos; ++i)
        hebra_barbero[i] = thread(funcion_barbero, i, monitor);

    for(int i=0; i<num_clientes; ++i)
        hebra_clientes[i] = thread(funcion_cliente, i, monitor);
    
    for(int i=0; i<num_barberos; ++i)
        hebra_barbero[i].join();
    
    for(int i=0; i<num_clientes; ++i)
        hebra_clientes[i].join();

    return 0;
}