select codpie, codpj, sum(cantidad) from ventas group by codpie, codpj;

commit;
exit;
