select codpie from pieza
where NOT EXISTS(
    select codpj from proyecto
    where ciudad = 'Londres'
    MINUS
    select codpie from ventas
    where ventas.codpie = pieza.codpie
);

commit;
exit;