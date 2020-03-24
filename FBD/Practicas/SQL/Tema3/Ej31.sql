select codpie, avg(cantidad)
from ventas
group by (codpie);

commit;
exit;