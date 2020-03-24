select codpj, avg(cantidad)
from ventas
where codpie = 'P1'
group by codpj;

commit;
exit;