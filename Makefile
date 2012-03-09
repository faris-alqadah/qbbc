####### Compiler, tools and options

CC	=	g++
CFLAGS	=	-O3 -g -Wno-deprecated
INCPATH	=	
LINK	=	g++
LFLAGS	=	
LIBS	=	

####### Directories

OBJ = objs
SOURCE = source
HEADER = headers
DRIVERS = drivers
BIN = bin
LIB = libs

OBJECTS = $(OBJ)/consistency.o \
			$(OBJ)/dispersion.o \
			$(OBJ)/basic_prefix_alpha.o \
			$(OBJ)/alpha_concepts_algos.o \
			$(OBJ)/helpers.o


LIBS = $(LIB)/libcorelib.a
#driver make programs

MAIN_OBJ = $(OBJ)/star_clus.o



TARGET = $(BIN)/star_clus

#targets


star_clus: $(OBJECTS) $(MAIN_OBJ)
		$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(MAIN_OBJ) $(LIBS)


#install and setup scripts
install:
		mkdir -p $(OBJ) 
		mkdir -p $(BIN)

clean:
		rm -rf $(OBJ)
		rm -rf $(BIN)
# main data structures and functionality



$(OBJ)/consistency.o: $(SOURCE)/consistency.cpp
		$(CC) $(CFLAGS) -c  $(SOURCE)/consistency.cpp -o $@
$(OBJ)/dispersion.o: $(SOURCE)/dispersion.cpp
		$(CC) $(CFLAGS) -c  $(SOURCE)/dispersion.cpp -o $@
$(OBJ)/helpers.o: $(SOURCE)/helpers.cpp
		$(CC) $(CFLAGS) -c  $(SOURCE)/helpers.cpp -o $@
$(OBJ)/basic_prefix_alpha.o: $(SOURCE)/basic_prefix_alpha.cpp
		$(CC) $(CFLAGS) -c  $(SOURCE)/basic_prefix_alpha.cpp -o $@
$(OBJ)/alpha_concepts_algos.o: $(SOURCE)/alpha_concepts_algos.cpp
		$(CC) $(CFLAGS) -c  $(SOURCE)/alpha_concepts_algos.cpp -o $@
$(OBJ)/star_clus.o: star_charm.cpp
		$(CC) $(CFLAGS) -c star_charm.cpp -o $@


