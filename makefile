SRCS = $(shell find ./src/ -name *.c)
OBJS = $(SRCS:.c=.o)
HEADERS = $(shell find ./inc/ -name *.h)

all : file_io.a

file_io.a : $(SRCS) $(HEADERS)
	$(MAKE) --directory=./src/
	ar crv file_io.a $(OBJS)
	ranlib file_io.a


.PHONY : clean all test



clean :
	$(MAKE) --directory=./src/ clean
	rm -f *.[oa]

test :
	@echo =========================
	@echo $(SRCS)
