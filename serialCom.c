#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/siginfo.h>
#include <string.h>

#define DATA_SIZE 5
#define FRAME_SIZE DATA_SIZE + 8

char send_buf[FRAME_SIZE], rec_buf[FRAME_SIZE];
unsigned int checksum(int datal,char* buff);

int main(int argc, char *argv[]) {

	int fd,res,s_res;

		fd = open( "/dev/ser1", O_RDWR);
		if (fd != -1) printf("(%d)- Port otwarty \n", fd);

	    while(1)
	    {
	    	memset(rec_buf,NULL,sizeof(rec_buf));
	    	res = readcond( fd, &rec_buf, FRAME_SIZE, FRAME_SIZE, 0, 30);

		    if( res >0 )
		    {
		    	printf ("Odczytane %d bajtow->%.*s \n", res,res, rec_buf);
		    	//printf ("Odebrany i doczytany: %d, rec_[3]=%c \n",strcmp(rec_buf, send_buf),rec_buf[3]);
		    	if(strcmp(rec_buf, send_buf) == 0 && (checksum(res,rec_buf))) printf("STATUS: Rozkaz przyjêty \n");
		    	else if(rec_buf[3] == 'R' && (checksum(res,rec_buf))) printf("DANE: Wartosc rejestru %.*s \n",res,rec_buf);
		    	else if (rec_buf[3] == 'N' && (checksum(res,rec_buf))) printf("ERROR: Blad sumy kontrolnej\n");

		    }
		    else
		    {
		    	memset(send_buf,NULL,sizeof(send_buf));
		    	printf("Podaj ramke do wyslania:\n");
		    	scanf("%s",send_buf);
		    	printf("Wyslane->%s\n",send_buf);
		    	s_res = write(fd,send_buf,sizeof(send_buf));
		    	if ( s_res == sizeof(send_buf) ) printf("Ramka wyslana %d bajtow\n",s_res);
		    }
	    }
	    close(fd);
	return EXIT_SUCCESS;
}

	unsigned int checksum(int data_len, char* buff){

		unsigned int i,data_sum=0,sum =0;

			for ( i =0; i< data_len -3 ;i++ )
			{
				sum = sum + buff[i];
			}
		data_sum = (int)strtol(&buff[data_len -3], NULL, 16);
		sum = sum%256;
		return (data_sum = sum) ? 1 : 0;
	}
