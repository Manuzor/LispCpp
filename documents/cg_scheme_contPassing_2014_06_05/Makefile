O=o
CFLAGS=-g

all: scheme

OBJS=schemeMain.$(O) \
     schemeReader.$(O) \
     schemeMemory.$(O) \
     schemeErrors.$(O) \
     schemePrinter.$(O) \
     schemeEnvironment.$(O) \
     schemeEval.$(O) \
     schemeBuiltin.$(O) \
     schemeDebug.$(O)


scheme: $(OBJS)
	$(CC) -o scheme $(OBJS)

$(OBJS): scheme.h schemeFunctions.h

.c.o:
	$(CC) $(CFLAGS) -c $*.c

clean:
	rm -f $(OBJS) scheme cscope.out

tar:
	tar cvfz scheme2014_contPassing.tgz *.c *.h Makefile *.scm
