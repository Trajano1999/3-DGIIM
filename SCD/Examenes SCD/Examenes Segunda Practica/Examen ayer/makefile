.SUFFIXES:
.PHONY: pc
.PRECIOUS: %_exe

compilador:=g++

pc_fifo: prodcons_fifo_exe
	./$<
	
pc_lifo: prodcons_lifo_exe
	./$<

pc_fifo_varios: prodcons_fifo_varios_exe
	./$<
	
pc_lifo_varios: prodcons_lifo_varios_exe
	./$<

fu: fumadores_exe
	./$<

fu_monitor:
	g++ -std=c++11 -pthread -o fu_monitor_exe fumadores_monitor.cpp HoareMonitor.cpp Semaphore.cpp
	./fu_monitor_exe

barbero:
	g++ -std=c++11 -pthread -o barbero_monitor_exe Barbero_durmiente.cpp HoareMonitor.cpp Semaphore.cpp
	./barbero_monitor_exe	

barberia2:
	g++ -std=c++11 -pthread -o barberia2_exe barberia2.cpp HoareMonitor.cpp Semaphore.cpp
	./barberia2_exe	

g_barberos: 
	g++ -std=c++11 -pthread -o g_barberos_exe G_Barberos.cpp HoareMonitor.cpp Semaphore.cpp
	./g_barberos_exe

g_barberos_singular:
	g++ -std=c++11 -pthread -o g_barberos_singular_exe G_Barberos_Singular.cpp HoareMonitor.cpp Semaphore.cpp
	./g_barberos_singular_exe

g_barberos_puerta:
	g++ -std=c++11 -pthread -o g_barberos_puerta_exe G_Barberos_Puertas.cpp HoareMonitor.cpp Semaphore.cpp
	./g_barberos_puerta_exe

g_barberos_puerta_maximo:
	g++ -std=c++11 -pthread -o g_barberos_puerta_maximo_exe G_Barberos_Puertas_Maximo.cpp HoareMonitor.cpp Semaphore.cpp
	./g_barberos_puerta_maximo_exe

%_exe: %.cpp
	$(compilador) -std=c++11 -pthread -o $@ $< Semaphore.cpp 

clean:
	rm -f *_exe
