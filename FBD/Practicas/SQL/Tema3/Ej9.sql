select ciudad from proveedor
UNION
select ciudad from pieza
UNION
select ciudad from proyecto;

commit;
exit;