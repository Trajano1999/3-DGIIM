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

%_exe: %.cpp
	$(compilador) -std=c++11 -pthread -o $@ $< Semaphore.cpp 

clean:
	rm -f *_exe
