// https://github.com/ZainullinsDV/UsefullSources/new/main/tcpserver
// use sudo ./a.out for portnumbers < 1000
// for ethical legal usage only!

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>


#define MAXBUFSIZE	50000
#define RESPONSEFILENAME	"./response.bin"

char respbuf[MAXBUFSIZE] = {0x0};
int  resplen=0;

void	readresponsefromfile() {
	int flag = O_RDWR;
	int fd;
	int i=0;
	int count = 1;
	int res = 0;
	int len=0;

	fd = open(RESPONSEFILENAME,flag);
	if(fd<0) {
		printf("FILE OPEN ERROR\n");
		return;
	}

	res = read(fd,respbuf,count);
	while(res > 0 and i+1<MAXBUFSIZE) {
		i++;
		res = read(fd,respbuf+i,count);
	}
	len=i;
	printf("\nREAD FROM FILE (len=%d):\n",len);
/*	printf("\n-------------------------------------\n");
	for(i=0;i<len;i++) {
		printf("%c",respbuf[i]);
	}
	*/
	printf("\n-------------------------------------\n");

	resplen = len;
	return;
}




int main() {
	int in_sock, listener,out_sock;
	struct sockaddr_in listen_addr;
	struct sockaddr_in con_addr;
	int bytes_read;
	char buf[MAXBUFSIZE] = {0x0};
	int enable = 1;

	memset(buf,0,MAXBUFSIZE);

	readresponsefromfile(); 

	listener = socket(AF_INET, SOCK_STREAM, 0);
	
	if(listener < 0) {
		printf("Socket create error");
		return 1;
	}
	printf("Socket success, continue...\n");

	listen_addr.sin_family = AF_INET;
	listen_addr.sin_port = htons(80);
	listen_addr.sin_addr.s_addr = htonl(INADDR_ANY);


	if (setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0) {
    		printf("setsockopt(SO_REUSEADDR) failed\n");
		return 1;
	}

	if(bind(listener, (struct sockaddr *)&listen_addr, sizeof(listen_addr)) < 0) {
		printf("bind failture");
		return 2;
	}
	printf("bind success, continue...\n");

	listen(listener, 10);
	printf("Перевели сокет в режим СЛУШАТЬ\n");

while(1) {

	in_sock = accept(listener,NULL, NULL);

	printf("Произошел выход из блокирующего вызова, значит пришло соединение, надо проверить сокет и получить данные\n");

	if(in_sock <0) {
		printf("accept failture\n");
		return 3;
	}
	printf("accept OK, calling recv...\n");


	bytes_read = recv(in_sock, buf, MAXBUFSIZE, 0);

        printf("Got %d bytes from client:\n",bytes_read);

	for(int i=0; i<bytes_read;i++) {
		printf("%c",buf[i]);
	}


	send(in_sock, respbuf, resplen, 0);
	printf("SENT RESPONSE\n-----------------------\n");

	shutdown(in_sock,2);
}

return 0;
}



