include ./Make.defines

.PHONY: daytimecli daytimesrv byteorder pg clean distclean

daytimecli:
	gcc -Ilib -Wall -Wextra -o bin/daytimetcpcli lib/*.c intro/daytimetcpcli.c
	./bin/daytimetcpcli 127.0.0.1

daytimesrv:
	gcc -Ilib -Wall -Wextra -o bin/daytimetcpsrv lib/*.c intro/daytimetcpsrv.c
	./bin/daytimetcpsrv

echosrv:
	gcc -Ilib -Wall -Wextra -o bin/tcpechosrv lib/*.c echo/tcpechosrv.c
	./bin/tcpechosrv

echocli:
	gcc -Ilib -Wall -Wextra -o bin/tcpechocli lib/*.c echo/tcpechocli.c
	./bin/tcpechocli 127.0.0.1

byteorder:
	gcc -Ilib -Wall -Wextra -Wunused-function -o bin/byteorder lib/*.c playground/byteorder.c
	./bin/byteorder

pg:
	gcc -Ilib -Wall -Wextra -o bin/playground lib/*.c playground/playground.c
	./bin/playground

clean:
	rm -f $(CLEANFILES)
 
distclean:
	rm -f $(CLEANFILES) config.cache config.log config.status config.h Make.defines Makefile