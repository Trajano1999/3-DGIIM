/*1.-Modificar la solución al problema del barbero durmiente, tal como se muestra a continuación,
guardando la solución en un archivo denominado barberia2.cpp. 
El comportamiento es idéntico al problema original, salvo en dos aspectos:
    
    a) En este caso, existen 2 tipos de clientes, tipo 0 y tipo 1, existiendo sillas específicas 
    en la sala de espera para los clientes de cada tipo concreto. Se puede asumir que existen
    3 clientes de cada tipo.

    b) el barbero no puede cortar de forma consecutiva a dos clientes del mismo tipo por lo que 
    debe ir cortando el pelo de forma alternada a los clientes de cada tipo.

    c) Cuando el barbero termina con un cliente de un tipo, comprueba si hay hay algún cliente 
    del otro tipo y, si no lo hay, se echa a dormir hasta que llegue uno del tipo apropiado.

    d) Si llega un cliente que no es demandado en ese instante por el barbero, debe entrar en la 
    sala de espera, aunque el barbero esté durmiendo. Al abrir la barbería, el barbero empieza 
    siempre pelando a un cliente del tipo 0, después a uno del tipo 1 y así sucesivamente.

    e) Una vez despertado, el barbero debe pelar al cliente que le despertó que ya estará sentado
    o estará a punto de sentarse en la silla de pelar.
*/

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

const int num_clientes_A = 1,
          num_clientes_B = 1;
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

void esperarFueraBarberia(int num_cliente, int id)
{
    chrono::milliseconds duracion(aleatorio<30,1000>());

    mtx_cout.lock();
    cout << "El cliente " << num_cliente
         << " del tipo " << id
         << " va a esperar fuera de la barberia " << duracion.count() 
         << " milisegundos" << endl; 
    mtx_cout.unlock();

    this_thread::sleep_for(duracion);

    mtx_cout.lock();
    cout << "El cliente " << num_cliente << " del tipo " << id << " entra a la barberia" << endl;
    mtx_cout.unlock();
}

//------------------------------------------------------------------------------

class Barberia : public HoareMonitor
{
    private:
        CondVar barbero,
                sala_espera_A,
                sala_espera_B,
                silla;
        int ultimo_pelado;
    public:
        Barberia();
        void cortarPelo(int num_cliente, int id);
        void siguienteCliente();
        void finCliente();
};

Barberia::Barberia()
{
    ultimo_pelado = 1;
    barbero = newCondVar();
    sala_espera_A = newCondVar();
    sala_espera_B = newCondVar();
    silla = newCondVar();
}

void Barberia::cortarPelo(int num_cliente, int id)
{   
    if(!barbero.empty() && id != ultimo_pelado){    //si esta vacio es que está trabajando
        mtx_cout.lock();
        cout << "El cliente " << num_cliente << " del tipo " << id << " despierta al barbero" << endl;
        mtx_cout.unlock();

        barbero.signal();
    }else{
        mtx_cout.lock();
        cout << "El cliente " << num_cliente << " del tipo " << id << " se sienta en la sala de espera" << endl;
        mtx_cout.unlock();

        if(id == 0)
            sala_espera_A.wait();
        else
            sala_espera_B.wait();       
    }
    
    mtx_cout.lock();
    cout << "El cliente " << num_cliente << " del tipo " << id << " se sienta en la silla" << endl;
    mtx_cout.unlock();
    silla.wait();
}

void Barberia::siguienteCliente()
{   
    if(ultimo_pelado == 1){
        if(sala_espera_A.empty() && silla.empty()){
            mtx_cout.lock();
            cout << "No hay clientes 0 esperando, el barbero se va a dormir " << endl << flush;
            mtx_cout.unlock();

            barbero.wait();
        }else if(silla.empty())
            sala_espera_A.signal();
    }else{
        if(sala_espera_B.empty() && silla.empty()){
            mtx_cout.lock();
            cout << "No hay clientes 1 esperando, el barbero se va a dormir " << endl << flush;
            mtx_cout.unlock();

            barbero.wait();
        }else if(silla.empty())
            sala_espera_B.signal();
    }
}

void Barberia::finCliente()
{
    ultimo_pelado = (ultimo_pelado+1)%2;

    mtx_cout.lock();
    cout << "\tUn cliente del tipo " << ultimo_pelado << " sale de la barberia" << endl << flush;
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

void funcion_cliente(int num_cliente, MRef<Barberia> monitor, int id)
{
    while(true){
        monitor->cortarPelo(num_cliente, id);
        esperarFueraBarberia(num_cliente, id);
    }
}

//------------------------------------------------------------------------------

int main()
{
    cout << "-------------------------------------------------------------------------------" << endl
         << "-------------------- Problema de la Barberia: Monitor SU ----------------------" << endl
         << "-------------------------------------------------------------------------------" << endl
         << "Nº clientes de tipo A: " << num_clientes_A << endl
         << "Nº clientes de tipo B: " << num_clientes_B << endl
         << flush << endl;

    MRef<Barberia> monitor = Create<Barberia>();

    thread hebra_barbero(funcion_barbero, monitor),
           hebra_clientes_A[num_clientes_A],
           hebra_clientes_B[num_clientes_B];
    
    for(int i=0; i<num_clientes_A; ++i)
        hebra_clientes_A[i] = thread(funcion_cliente, i, monitor, 0);   // El ultimo parámetro es un identificador
    for(int i=0; i<num_clientes_B; ++i)
        hebra_clientes_B[i] = thread(funcion_cliente, i, monitor, 1);   // El ultimo parámetro es un identificador
    
    hebra_barbero.join();
    for(int i=0; i<num_clientes_A; ++i)
        hebra_clientes_A[i].join();
    for(int i=0; i<num_clientes_B; ++i)
        hebra_clientes_B[i].join();

    return 0;
}