create table proyecto(
    codpj varchar2(3) constraint codpj_clave_primaria primary key,
    nompj varchar2(20) constraint nompj_no_nulo not null,
    ciudad varchar2(15)
);
commit;
exit