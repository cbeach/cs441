
OBJECTS :=\
	search.o

SOURCE :=\
	search.cpp

CC := g++
HEADERFILES := 
LDFLAGS := -o
CPFLAGS := -c -g -Wall -fpermissive


INCLUDES := 

all: $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) search $(INCLUDES)

$(OBJECTS): $(SOURCE)
	$(CC) $(HEADERFILES) $(SOURCE) $(CPFLAGS)

clean:
	rm -f *.o\
	all
	

