select ventas.codpro, ventas.codpie, ventas.codpj
from ventas, proyecto, pieza, proveedor
where proyecto.ciudad = pieza.ciudad and pieza.ciudad = proveedor.ciudad;

commit;
exit;