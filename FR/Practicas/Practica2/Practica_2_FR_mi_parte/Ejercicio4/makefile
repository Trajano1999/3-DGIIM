Procesador=ProcesadorYodafyUDP
Servidor=ServidorIterativoUDP
Cliente=ClienteUDP

servidor:
	javac $(Servidor).java $(Cliente).java $(Procesador).java
	java $(Servidor)
	
cliente: 
	java $(Cliente)

clean:
	rm -f *.class
