objects = ComStruct.o
CC = g++  -O2 
MATCHFLAGS =  -I  ../GraphMatch/include  
install : $(objects)
	
ComStruct.o  : ComStruct.cpp  ComStruct.h    
	$(CC)   -c  $<    $(MATCHFLAGS)
 
clean :
	rm  $(objects) 

