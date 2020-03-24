##ALMACENAMIENTO EN LA NUBE
```
- Introducción
- Historia
- Arquitectura
- Ventajas
- Precauciones
- Mejores servicios / Ejemplos
- Bibliografia
```

####*Indice*: 
Marta

####*Introducción*
Marta
... las empresas disponen de 3 modelos principales...

####*Historia*

Se considera que el concepto se forjó en la década de los años 1960 por **Joseph Carl Robnett Licklider**, quien fue, un informático estadounidense, considerado una de las figuras más importantes en ciencia computacional y de historia de la informática. 
Se recuerda particularmente por ser unos de los primeros en investigar sobre la **computación interactiva moderna**.
Fue pionero de Internet, con una visión temprana de una red de ordenadores mundial mucho antes de que fuera construida. Hizo lo posible para hacerlo realidad mediante su financiación para la investigación, incluyendo:
    -> La **interfaz gráfica de usuario** o mas conocida como **GUI**, utiliza un conjunto de imágenes y objetos gráficos para representar la información y acciones disponibles en la interfaz. Su principal uso, consiste en proporcionar un entorno visual sencillo para permitir la comunicación con el sistema operativo.
    -> **ARPANET**, que fue una red de computadoras creada por encargo del Departamento de Defensa de los Estados Unidos para utilizarla como medio de comunicación entre las diferentes instituciones académicas y estatales.

Los bajos costes de almacenamiento y procesamiento no se extendieron hasta finales de los años 1990, lo cual retrasó la implementación y el desarrollo masivo de las soluciones basadas en computación en nube. Por ello las implementaciones recientes se deben a la **Web 2.0** (término que comprende aquellos sitios web que facilitan el compartir información, la interoperabilidad, el diseño centrado en el usuario y la colaboración en la World Wide Web).

En 1999, llegó uno de los logros tempranos de la computación en nube, con **"salesforce.com"** pionera en la entrega de aplicaciones corporativas por medio de una interfaz web. Esta compañía demostró a los especialistas y compañías de desarrollo de software, la ventaja del uso de portales web para entrega de sus productos. **FilesAnywhere** también colaboró en el esquema de servicios basados en almacenamiento en nube, que permitió a los usuarios compartir sus archivos de forma segura por Internet.

####*Arquitectura*

Envidiando el trabajo de mi compañera, yo también quiero definir el **almacenamiento en la nube** pero desde un punto de vista más enfocado a la arquitectura. Lo definimos como un entorno de almacenamiento compuesto por muchos **recursos distribuidos**,pero actúa como uno solo con gran tolerancia a fallos porque implementa redundancia y distribución de datos, que posibilita la recuperación de la información, mejorando la consistencia de datos.

Hablando de la arquitectura, esta debe ofrecer:
    -**Disponibilidad**: Cuando los usuarios prueban un servicio en la nube, normalmente suelen hacerlo como alternativa a que una aplicación se ejecute directamente en su equipo de escritorio. Si un servicio basado en la nube no proporciona una experiencia en cuanto a **fiabilidad** y **velocidad** comparable con la de una aplicación en el dispositivo, los usuarios no lo utilizarán.
    -**Seguridad** sin incertidumbres. Un servicio en la nube no prosperará a menos que los clientes confíen en que sus datos confidenciales estarán seguros en el servicio. 

Las **herramientas de software** son también muy importantes a la hora de crear servicios y soluciones. Entre ellas se incluyen las API, software intermedio, base de datos, aplicaciones e infraestructuras de escritorios virtuales (VDI), junto con la aplicación stackware en la nube y herramientas de gestión asociadas.

####*Ventajas*
Marta

...el almacenamiento en la nube se puede asignar como una unidad local con
el protocolo WebDAV...

####*Precauciones*

Cuando los datos se han distribuido, se almacenan en más ubicaciones, lo que aumenta el riesgo de **acceso físico no autorizado** a los datos. Por ejemplo, en la arquitectura basada en la nube, los datos se replican y se mueven con frecuencia, por lo que el riesgo de recuperación de datos no autorizados aumenta. 
El número de personas con acceso a los datos que podrían verse comprometidas, también aumenta dramáticamente, pudiendo ser sobornadas o coaccionadas. Ya que una sola compañía puede tener un pequeño equipo de administradores, ingenieros de red y técnicos, pero una compañía de almacenamiento en la nube tendrá muchos clientes y miles de servidores, por lo tanto, un equipo mucho más grande de personal técnico con acceso físico y electrónico a casi todos los datos.
En cuanto a compartir múltiples datos en la nube con múltiples usuarios, se debe distribuir una gran cantidad de claves a los usuarios a través de canales seguros para el descifrado.

Algunas de las soluciones para evitar estos problemas son:
    -Usar una **nube privada** en lugar de una pública.
    -Ingerir datos en **formato cifrado** donde la clave se mantiene dentro de su infraestructura local.

####*Ejemplos / Mejores servicios*

...ICLOUD...

**Amazon Web Services**

Es una colección de servicios de computación en la nube pública. Es usado en aplicaciones populares como Dropbox, Foursquare, HootSuite. Es una de las ofertas internacionales más importantes de la computación en la nube y compite directamente contra servicios como Microsoft Azure y Google Cloud Platform. Es considerado como un pionero en este campo.

Propone soluciones diferentes dependiendo de si se desean almacenar por bloques, por archivos o por objetos:

*Por bloques*: 
Está enfocado a las bases de datos, en donde se ofrecen soluciones como RDS, un manejo de **base de datos relacionales**. Acepta las tecnologías estándar y ofrece su propia tecnología diseñada especialmente para la nube llamada **Aurora**.

*Por archivos*:
Ofrece la posibilidad de manejar un **sistema de archivos en la nube**, en donde se puede almacenar cantidades en el orden de PetaBytes y posee la suficiente concurrencia como para que miles de instancias accedan simultáneamente al mismo archivo. 

*Por objetos*:
Ofrece la posibilidad de **recuperar archivos** desde cualquier ubicación. Por defecto almacenar los datos en uno de estos contenedores asegura que aunque una zona entera de disponibilidad haya caído por completo los datos aún pueden ser recuperados ya que se almacenan en tres zonas de disponibilidad conectadas por cables de fibra totalmente **independientes**. Además, la capacidad de almacenamiento es ilimitada y el precio del servicio depende del tamaño de los datos almacenados y de la cantidad de veces que se accede a ellos.   

Todos los datos almacenados dentro de la plataforma de Amazon cuentan con posibilidad de ser cifrados con claves provistas por el usuario.

####Bibliografia
https://en.wikipedia.org/wiki/Cloud_storage
https://es.wikipedia.org/wiki/Almacenamiento_en_nube
https://azure.microsoft.com/es-es/overview/what-is-cloud-storage/
https://www.seagate.com/es/es/tech-insights/cloud-compute-and-cloud-storage-architecture-master-ti/
