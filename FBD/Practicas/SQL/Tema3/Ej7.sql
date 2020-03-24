select ciudad from proveedor where status>2
INTERSECT
select ciudad from pieza where codpie!='P1';

select ciudad from proveedor where status>2
MINUS
select ciudad from pieza where codpie='P1';

commit;
exit;