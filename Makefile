CXX = g++
CXXFLAGS = 
CPPFLAGS = -Wall -g -I/scratch/perkins/include


#Define library path
LDFLAGS = -L/scratch/perkins/lib

#Define any libraries to link into executable
LDLIBS = -lcdk -lcurses

#PROJECTNAME for backup target 
PROJECTNAME = CS3377.HW6

EXECFILE = HW6

OBJS = HW6.o

all: $(EXECFILE)

clean:
	rm -f $(OBJS) $(EXECFILE) *.P *~ \#*


$(EXECFILE): $(OBJS)
	$(CXX) -o $(EXECFILE) $(OBJS) $(IFLAGS) $(LDFLAGS) $(LDLIBS)

backup:
	@make clean
	@mkdir -p ~/backups; chmod 700 ~/backups
	@$(eval CURDIRNAME := $(shell pwd))
	@$(eval MKBKUPNAME := ~/backups/$(PROJECTNAME)-$(shell date +'%Y.%m.%d-%H.%M.%S').tar.gz)
	@echo
	@echo Writing backup file to: $(MKBKUPNAME)
	@echo
	@-tar zcvf $(MKBKUPNAME) $(CURDIRNAME)
	@chmod 600 $(MKBKUPNAME)
	@echo
	@echo DONE!!
