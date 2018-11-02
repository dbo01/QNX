#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/iomsg.h>
#include <sys/siginfo.h>
#include <string.h>


#define DATA_SIZE 5
#define FRAME_SIZE DATA_SIZE + 8

unsigned char send_buf[FRAME_SIZE], rec_buf[FRAME_SIZE];

int main(int argc, char *argv[]) {

	int fd,res,s_res;

		printf("Komunikacja po porcie szeregowym \n");

		fd = open( "/dev/ser1", O_RDWR);

		if (fd != -1) printf("(%d)- Port otwarty \n", fd);

	    while(1)
	    {
	    	memset(rec_buf,' ',sizeof(rec_buf));

	    	res = readcond( fd, &rec_buf, FRAME_SIZE, FRAME_SIZE, 0, 30);

		    if( res >0 )
		    {
		    	printf ("Odczytane %d bajtow: %.*s \n", res,res, rec_buf);
		    	//printf ("Odebrany i doczytany: %d, rec_[3]=%c \n",strcmp(rec_buf, send_buf),rec_buf[3]);
		    	if(strcmp(rec_buf, send_buf) == 1) printf("Rozkaz przyjêty \n");
		    	else if(rec_buf[3] == 'R') printf("Wartosc rejestru %.*s \n",res,rec_buf);
		    	else if (rec_buf[3] == 'N') printf("ERROR\n");
		    }
		    else
		    {

		    	memset(send_buf,' ',FRAME_SIZE);
		    	printf("Podaj ramke do wyslania:\n");
		    	scanf("%s",send_buf);
		    	s_res = write(fd,send_buf,sizeof(send_buf));
		    	if ( s_res == sizeof(send_buf) ) printf("Ramka wyslana %d bajtow\n",s_res);

		    }
	    }
	    close(fd);

	return EXIT_SUCCESS;
}
