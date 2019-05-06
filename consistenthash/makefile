CFLAGS+=-g -Wall -std=c++11 
INC+= -I. -I../
LIB+= -ldl
vpath %.cc ../common/
OBJ+= main.o\
	consistenthash.o\
	md5.o\
	common.o\

	  
	  
	
TARGET=consistenthash


#############################################################
$(TARGET):$(OBJ)
	g++ $(CFLAGS) -o $@ $^ $(LIB) 
	rm -f *.o

%.o: %.cpp
	g++ $(CFLAGS) $(INC) -c -o $@ $< 
%.o: %.cc
	g++ $(CFLAGS) $(INC) -c -o $@ $< 

clean:
	rm -f *.o
	rm -f $(TARGET) 
