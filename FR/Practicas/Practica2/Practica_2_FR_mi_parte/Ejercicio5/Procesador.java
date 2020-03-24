import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.Socket;
import java.util.Random;
import java.io.PrintWriter;
import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.*;

public class Procesador {

	// Referencia a un socket para enviar/recibir las peticiones/respuestas
	private Socket socket;

	private BufferedReader inReader;
	private PrintWriter outPrinter;

    private String[][] medicos = new String [4][3];
    
	// Constructor que tiene como parámetro una referencia al socket abierto
	public Procesador(Socket socket) {
		this.socket=socket;
        
        //Inicializo la matriz de medicos
        for (int i=0; i<medicos.length; ++i)
            for (int j=0; j<medicos[i].length; ++j)
                medicos[i][j] = "";
	}
	
	void procesa(){
		String datosRecibidos=new String();
		String datosEnviar;
		String opcion;
		boolean fin=false;
		
		try {
			// Obtiene los flujos de escritura/lectura
			inReader = new BufferedReader(new InputStreamReader(socket.getInputStream()));
			outPrinter = new PrintWriter(socket.getOutputStream(), true);

			do{
				boolean condicion = false;

                datosEnviar = menuInicio();
				outPrinter.flush();
				
				//Mandamos el menu
                outPrinter.println(datosEnviar);
                outPrinter.flush();

                //Recibimos la respuesta del cliente
                datosRecibidos = inReader.readLine();
                opcion = new String(datosRecibidos);

                switch(opcion){
                         
                    case "1":
                        outPrinter.println(menuCitas());
                        outPrinter.flush();

                        datosRecibidos = inReader.readLine();
                        opcion = new String(datosRecibidos);
                        String medico = new String(opcion);
                        switch(medico){
                            
                            case "1":
                            case "2":   
                            case "3":
                            case "4":                             
                                outPrinter.println(menuHorarios());
                                outPrinter.flush();

                                String horario;

                                do{
                                    datosRecibidos = inReader.readLine();
                                    opcion = new String(datosRecibidos);
                                    horario = new String(opcion);

                                    condicion = seleccionarMedico(medico, horario);
                                    String res = new String();
                                    if (condicion == false){
                                        res = "0"; 
                                        datosEnviar = "Cita NO disponible, seleccione otra";
                                    }
                                    else{
                                        res = "1"; 
                                        datosEnviar = "Cita concertada con exito";
                                    }
                                    
                                    outPrinter.println(res);
                                    outPrinter.flush();
                                    outPrinter.println(datosEnviar);
                                    outPrinter.flush();

                                }while(!condicion);

                                outPrinter.println("Introduzca su nombre para concertar la cita: ");
                                outPrinter.flush();
                                        
                                datosRecibidos = inReader.readLine();
                                opcion = new String(datosRecibidos);
                                
                                int num_medico = Integer.parseInt(medico);     //Pasamos de String a entero
                                int num_horario = Integer.parseInt(horario);   //Pasamos de String a entero
                                medicos[num_medico-1][num_horario-1] = opcion;

                                outPrinter.println( opcion +
                                                    " tiene cita con el medico " + dimeElMedico(num_medico) +
                                                    " a las " + dimeElHorario(num_horario) + " horas" );
                                outPrinter.flush();
                                    
                            break;

                            case "-1":
                                datosEnviar = "...Volviendo al menu de Inicio...";
                                outPrinter.println(datosEnviar);
                                outPrinter.flush();
                            break;

                            default:
                                datosEnviar = mensajeError();
                                outPrinter.println(datosEnviar);
                                outPrinter.flush();
                            break;
                        }

                    break;

                    case "2":
                        datosEnviar = mostrarReservas();
                        outPrinter.println(datosEnviar);
                        outPrinter.flush();
                        outPrinter.println("FIN");
                        outPrinter.flush();
                    break;

                    case "3":
                        datosEnviar = mostrarReservas() +
                        "\nIndique la reserva que desea cancelar de la siguiente forma: " +
                        "medicohorario";
                        outPrinter.println(datosEnviar);
                        outPrinter.flush();
                        outPrinter.println("FIN");
                        outPrinter.flush();
                        
                        do{
                            datosRecibidos = inReader.readLine();
                            opcion = new String(datosRecibidos);

                            if(Integer.parseInt(opcion) < 10 || Integer.parseInt(opcion) > 99){
                                outPrinter.println(mensajeError2());
                                outPrinter.flush();
                            }
                        }while(Integer.parseInt(opcion) < 10 || Integer.parseInt(opcion) > 99);

                        int dato2 = Integer.parseInt(opcion) % 10; 
                        int dato1 = Integer.parseInt(opcion) / 10;

                        outPrinter.println(cancelarCita(dato1-1, dato2-1));
                        outPrinter.flush();
                        outPrinter.println("FIN");
                        outPrinter.flush();
                    break;

                    case "-1":
                        datosEnviar = "¡Que pase un buen dia!";
                        outPrinter.println(datosEnviar);
                        outPrinter.flush();
                                                            
                        fin = true;
                    break;

                    default:
                        datosEnviar = mensajeError();
                        outPrinter.println(datosEnviar);
                        outPrinter.flush();
                    break;
                }
                                    
			}while(!fin);

			socket.close();

		} catch (IOException e) {
			System.err.println("Error al obtener los flujos de entrada/salida.");
		}
	}

