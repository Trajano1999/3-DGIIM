select distinct pie.nompie, pie.peso
from pieza p, pieza pie
where p.peso < pie.peso;

commit;
exit;