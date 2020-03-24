CREATE TABLE plantilla2(
dni char(8) PRIMARY KEY,
nombre varchar2(15),
estadocivil varchar(10),
fechaalta date
);
alter table plantilla2 add(
fechabaja date
);
describe plantilla2;
commit;
exit;