	private String menuInicio(){
		return "\n\tSERVICIO DE CITAS MEDICAS: " +
			   "\n\t (1) Concertar cita " + 
			   "\n\t (2) Consultar citas " +  
			   "\n\t (3) Cancelar cita " +
			   "\n\t(-1) Salir ";
	} 

	private String menuCitas(){
		return "\n\tCONCERTAR CITA: " +
			   "\n\t (1) Medicina General    : Dr. Ruiz Garcia " +
			   "\n\t (2) Traumatologia       : Dra. Lopez Benito " + 
			   "\n\t (3) Pediatria           : Dr. Alarcon Zurita " +
			   "\n\t (4) Otorrinolaringologia: Dr. Perez Casas " +
			   "\n\t(-1) Volver";
	}

    private String menuHorarios(){
        return "\n\tHORARIOS: " +
               "\n\t (1) 10:00 " +
               "\n\t (2) 12:30 " +
               "\n\t (3) 17:00 ";
    }

    private String mensajeError(){
        return "Ha introducido una opcion incorrecta ! " +
               "\n...Volviendo al menu de Inicio...";
    }

    private String mensajeError2(){
        return "Ha introducido una opcion incorrecta! ";
    }

    private boolean seleccionarMedico(String el_medico, String el_horario){
        boolean resultado;
        
        int n_medico = Integer.parseInt(el_medico);
        int n_horario = Integer.parseInt(el_horario);

        if(medicos[n_medico-1][n_horario-1].equals(""))
            resultado = true;
        else
            resultado = false;

        return resultado;
    }

    private String mostrarReservas(){
        String resultado=""; 
        
        for (int i=0; i<medicos.length; ++i)
            for (int j=0; j<medicos[i].length; ++j)
                if(!medicos[i][j].equals("")){
                    resultado = resultado + medicos[i][j] + 
                                " tiene cita con el medico " +
                                dimeElMedico(i+1) +
                                " a las " + 
                                dimeElHorario(j+1) + " horas" + "\n";
                }

        if(resultado.equals(""))
            resultado = "No hay ninguna consulta aun";

        return resultado;
    }

    private String dimeElMedico(int n_medico){      //nmedico = 1,2,3,4
        String resultado;
        
        if (n_medico == 1)
            resultado = "Dr. Ruiz Garcia";
        else if (n_medico == 2)
            resultado = "Dra. Lopez Benito";
        else if (n_medico == 3)
            resultado = "Dr. Alarcon Zurita";
        else if (n_medico == 4)
            resultado = "Dr. Perez Casas";
        else
            resultado = "";

        return resultado;
    }

    private String dimeElHorario(int n_horario){    //nhorario = 1,2,3
        String resultado;
        
        if (n_horario == 1)
            resultado = "10:00";
        else if (n_horario == 2)
            resultado = "12:30";
        else if (n_horario == 3)
            resultado = "17:00";
        else
            resultado = "";

        return resultado;
    }

    private String cancelarCita(int n_medico, int n_horario){
        String resultado;

        if(medicos[n_medico][n_horario].equals(""))
            resultado = "No hay ninguna reserva a las " +
                        dimeElHorario(n_horario) + " horas " + 
                        "con el medico " + dimeElMedico(n_medico);
        else{
            medicos[n_medico][n_horario] = "";
            resultado = "Se ha cancelado la reserva correctamente";
        }
        return resultado;
    }
}
