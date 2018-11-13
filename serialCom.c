#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/siginfo.h>
#include <string.h>

#define DATA_SIZE 10 //def 5
#define FRAME_SIZE DATA_SIZE + 8

char send_buf[FRAME_SIZE], rec_buf[FRAME_SIZE],data[DATA_SIZE];
unsigned int checksum(int datal,char* buff);

float rec_f[5] ={1.1 ,2.2, 3.3, 4.4};
char rec_c[5][17];
unsigned int rec_h[5];


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
		    	printf ("Odczytane %d bajtow->%.*s check := %d \n", res,res, rec_buf,(checksum(res,rec_buf)));
		    	//printf ("Odebrany i doczytany: %d, rec_[3]=%c \n",strcmp(rec_buf, send_buf),rec_buf[3]);
		    	if(/*strcmp(rec_buf, send_buf) == 0 &&*/ (checksum(res,rec_buf)) && rec_buf[3] == 'W' ) printf("STATUS: Rozkaz przyjêty \n");

		    	else if(rec_buf[3] == 'R' && (checksum(res,rec_buf)))
		    		{
		    		strncpy(data, &rec_buf[5], res-5-3 );
		    		//printf("DANE: Wartosc rejestru %.*s \n",res,rec_buf);
		    			if('0'<(int)rec_buf[4]<'9') reg_index = (int)rec_buf[4] - '0';

		    			if (0<=((int)rec_buf[4]-'0') && ((int)rec_buf[4]-'0')<=5)
		    				{
		    					rec_f[((int)rec_buf[4]-'0')] = strtof(&data,NULL);
		    					printf("Zapisane %f w rej %d \n",rec_f[((int)rec_buf[4]-'0')],((int)rec_buf[4]-'0'));
		    				}
		    			else if (6<=((int)rec_buf[4]-'0')<=10)
		    				{
		    					strncpy(rec_c[((int)rec_buf[4]-'0')-6], &data, res-2-5 );
		    					printf("Zapisane %s w rej %d \n",rec_c[((int)rec_buf[4]-'0')-6],((int)rec_buf[4]-'0'));
		    				}

		    			else if (11<=((int)rec_buf[4]-'0')<=16)
		    				{
		    					rec_h[((int)rec_buf[4]-'0')-11] = strtod(&data,NULL);
		    					printf("Zapisane %d w rej %d \n",rec_h[((int)rec_buf[4]-'0')-11],((int)rec_buf[4]-'0'));
		    				}
		    			memset(data,NULL,sizeof(data));
		    		}

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

			for ( i =0; i< data_len -3 ;i++ ){sum = sum + buff[i];}

		data_sum = (int)strtol(&buff[data_len -3], NULL, 16);
		sum = sum%256;

		return (data_sum == sum) ? sum : 0;
	}
