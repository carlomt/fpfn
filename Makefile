CXX           = g++ 
CXXFLAGS      = -O -Wall -fPIC 
LD            = g++ 
LDFLAGS       = -O -Wall -fPIC

OBJS	= fpfnlib.o 
HEADERS	= myStod.h myStoi.h

default: ${OBJS}  fpfn.o ${HEADERS}
		$(LD) $(LDFLAGS) ${OBJS} fpfn.o   -o fpfn.x

paperIso:  ${OBJS}  paperIso.o ${HEADERS}
		$(LD) $(LDFLAGS) ${OBJS} paperIso.o -o paperIso.x
clean:
		rm -f fpfn.x ${OBJS} paperIso.o fpfn.o

fpfn.o:	fpfn.C 
		$(CXX) $(CXXFLAGS) -c fpfn.C -o fpfn.o

paperIso.o:	paperIso.C 
		$(CXX) $(CXXFLAGS) -c paperISO.C -o paperIso.o

fpfnlib.o:	fpfnlib.C fpfnlib.h
		$(CXX) $(CXXFLAGS) -c fpfnlib.C -o fpfnlib.o

# myStod.o: myStod.C
# 		$(CXX) $(CXXFLAGS) -c myStod.C -o myStod.o


minAct.o: minAct.C fpfn2.h
		$(CXX) $(CXXFLAGS) -c minAct.C -o minAct.o

minAct.x: minAct.o
		 $(LD) $(LDFLAGS) minAct.o  -o minAct.x 
