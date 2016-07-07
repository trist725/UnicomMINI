ROOTSRC=$(wildcard *.cpp)
ROOTOBJ=$(ROOTSRC:%.cpp=%.o)
#ROOTSRCPP=$(shell ls *.cpp)
#ROOTOBJPP=$(ROOTSRC:%.cpp=%.o)
ALL:clean main clean_o
main:$(ROOTOBJ)
#	@echo $(ROOTSRC)
#	@echo $(ROOTOBJ)
	g++ -g -o myUI $(ROOTOBJ) -lncurses -lpthread
%.o:%.cpp
	g++ -c -g $< -o $@
clean_o:
	rm -rf *.o
clean:
	rm -rf *.o myUI
