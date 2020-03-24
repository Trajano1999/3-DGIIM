select codpj
from proyecto
where ciudad IN (select ciudad from pieza);

commit;
exit;