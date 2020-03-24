select * from ventas;

select codpro, codpie, codpj from ventas where cantidad <= 300 and cantidad >= 200;
commit;
exit;