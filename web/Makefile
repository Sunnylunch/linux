
.PHONY:all
all:thttpd
	cd cgi_bin;make;cd -

thttpd:/home/admin/code/web/main.o /home/admin/code/web/thttpd.o 
	gcc -o $@ $< -lpthread
%.o:%.c
	gcc -c $<

.PHONY:output
output:
	mkdir output
	cp -rf log output/
	cp -rf conf output/
	cp thttpd output/
	cp -rf wwwroot output/
	cp thttp_ctl.sh output/
	mkdir -p output/wwwroot/cgi_bin
	cp cgi_bin/cgi_math output/wwwroot/cgi_bin

.PHONY:clean
clean:
	rm -rf thttpd *.o output;cd cgi_bin;make clean;cd -
