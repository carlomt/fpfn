CXX           = g++ 
CXXFLAGS      = -O -Wall -fPIC 
LD            = g++ 
LDFLAGS       = -O -Wall -fPIC

OBJS	= fpfn.o
HEADERS	= myStod.h myStoi.h

default: ${OBJS} ${HEADERS}
		$(LD) $(LDFLAGS) ${OBJS}  -o fpfn.x

clean:
		rm -f fpfn.x ${OBJS}

fpfn.o:	fpfn.C
		$(CXX) $(CXXFLAGS) -c fpfn.C -o fpfn.o

# myStod.o: myStod.C
# 		$(CXX) $(CXXFLAGS) -c myStod.C -o myStod.o
