select distinct ciudad 
from proyecto NATURAL JOIN ventas;

select distinct ciudad
from proyecto, ventas
where proyecto.codpj=ventas.codpj;

select codpro
from ventas NATURAL JOIN pieza
where NOT EXISTS (
    select ciudad from pieza
    MINUS
    select ciudad from proyecto
);

commit;
exit;