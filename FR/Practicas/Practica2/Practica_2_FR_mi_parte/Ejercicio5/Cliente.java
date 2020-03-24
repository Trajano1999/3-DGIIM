import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.PrintWriter;
import java.net.Socket;
import java.net.UnknownHostException;
import java.util.Scanner;

public class Cliente{
	public static void main(String[] args){

		String buferEnvio;
		String buferRecepcion=new String();
		int bytesLeidos=0;
		
		// Nombre del host donde se ejecuta el servidor:
		String host="localhost";
		// Puerto en el que espera el servidor:
		int port=8989;

		// Socket para la conexión TCP
		Socket socket=null;
		
		boolean condicion;
		boolean fin = false;
		String opcion;
		Scanner s = new Scanner(System.in);

		try {
			// Creamos un socket que se conecte a "host" y "port":
			try {
				socket = new Socket(host, port);
			} catch (UnknownHostException e) {
				System.err.println("Error: Equipo desconocido");
			} catch (IOException e) {
				System.err.println("Error: No se pudo establecer la conexión");
			}
			
			InputStreamReader inputStream = new InputStreamReader(socket.getInputStream());
			OutputStream outputStream = socket.getOutputStream();

			BufferedReader inReader = new BufferedReader(inputStream);
			PrintWriter outPrinter = new PrintWriter(outputStream, true);
			
			do{
                condicion = false;

                //Mostramos el menú principal
                for( int i=0; i<6; i++){
                    buferRecepcion = inReader.readLine();
                    System.out.println(buferRecepcion);
                }

                //Recogemos y mandamos la respuesta
                opcion = s.nextLine();
                outPrinter.println(opcion);

                switch(opcion){
					case "1":
						//Mostramos los medicos
						for( int i=0; i<7; i++){
                            buferRecepcion = inReader.readLine();
                            System.out.println(buferRecepcion);
						}
						
						//Recogemos y mandamos la respuesta
                        opcion = s.nextLine();
                        outPrinter.println(opcion);
						String pelicula = opcion;
						
						switch(pelicula){
							case "1":
							case "2":
							case "3":
							case "4":
								//Mostramos los horarios
								for( int i=0; i<5; i++){
									buferRecepcion = inReader.readLine();
									System.out.println(buferRecepcion);
								}

								while(!condicion){
									//Recogemos y mandamos la respuesta
									opcion = s.nextLine();
									outPrinter.println(opcion);
									String horario = opcion;

								 	String res = inReader.readLine();
									if(res.equals("1"))
										condicion = true;
								
									buferRecepcion = inReader.readLine();
									System.out.println(buferRecepcion);
								}

								//Reserva
								buferRecepcion = inReader.readLine();
								System.out.println(buferRecepcion);

								//Recogemos y mandamos la respuesta
								opcion = s.nextLine();
								outPrinter.println(opcion);

								//Mostramos el resultado de la cita
								buferRecepcion = inReader.readLine();
								System.out.println(buferRecepcion);
							break;

							case "-1":
								buferRecepcion = inReader.readLine();
								System.out.println(buferRecepcion);
								System.out.flush();
							break;

							default:
								buferRecepcion = inReader.readLine();
								System.out.println(buferRecepcion);
								buferRecepcion = inReader.readLine();
								System.out.println(buferRecepcion);
								System.out.flush();
							break;
						}

					break;

					case "2":
						//Muestra todas las lineas hasta llegar a "FIN"
						buferRecepcion = inReader.readLine();
						while(!buferRecepcion.equals("FIN")){
							System.out.println(buferRecepcion);
							buferRecepcion = inReader.readLine();
						}
					break;

					case "3":
						//Muestra todas las lineas hasta llegar a "FIN"
						buferRecepcion = inReader.readLine();
						while(!buferRecepcion.equals("FIN")){
							System.out.println(buferRecepcion);
							buferRecepcion = inReader.readLine();
						}

						do{
							//Recogemos y mandamos la respuesta
							opcion = s.nextLine();
							outPrinter.println(opcion);

							if(Integer.parseInt(opcion) < 10 || Integer.parseInt(opcion) > 99){
								buferRecepcion = inReader.readLine();
								System.out.println(buferRecepcion);
							}
						}while(Integer.parseInt(opcion) < 10 || Integer.parseInt(opcion) > 99);
						
						//Muestra todas las lineas hasta llegar a "FIN"
						buferRecepcion = inReader.readLine();
						while(!buferRecepcion.equals("FIN")){
							System.out.println(buferRecepcion);
							buferRecepcion = inReader.readLine();
						}
					break;

					case "-1":
						// Leemos mensaje de despedida
                        buferRecepcion = inReader.readLine();
                        System.out.println(buferRecepcion);
						System.out.flush();
						
                        fin = true;
					break;

					default:
						//Leemos el mensaje de error
						buferRecepcion = inReader.readLine();
						System.out.println(buferRecepcion);
						buferRecepcion = inReader.readLine();
						System.out.println(buferRecepcion);
						System.out.flush();
					break;
                }
                
            }while(!fin);

			// Una vez terminado el servicio, cerramos el socket (automáticamente se cierran
			// el inpuStream  y el outputStream)
			socket.close();
	
			// Excepciones:
		} catch (UnknownHostException e) {
			System.err.println("Error: Nombre de host no encontrado.");
		} catch (IOException e) {
			System.err.println("Error de entrada/salida al abrir el socket.");
		}
	}
}
