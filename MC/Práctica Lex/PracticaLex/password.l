	 /*------------------Sección de declaraciones -------------------------*/

%{
#include <stdio.h>
int valida;
%}

digito							[0-9]+
letra_minuscula			[a-z]+
letra_mayuscula			[A-Z]+
caracter_especial			[\.,:;\+\-\*&#%\$¡!¿?]+

x								[^ \b\t\n]*
_0								{digito}
_1								{letra_minuscula}
_2								{letra_mayuscula}
_3								{caracter_especial}

password					(({x}{_0}{x}{_1}{x}{_2}{x}{_3}{x})|({x}{_0}{x}{_1}{x}{_3}{x}{_2}{x})|({x}{_0}{x}{_2}{x}{_1}{x}{_3}{x})|({x}{_0}{x}{_2}{x}{_3}{x}{_1}{x})|({x}{_0}{x}{_3}{x}{_2}{x}{_1}{x})|({x}{_0}{x}{_3}{x}{_1}{x}{_2}{x})|({x}{_1}{x}{_0}{x}{_2}{x}{_3}{x})|({x}{_1}{x}{_0}{x}{_3}{x}{_2}{x})|({x}{_1}{x}{_2}{x}{_0}{x}{_3}{x})|({x}{_1}{x}{_2}{x}{_3}{x}{_0}{x})|({x}{_1}{x}{_3}{x}{_0}{x}{_2}{x})|({x}{_1}{x}{_3}{x}{_2}{x}{_0}{x})|({x}{_2}{x}{_0}{x}{_1}{x}{_3}{x})|({x}{_2}{x}{_0}{x}{_3}{x}{_1}{x})|({x}{_2}{x}{_1}{x}{_0}{x}{_3}{x})|({x}{_2}{x}{_1}{x}{_3}{x}{_0}{x})|({x}{_2}{x}{_3}{x}{_0}{x}{_1}{x})|({x}{_2}{x}{_3}{x}{_1}{x}{_0}{x})|({x}{_3}{x}{_0}{x}{_1}{x}{_2}{x})|({x}{_3}{x}{_0}{x}{_2}{x}{_1}{x})|({x}{_3}{x}{_1}{x}{_0}{x}{_2}{x})|({x}{_3}{x}{_1}{x}{_2}{x}{_0}{x})|({x}{_3}{x}{_2}{x}{_0}{x}{_1}{x})|({x}{_3}{x}{_2}{x}{_1}{x}{_0}{x}))+

%%
    /*---------------------Sección de reglas -----------------------------*/

{password}					{valida=1;}

%%
    /*-------------------- Sección de procedimientos ---------------------*/

int main(int argc, char* argv[]){
    if(argc == 2){
        yyin = fopen(argv[1],"rt");
        if(yyin == NULL){
            printf("El fichero %s no se puede abrir\n", argv[1]);
            exit(-1);
        }
    }else yyin = stdin;

    valida = 0;
    yylex();

	if(valida == 0)
   		printf("La contraseña NO es válida\n");
	else		
   		printf("La contraseña SI es válida\n");

    return 0;
}
