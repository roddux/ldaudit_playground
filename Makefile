#CFLAGS=-Wall -Wextra -pedantic
.PHONY: default clean
default: all

src=testlib.c testbin.c audit.c
obj=$(src:.c=.o)
lib=testlib.so audit.so
bin=testbin
dist=./dist

.c.o:
	$(CC) $(CFLAGS) $< -c -fPIC -o $@

%.so:
	$(CC) $(CFLAGS) $(subst .so,.o,$@) -shared -fPIC -o $@

$(bin):
	$(CC) $(CFLAGS) $@.o testlib.so -Wl,-rpath,. -o $@

all: $(obj) $(lib) $(bin)

clean:
	rm -rf $(obj) $(lib) $(bin) $(dist)

install:
	mkdir $(dist)
	mv $(bin) $(lib) $(dist)

run:
	LD_AUDIT=audit.so ./testbin
