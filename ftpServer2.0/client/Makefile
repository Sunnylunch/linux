SRC=main.c ftpClient.c ../common/common.c
CC=gcc

ftpClient:$(SRC)
	$(CC) -o $@ $^

.PHONY:clean
clean:
	rm -rf ftpClient
