select codpro, codpie, TO_CHAR(fecha,'"Dia" day,/mm/yy') from ventas;

commit;
exit;