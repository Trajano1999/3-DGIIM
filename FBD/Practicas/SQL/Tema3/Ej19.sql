select codpie
from ventas
where codpro IN (select codpro from proveedor where ciudad = 'Madrid');

select codpie
from ventas NATURAL JOIN (select * from proveedor where ciudad = 'Madrid');

commit;
exit;