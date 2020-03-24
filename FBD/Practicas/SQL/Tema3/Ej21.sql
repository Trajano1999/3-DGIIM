select codpj
from ventas
where codpro IN (select codpro from proveedor where ciudad = 'Londres')
and codpie IN(select codpie from pieza where color != 'Rojo');

commit;
exit;