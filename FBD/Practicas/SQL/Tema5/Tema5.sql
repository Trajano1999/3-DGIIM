--TEMA 5--

--Ejercicio 2

create table acceso (testigo number);

insert into acceso values (1);
insert into acceso values (2);

select * from acceso;

--Autorizar al usuario de tu derecha para que pueda hacer consultas sobre esa tabla

grant select ON acceso TO usuario_derecha;

--Comprobar que se puede acceder a la tabla del usuario de la izquierda

select * from usuario_izquierda.acceso;

--Retirar el privilegio de consulta antes concedido

revoke select ON acceso FROM usuario_derecha;

--Autorizar ahora al usuario de la derecha para que pueda hacer consultas sobre la tabla,
--pero ahora con posibilidad de que este propague ese privilegio

grant select ON acceso TO usuario_derecha with grant option;

--Propagar el privilegio concedido por el usuario de la izquierda hacia el usuario de la derecha

grant select ON usuario_izquierda.acceso TO usuario_derecha.acceso;

--Comprobar que se pueden acceder a las tablas del usuario de la derecha y del anterior

SELECT * FROM usuario_izquierda.acceso;
SELECT * FROM usuario_izquierda_del_usuario_izquierda.acceso;

--Retira el privilegio antes concedido. ¿Qué ocurre con los accesos?

revoke select ON acceso FROM usuario_derecha;
select * from usuario_izquierda.acceso;
select * from usuario_izquierda_usuario_izquierda.acceso;