#include <iostream>
#include <cassert>
#include <thread>
#include <mutex>
#include <random> // dispositivos, generadores y distribuciones aleatorias
#include <chrono> // duraciones (duration), unidades de tiempo
#include "HoareMonitor.h"
#include <vector>
#include <condition_variable>
#include <future>
#include <iomanip>


using namespace std ;
using namespace HM ;

const int num_clientes = 3;
mutex mtx;


template< int min, int max > int aleatorio()
{
  static default_random_engine generador( (random_device())() );
  static uniform_int_distribution<int> distribucion_uniforme( min, max ) ;
  return distribucion_uniforme( generador );
}

void pelarCliente(int id_cliente, int id_barbero){

   // calcular milisegundos aleatorios de duración de la acción de cortar el pelo)
   chrono::milliseconds duracion_cortar( aleatorio<20,200>() );

   // informa de que comienza a cortarle el pelo
   
   cout << "El barbero " << id_barbero << " está cortando el pelo  al cliente "<< id_cliente << " (" << duracion_cortar.count() << " milisegundos)" << endl;
   
   // espera bloqueada un tiempo igual a ''duracion_cortar' milisegundos
   this_thread::sleep_for( duracion_cortar );

   // informa de que ha terminado de cortarle el pelo
   cout << "El barbero " << id_barbero << " termina de pelar al cliente "<< id_cliente  << endl;
   
}

void esperarFuera(int num_cliente){
    chrono::milliseconds duracion_esperar( aleatorio<1000,3000>() );
   
    cout << "El Cliente " << num_cliente << ": espera fuera de la barberia (" << duracion_esperar.count() << " milisegundos)" << endl;
  
    this_thread::sleep_for( duracion_esperar );
    cout << "Cliente " << num_cliente << ": entra a la barberia." << endl;
  
}



class Barberia : public HoareMonitor
{
	private:

		CondVar cama;
		CondVar sillas;
    CondVar sillon[2];
		CondVar esperarSillon[2];
    int clientes[2];
       

	public:

	Barberia();

	void cortarPelo(int id_cliente);
    int siguienteCliente(int id_barbero); //devuelve el identificador del cliente
    void finCliente(int id_barbero);
	
};

Barberia::Barberia(){
	
	cama = newCondVar();
	sillas = newCondVar();
  sillon[0] = newCondVar();
  sillon[1] = newCondVar();
	esperarSillon[0] = newCondVar();
	esperarSillon[1] = newCondVar();

}


void Barberia::cortarPelo(int i){
    cout << "El Cliente " << i << " quiere cortarse el pelo" << endl;
	    
    if ( !cama.empty() ){ // si barbero NO ESTA VACIO -->empty es falso es que está cortando el pelo
        
				//cout <<" Un barbero se despierta"  << endl;
        cama.signal();

    } else if( !sillas.empty() or ( !sillon[0].empty()  and !sillon[1].empty() ) ){
        cout << "El cliente " << i << " se sienta a esperar en la sala de espera. " << endl << flush;
        sillas.wait();
    } 
    
    if( sillon[0].empty() ){
        cout << "El cliente "<< i << " se sienta en el sillon 0 " << endl << flush;
        clientes[0]=i;
				esperarSillon[0].signal();
        sillon[0].wait();
       
    } else if (sillon[1].empty() ){
        cout << "El cliente "<< i << " se sienta en el sillon 1 " << endl << flush;
				clientes[1]=i;
				esperarSillon[1].signal(); 
				sillon[1].wait();
    }
  
}

int Barberia::siguienteCliente(int id_barbero){
     
	if (sillas.empty() && sillon[id_barbero].empty() ){
        cout << "No hay clientes esperando, el barbero "<< id_barbero << " se va a dormir " << endl << flush;
        cama.wait();
				cout << " Se despierta el barbero " << id_barbero << endl;
  } else if (sillon[id_barbero].empty()){
       sillas.signal();
	}
   
  if(sillon[id_barbero].empty())
		esperarSillon[id_barbero].wait();

    return clientes[id_barbero];
      
}

void Barberia::finCliente(int id_barbero){
	cout << "El cliente " << clientes[id_barbero] << " sale de la barberia " << endl << flush;
    sillon[id_barbero].signal();    
}

void funcion_hebra_cliente(int i, MRef<Barberia> monitor){
    while(true){
        monitor->cortarPelo(i);
        esperarFuera(i);
    }
}


void funcion_hebra_barbero(MRef<Barberia> monitor, int id_barbero){
	while(true){
		int id_cliente = monitor->siguienteCliente(id_barbero);
        pelarCliente(id_cliente, id_barbero);
		monitor->finCliente(id_barbero);
	}
}





int main(){
   cout << "-------------------------------------------------------------------------------" << endl
        << "                    Problema de la Barberia  : Monitor SU "                      << endl
        << "-------------------------------------------------------------------------------" << endl
        << flush ;

    MRef<Barberia> monitor = Create<Barberia>();

   thread hebra_barbero0(funcion_hebra_barbero, monitor, 0);
   thread hebra_barbero1(funcion_hebra_barbero, monitor, 1);

   
   thread hebra_cliente0(funcion_hebra_cliente, 0, monitor),
	  hebra_cliente1(funcion_hebra_cliente, 1, monitor),
	  hebra_cliente2(funcion_hebra_cliente, 2, monitor);
		

    hebra_barbero0.join();
    hebra_barbero1.join();
    hebra_cliente0.join();
    hebra_cliente1.join();
    hebra_cliente2.join();
}
