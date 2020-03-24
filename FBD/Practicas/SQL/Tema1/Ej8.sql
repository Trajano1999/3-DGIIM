create table equipos(
    codE varchar2(4) constraint codE_clave_primaria primary key,
    nombreE varchar2(10) UNIQUE constraint nombreE_no_nulo NOT NULL,
    localidad varchar2(3) constraint localidad_no_nulo NOT NULL,
    entrenador varchar2(5) constraint entrenador_no_nulo NOT NULL,
    fecha_crea varchar2(10) constraint fecha_crea_no_nulo NOT NULL
);
describe equipos;

create table jugadores(
    codJ varchar2(4) constraint codJ_clave_primaria primary key,
    codE constraint codE_no_nulo NOT NULL constraint codE_clave_externa_equipos references equipos(codE),
    nombreJ varchar2(10) constraint nombreJ_no_nulo NOT NULL    
);
describe jugadores;

create table encuentros(
    ELocal constraint ELocal_clave_externa references equipos(codE),
    EVisitante constraint EVisitante_clave_externa references equipos(codE),
    fecha varchar(2),
    PLocal int default 0 constraint PLocal_positivo check (PLocal >= 0),
    PVisitante int constraint PVisitante_positivo check (PVisitante >= 0),
    constraint clave_primaria_encuentros primary key (ELocal,EVisitante) 
);
describe encuentros;

create table faltas(
    codJ constraint codJ_clave_externa references jugadores(codJ),
    ELocal varchar2(4),
    EVisitante varchar(2),
    constraint clave_externa_faltas foreign key (ELocal,EVisitante) references encuentros(ELocal,EVisitante),
    numero int default 0 constraint numero_entre_0_y_5 check(numero >= 0 and numero <= 5),
    constraint clave_primaria_faltas primary key (codJ,ELocal,EVisitante)
);
describe faltas;

commit;
exit;
