##TIPOS DE MENSAJES
Hay distintos tipos de mensajes en nuestro programa aunque debemos mencionar que los códigos **positivos** van a indicar el avance del programa mientras que los **negativos** van a permitir, o salir del programa, o volver a un menu anterior; según el menú en el que nos encontremos:

* OPCIONES DEL PRIMER MENÚ:
    -  1: Concertar cita. Se encarga de pedir unos datos como el medico y el horario que desea, y en caso permitido, reserva una cita.
    -  2: Consultar cita. Simplemente muestra los usuarios que han solicitado una cita.
    -  3: Cancelar cita. Se encarga de eliminar una cita fijada.
    - -1: Termina el programa.

Dependiendo de la opción solicitada, el usuario avanzará por un menú o por otro. En caso que escoja la opción 1, llegara al siguiente menú:

* OPCIONES DEL SEGUNDO MENÚ:
    -  1: Medicina General    : Dr. Ruiz Garcia
    -  2: Traumatologia       : Dra. Lopez Benito
    -  3: Pediatria           : Dr. Alarcon Zurita
    -  4: Otorrinolaringologia: Dr. Perez Casas
    - -1: Volver. Vuelve al menú anterior.

En caso de que escoja alguna de las opciones positivas, avanzará al último menú, en caso contrario, volverá al primero.

* OPCIONES DEL TERCER MENÚ:
    - 1: 10:00
    - 2: 12:30
    - 3: 17:00 

Aqui solo encontramos opciones positivas, esto es, solo puede seleccionar un menú.
Por tanto, el usuario solo puede salir del programa con la opción -1.
Finalmente, cabe mencionar que a la hora de cancelar una cita, hay que indicar el medico y el horario que se desea cancelar idnciando el identificador (número) de cada uno concatenados.