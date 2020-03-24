--Ejemplo 21
--Hallar la cantidad media de ventas realizadas por cada proveedor, indicando
--solamente los códigos de proveedores que han hecho más de 3 ventas

select codpro, avg(cantidad)
from ventas 
group by codpro
having count(*) > 3;

--Ejemplo 22
--Mostrar la media de unidades vendidas de la pieza ’P1’ realizadas por cada
--proveedor, indicando solamente la información de aquellos proveedores que han hecho entre 2 y 10 ventas. 

select codpro, codpie, avg(cantidad) 
from ventas
where codpie = 'P1'
group by (codpro,codpie)
having count(*) between 2 and 10;

--Ejercicio 23
--Encuentra los nombres de proyectos y cantidad media de piezas que recibe por proveedor

select codpro, codpj, avg(cantidad) from ventas group by codpro, codpj;
select codpro, nompj, avg(cantidad) from ventas natural join proyecto group by codpro, codpj, nompj;

--Ejercicio 35
--Mostrar los nombres de proveedores tales que el total de sus ventas superen
--la cantidad de 1000 unidades

select nompro from ventas NATURAL JOIN proveedor group by codpro, nompro having sum(cantidad) > 1000;

--Ejemplo 24
--Mostrar el proveedor que más ha vendido en total

select v1.codpro from ventas v1 group by v1.codpro having sum(v1.cantidad) = (
    select max(sum(v2.cantidad)) from ventas v2 group by v2.codpro
);

--Ejercicio 36
--Mostrar la pieza que más se ha vendido en total

--Esto se refiere al numero de ventas
select codpie, count(*) from ventas group by codpie having count(*) = (
    select max(count(*)) from ventas group by codpie   
);

--Esto se refiere a la suma de la cantidad
select codpie, sum(cantidad) from ventas group by codpie having sum(cantidad) = (
    select max(sum(cantidad)) from ventas group by codpie   
);

--Ejemplo 26
--Encontrar las ventas realizadas entre el 1 de enero de 2002 y el 31 de diciembre de 2004

select * from ventas where fecha between to_date('01/01/2002', 'dd/mm/yyyy') and to_date('31/12/2004', 'dd/mm/yyyy');

--Ejemplo 27
--Mostrar las piezas que nunca fueron suministradas despues del año 2001. 

select distinct codpie from pieza
MINUS
select distinct codpie from ventas where to_number(to_char(fecha, 'YYYY')) > 2001;

--Ejemplo 28
--Agrupar los suministros de la tabla de ventas por años y sumar las cantidades totales anuales.

select * from ventas;

select to_char(fecha, 'yyyy'), sum(cantidad) from ventas group by to_char(fecha, 'yyyy');

--Ejercicio 38
--Encontrar la cantidad media de piezas suministradas cada mes

select to_char(fecha, 'mm'), avg(cantidad) from ventas group by to_char(fecha, 'mm') order by to_char(fecha, 'mm');

--Ejemplo 29
--Mostrar la información de todos los usuarios del sistema; la vista que nos interesa es ALL_USERS

select * from all_users;
describe all_users;

--Ejemplo 30 
--Queremos saber qué índices tenemos definidos sobre nuestras tablas, pero en
--esta ocasión vamos a consultar al propio catálogo para que nos muestre algunas de las vistas que
--contiene (así ya no necesitamos chuleta)

describe dictionary;
select * from dictionary;

--Ejercicio 42
--Mostrar los códigos de aquellos proveedores que hayan superado las ventas
--totales realizadas por el proveedor ’S1’.

select codpro from ventas group by codpro having sum(cantidad) > (
    select sum(cantidad) from ventas where codpro = 'S1'
);

--Ejercicio 43
--Mostrar los mejores proveedores, entendiéndose como los que tienen mayores cantidades totales

select codpro,sum(cantidad) from ventas group by codpro having sum(cantidad) >= (
    select max(sum(cantidad)) from ventas group by codpro
);

--Ejercicio 44
--Mostrar los proveedores que venden piezas a proyectos que estan en todas las ciudades 
--a los que suministra ’S3’, sin incluirlo

--Esta es una consulta que no hace lo del ejercicio pero fue como entendi el ejercicio 
--la primera vez que lo leí
select codpro from proveedor
MINUS
select codpro from (
    (
        select v1.codpro, v2.ciudad
        from (select codpro from proveedor) v1, (select ciudad from (
            proyecto NATURAL JOIN (select * from ventas where codpro = 'S3')
        )) v2
    )
    MINUS
    select codpro, ciudad from proveedor
);

