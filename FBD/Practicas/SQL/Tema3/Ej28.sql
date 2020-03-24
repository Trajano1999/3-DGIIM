select codpie, peso
from pieza
where peso >= (select max(peso) from pieza);

commit;
exit;