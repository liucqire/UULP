smsh1:smsh1.o execute.o splitline.o
	gcc -Wall -o $@ $^
smsh1.o:smsh1.c
	gcc -Wall -c -o $@ $^ 
execute.o:execute.c
	gcc -Wall -c -o $@ $^
splitline.o:splitline.c
	gcc -Wall -c -o $@ $^
clean:
	@rm *.o
