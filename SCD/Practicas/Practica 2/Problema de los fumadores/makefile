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

%_exe: %.cpp
	$(compilador) -std=c++11 -pthread -o $@ $< Semaphore.cpp 

clean:
	rm -f *_exe
