.SUFFIXES:
.PHONY:    pc,pc2,pcm,fp,f,fi,fc,clean

compilador := mpicxx -std=c++11
flagsc     := -g -Wall

pc-plantilla: prodcons-plantilla_exe
	mpirun -np  3 ./$<

pc2-plantilla: prodcons2-plantilla_exe
	mpirun -np  3 ./$<

pc2: prodcons2_exe
	mpirun -np  5 ./$<

pcm: prodcons2-mu_exe
	mpirun -np  10 ./$<

fi-plantilla: filosofos-plantilla_exe
	mpirun -np 10 ./$<

fi: filosofos_exe
	mpirun -np 10 ./$<

fi_interbloqueo: filosofos_interbloqueo_exe
	mpirun -np 10 ./$<

fi_camarero: filosofos_camarero_exe
	mpirun -np 11 ./$<

filo_cam_ex: filosofos_camarero_5tenedores_exe
	mpirun -np 7 ./$<

ex_ayer: examen_ayer_exe
	mpirun -np 11 ./$<

pc_p: prodcons2_prioridad
	mpirun -np 11 ./$<

f_c_p: filosofos-cam-prioridad_exe
	mpirun -np 11 ./$<

fi_gordo: filosofo_gordo_exe
	mpirun -np 11 ./$<

f: filosofos_exe
	mpirun -np 10 ./$<

fc: filosofos-cam_exe
	mpirun -np 11 ./$<

%_exe: %.cpp
	$(compilador) $(flagsc) -o $@ $<

clean:
	rm -rf *_exe *.dSYM
