all : randmain
randmain : randmain.o randcpuid.o
	gcc -ldl -Wl,-rpath=$(PWD) $(CFLAGS) randcpuid.o randmain.o -o randmain
randmain.o : randmain.c randcpuid.h
	gcc -c $(CFLAGS) randmain.c
randcpuid.o : randcpuid.c randcpuid.h
	gcc -c $(CFLAGS) randcpuid.c
randlibhw.so : randlibhw.o
	gcc $(CFLAGS) -shared randlibhw.o -o randlibhw.so
randlibhw.o : randlibhw.c randlib.h
	gcc -fPIC -c $(CFLAGS) randlibhw.c
randlibsw.so : randlibsw.o
	gcc $(CFLAGS) -shared randlibsw.o -o randlibsw.so
randlibsw.o : randlibsw.c randlib.h
	gcc -fPIC -c $(CFLAGS) randlibsw.c
