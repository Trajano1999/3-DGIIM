select distinct pro.nompro, p.nompro
from proveedor pro, proveedor p
where pro.ciudad != p.ciudad;

commit;
exit;