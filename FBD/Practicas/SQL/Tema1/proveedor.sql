create table proveedor(
codpro varchar2(3) constraint codpro_no_nulo NOT NULL
constraint codpro_clave_primaria primary key,
nompro varchar2(30) constraint nompro_no_nulo NOT NULL,
status number constraint status_entre_1_y_10
    CHECK (status>=1 and status<=10),
ciudad varchar2(15)
);
commit;
exit;