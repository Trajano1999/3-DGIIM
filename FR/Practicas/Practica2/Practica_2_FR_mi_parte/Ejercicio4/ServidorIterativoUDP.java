import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.ServerSocket;
import java.net.Socket;
import java.net.DatagramSocket;

//
// YodafyServidorIterativo
// (CC) jjramos, 2012
//
public class ServidorIterativoUDP{
	public static void main(String[] args) {
	
		// Puerto de escucha
		int port = 8989;
		DatagramSocket socketServidor = null;
		// array de bytes auxiliar para recibir o enviar datos.
		//byte []buffer=new byte[256];
		// Número de bytes leídos
		//int bytesLeidos=0;

		do {
			try {
				socketServidor = new DatagramSocket(port);
			} catch (IOException e) {
				System.out.println("Error: no se pudo atender en el puerto " + port);
			}

			// Creamos un objeto de la clase ProcesadorYodafyUDP, pasándole como 
			// argumento el nuevo socket, para que realice el procesamiento
			// Este esquema permite que se puedan usar hebras más fácilmente.
			ProcesadorYodafyUDP procesador = new ProcesadorYodafyUDP(socketServidor);
			procesador.procesa();
			
		} while (true);
	}
}