objects =ComStruct/ComStruct.o  
CC = g++  -std=c++11  -g  -I ComStruct/ 
all:    prepareall
	$(CC)  -o   GraphGen    main.cpp           $(objects) 
prepareall:    subsystem
subsystem:
	$(MAKE) -C ComStruct/
clean :  cleansub
	rm     GraphGen  
cleansub :
	rm  $(objects)

