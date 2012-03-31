export DEBUG    = no 
export STATIC   = no

ifeq ($(DEBUG), yes)
    DEBUGFLAGS = -gstabs+ -DDEBUG
else
    DEBUGFLAGS = -O3 -DNDEBUG
endif

SRC = Vector.cpp POSIXTimer.cpp
OBJ = $(SRC:.cpp=.o)

CPP         = g++
INCLUDES    = 
LIBFLAGS    = -lSDL -lSDL_image
CFLAGS      = -Wall -pedantic $(LIBFLAGS) $(INCLUDES) $(DEBUGFLAGS)

all: $(OBJ) earf

earf: earf.cpp
	$(CPP) $(CFLAGS) -o $@ $(OBJ) earf.cpp

$(OBJ): %.o: %.cpp
	$(CPP) -o $@ -c $< $(CFLAGS)

clean:
	@(rm -rf *.o earf)
