select codpie
from pieza
where peso > ANY (select peso from pieza where nompie LIKE '_ornillo');

commit;
exit;