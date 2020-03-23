.SUFFIXES:
.PHONY: x1, x2, clean

# Para compilar y ejecutar productores y consumidores con un monitor SC usar `xsc`
# Para compilar y ejecutar productores y consumidores con un monitor SU usar `xsu`

compilador:=g++
opcionesc:= -std=c++11 -pthread -Wfatal-errors
hmonsrcs:= HoareMonitor.h HoareMonitor.cpp Semaphore.h Semaphore.cpp

x1: monitor_em_exe
	./$<

x2: barrera1_sc_exe
	./$<

x3: barrera2_sc_exe
	./$<

x4: barrera2_su_exe
	./$<

x5: prodcons1_sc_exe
	./$<

x6: prodcons2_sc_exe
	./$<

x7: prodcons2_su_exe
	./$<

x8: fumadores_exe
	./$<

x9: prodconsM_su_exe
	./$<

x10: prodconsM_sc_exe
	./$<

xsc: pc_monitores_sc_exe
	./$<

xsu: pc_monitores_su_exe
	./$<

monitor_em_exe: monitor_em.cpp
	$(compilador) $(opcionesc)  -o $@ $<

barrera1_sc_exe: barrera1_sc.cpp
	$(compilador) $(opcionesc)  -o $@ $<

barrera2_sc_exe: barrera2_sc.cpp
	$(compilador) $(opcionesc)  -o $@ $<

barrera2_su_exe: barrera2_su.cpp $(hmonsrcs)
	$(compilador) $(opcionesc)  -o $@ $< HoareMonitor.cpp Semaphore.cpp

prodcons1_sc_exe: prodcons1_sc.cpp $(hmonsrcs)
	$(compilador) $(opcionesc)  -o $@ $<

prodcons2_sc_exe: prodcons2_sc.cpp $(hmonsrcs)
	$(compilador) $(opcionesc)  -o $@ $<

prodcons2_su_exe: prodcons2_su.cpp $(hmonsrcs)
	$(compilador) $(opcionesc)  -o $@ $<  HoareMonitor.cpp Semaphore.cpp

fumadores_exe: fumadores.cpp $(hmonsrcs)
	$(compilador) $(opcionesc)  -o $@ $<  HoareMonitor.cpp Semaphore.cpp

pc_monitores_sc_exe: prodcons_monitores_sc.cpp $(hmonsrcs)
	$(compilador) $(opcionesc)  -o $@ $<  HoareMonitor.cpp Semaphore.cpp

pc_monitores_su_exe: prodcons_monitores_su.cpp $(hmonsrcs)
	$(compilador) $(opcionesc)  -o $@ $<  HoareMonitor.cpp Semaphore.cpp

clean:
	rm -f *_exe