--Resultado bueno
(select codpro from proyecto NATURAL JOIN ventas)
MINUS
(select codpro from(
    (
        select v1.codpro, v2.ciudad
        from 
            (select codpro from proyecto NATURAL JOIN ventas) v1,
            (select ciudad from(
                proyecto NATURAL JOIN (select * from ventas where codpro = 'S3')
            )) v2
    )
    MINUS
    (select codpro, ciudad from proyecto NATURAL JOIN ventas)
))
MINUS
(select codpro from ventas where codpro = 'S3');

--Ejercicio 45
--Encontrar aquellos proveedores que hayan hecho al menos diez pedidos

select codpro, count(*) from ventas group by codpro having count(*) >= 10;

--Ejercicio 46
--Encontrar aquellos proveedores que venden todas las piezas suministradas por S1

select codpro from ventas 
MINUS
select codpro from(
    (
        select v1.codpro, v2.codpie
        from (select distinct codpro from ventas) v1, (select codpie from ventas where codpro = 'S1') v2
    )
    MINUS
    (select codpro, codpie from ventas)
);

--Ejercicio 47
--Encontrar la cantidad total de piezas que ha vendido cada proveedor que
--cumple la condición de vender todas las piezas suministradas por S1

select codpro, sum(cantidad) from ventas group by codpro having codpro IN(
    --ejercicio 46 (justo encima)
    select codpro from ventas 
    MINUS
    select codpro from(
        (
            select v1.codpro, v2.codpie
            from (select distinct codpro from ventas) v1, (select codpie from ventas where codpro = 'S1') v2
        )
        MINUS
        (select codpro, codpie from ventas)
    )
);

--Ejercicio 48
--Encontrar qué proyectos están suministrados por todos los proveedores que
--suministran la pieza P3.

----------------------------------------------------------------------------

--Ejercicio 49
--Encontrar la cantidad media de piezas suministrada a aquellos proveedores
--que venden la pieza P3

select codpro, avg(cantidad) from ventas group by codpro having codpro IN(
    select codpro from ventas where codpie = 'P3'
);

--Ejercicio 52
--Mostrar para cada proveedor la media de productos suministrados cada año

select codpro, to_char(fecha,'yyyy'), avg(cantidad) from ventas group by codpro, to_char(fecha, 'yyyy');

--Ejercicio 53
--Encontrar todos los proveedores que venden una pieza roja

----------------------------------------------------------------------------

--Ejercicio 54
--Encontrar todos los proveedores que venden todas las piezas rojas.

select codpro from ventas
MINUS
(select codpro from(
    (
        select v1.codpro, v2.codpie
        from (select codpro from ventas) v1, (select codpie from pieza where color = 'Rojo') v2
    )
    MINUS
    select codpro, codpie from ventas
));

--Ejercicio 55
--Encontrar todos los proveedores tales que todas las piezas que venden son rojas

select codpro from ventas
MINUS
select codpro from ventas NATURAL JOIN pieza where pieza.color != 'Rojo';

--Ejercicio 56
--Encontrar el nombre de aquellos proveedores que venden más de una pieza roja

create view ej56 as(
(select codpro from ventas)
MINUS
(select codpro 
from (ventas) NATURAL JOIN (select * from pieza where color = 'Rojo')
where cantidad <= 1)
);

select nompro from proveedor NATURAL JOIN (ej56);

--Ejercicio 57
--Encontrar todos los proveedores que vendiendo todas las piezas rojas cumplen
--la condición de que todas sus ventas son de más de 10 unidades

select codpro, sum(cantidad) from ventas group by codpro having codpro IN(
    select codpro from ventas
    MINUS
    (select codpro from(
        (
            select v1.codpro, v2.codpie
            from (select codpro from ventas) v1, (select codpie from pieza where color = 'Rojo') v2
        )
        MINUS
        select codpro, codpie from ventas
    ))
) and sum(cantidad) > 10;

--Ejercicio 58
--Coloca el status igual a 1 a aquellos proveedores que sólo suministran la pieza P1

update proveedor set status = 1 where codpro IN(
select codpro from ventas 
MINUS
select codpro from ventas where codpie != 'P1'
);

--Ejercicio 59
--Encuentra, de entre las piezas que no se han vendido en septiembre de 2009,
--las ciudades de aquellas que se han vendido en mayor cantidad durante Agosto de ese mismo año

create view ej59 as(
select codpie from ventas
MINUS
select codpie from ventas where fecha = to_date('2009', 'YYYY')
);

create view ej592 as(
select codpie from ej59 NATURAL JOIN ventas group by codpie having sum(cantidad) = (
    select max(sum(cantidad)) from ventas group by codpie
));

select ciudad from ej592 NATURAL JOIN pieza;