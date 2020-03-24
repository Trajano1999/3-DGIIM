UPDATE ventas
SET fecha = TO_DATE(2005,'YYYY')
WHERE codpro='S5';

select * from ventas;

commit;
exit;