insert into equipos
values('E1', 'Ogijares', 'Gra', 'Javi', '1/1/2000');
insert into equipos
values('E2', 'Armilla', 'Gra', 'Juan', '1/2/2000');
insert into equipos
values('E3', 'Padul', 'Gra', 'Jose', '3/4/2010');
insert into equipos
values('E4', 'Zubia', 'Gra', 'Adri', '1/1/2000');

insert into jugadores
values('J1', 'E1', 'Joaquin');
insert into jugadores
values('J2', 'E1', 'Joaquin');
insert into jugadores
values('J3', 'E1', 'Joaquin');
insert into jugadores
values('J4', 'E1', 'Joaquin');
insert into jugadores
values('J5', 'E1', 'Joaquin');

insert into jugadores
values('J6', 'E2', 'Ceci');
insert into jugadores
values('J7', 'E2', 'Ceci');
insert into jugadores
values('J8', 'E2', 'Ceci');
insert into jugadores
values('J9', 'E2', 'Ceci');
insert into jugadores
values('J10', 'E2', 'Ceci');
...

insert into encuentros
values('E1', 'E2', '12', 5, 0);
...

select * from equipos;
select * from jugadores;
select * from encuentros;

commit;
exit;