thttpd:/home/admin/code/web/main.o /home/admin/code/web/thttpd.o 
	gcc -o $@ $< -lpthread
%.o:%.c
	gcc -c $<
.PHONY:clean
clean:
	rm -rf thttpd *.o
