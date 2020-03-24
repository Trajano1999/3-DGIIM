select codpie, peso
from pieza
where peso >= ALL(select peso from pieza);

commit;
exit;