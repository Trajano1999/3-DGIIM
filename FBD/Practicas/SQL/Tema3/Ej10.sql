select ciudad from proveedor
UNION ALL
select ciudad from pieza
UNION ALL
select ciudad from proyecto;

commit;
exit;