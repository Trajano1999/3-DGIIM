select codpro
from ventas v1
where (
    select count(*) from ventas v2 where v2.codpro = v1.codpro
) > 3;

commit;
exit;