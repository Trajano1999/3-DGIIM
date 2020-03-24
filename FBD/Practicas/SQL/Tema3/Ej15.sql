select codpie
from ventas NATURAL JOIN (select * from proveedor where ciudad = 'Madrid');

commit;
exit;