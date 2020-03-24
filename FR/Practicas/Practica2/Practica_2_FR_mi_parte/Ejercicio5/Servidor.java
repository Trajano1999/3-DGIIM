import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.ServerSocket;
import java.net.Socket;

public class Servidor {
	public static void main(String[] args) {
	
		// Puerto de escucha
		int port=8989;
		// array de bytes auxiliar para recibir o enviar datos.
		byte []buffer=new byte[256];
		// Número de bytes leídos
		int bytesLeidos=0;
		
		try {
			// Abrimos el socket en modo pasivo, escuchando en el puerto indicado
			ServerSocket serverSocket = null;
			try{
				serverSocket= new ServerSocket(port);
			} catch (IOException e){
				System.out.println("Error: no se pudo atender en el puerto "+port);
			}
			
			do {
				// Aceptamos una nueva conexión con accept()
				Socket socket;
                socket=serverSocket.accept();
				
				// Creamos un objeto de la clase Procesador, pasándole como 
				// argumento el nuevo socket, para que realice el procesamiento
				Procesador procesador=new Procesador(socket);
				procesador.procesa();
				
			} while(true);
			
		} catch (IOException e) {
			System.err.println("Error al escuchar en el puerto "+port);
		}
	}
}
