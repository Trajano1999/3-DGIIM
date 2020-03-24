select codpro from ventas
MINUS
select codpro from(
    select v.codpro, p.codpie
    from (select distinct codpro from ventas) v, (select codpie from pieza) p
    MINUS
    select codpro,codpie from ventas
);

select codpro from proveedor
where NOT EXISTS (
    select * from pieza
    where NOT EXISTS (
        select * from ventas
        where pieza.codpie = ventas.codpie and proveedor.codpro = ventas.codpro
    )
);

select codpro from proveedor
where NOT EXISTS(
    select distinct codpie from pieza
    MINUS
    select distinct codpie from ventas where proveedor.codpro=ventas.codpro
);

commit;
exit;