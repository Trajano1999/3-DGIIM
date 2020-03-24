all: password.out
	./password.out  ejemplo.txt

password.out:  password.l ejemplo.txt
	flex password.l
	gcc lex.yy.c -o $@ -lfl

clean: 
	rm *.out  *.c
