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

fpfn.o:	fpfn2.C fpfn2.h
		$(CXX) $(CXXFLAGS) -c fpfn2.C -o fpfn.o

# myStod.o: myStod.C
# 		$(CXX) $(CXXFLAGS) -c myStod.C -o myStod.o


minAct.o: minAct.C fpfn2.h
		$(CXX) $(CXXFLAGS) -c minAct.C -o minAct.o

minAct.x: minAct.o
		 $(LD) $(LDFLAGS) minAct.o  -o minAct.x 
