--TEMA 4--

CREATE VIEW VentasParis (codpro,codpie,codpj,cantidad,fecha) AS
    SELECT codpro,codpie,codpj,cantidad,fecha
    FROM ventas
    WHERE (codpro,codpie,codpj) IN(
        SELECT codpro,codpie,codpj
        FROM proveedor,pieza,proyecto
        WHERE proveedor.ciudad='Paris' AND
        pieza.ciudad='Paris' AND
        proyecto.ciudad='Paris'
    );
    
select * from all_views;

--Ejemplo 2
--Extraer el conjunto de piezas procedentes de Londres, prescindiendo del atributo
--ciudad de la tabla original.

create view PiezasLondres as(
    select codpie, nompie, color, peso from pieza where ciudad = 'Londres'
);

insert into PiezasLondres values('P9', 'Pieza 9', 'Rojo', 90);

--Ejercicio 1
--Crear una vista con los proveedores de Londres. ¿Qué sucede si insertamos
--en dicha vista la tupla (’S7’,’Jose Suarez’,3,’Granada’)?. (Buscar en [8] la cláusula
--WITH CHECK OPTION ). 

create or replace view ProveedoresLondres as(
    select codpro, nompro, status, ciudad from proveedor where ciudad = 'Londres'
);

select * from ProveedoresLondres;
insert into ProveedoresLondres values('S7','Jose Suarez',3,'Granada');

--Ejercicio 2
--Crear una vista con los nombres de los proveedores y sus ciudades. Inserta
--sobre ella una fila y explica cuál es el problema que se plantea. ¿Habría problemas de actualización?

create view ej2 as(
    select nompro,ciudad from proveedor
);

insert into ej2 values('Proveedor 28', 'Granada');
--No me permite porque falta la clave primaria

--Ejercicio 3
--Crear una vista donde aparezcan el código de proveedor, el nombre de proveedor
--y el código del proyecto tales que la pieza sumistrada sea gris. Sobre esta vista realiza
--alguna consulta y enumera todos los motivos por los que sería imposible realizar una inserción

create view ej3 as(
    select x.codpro, y.codpj, x.nompro 
    from 
        proveedor x,
        (select * from (ventas NATURAL JOIN (select * from pieza where color = 'Gris'))) y
    where x.codpro = y.codpro
);

commit;
exit;