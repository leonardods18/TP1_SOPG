#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdint.h>
#include <signal.h>
#define FIFO_NAME "myfifo"
#define BUFFER_SIZE 300
char outputBuffer2[BUFFER_SIZE+6];
char outputBuffer[BUFFER_SIZE];
uint32_t bytesWrote;
int32_t returnCode, fd;
int32_t f_signal;

void hand(int sig)			// espero solo una señal
{
		f_signal=1;
		outputBuffer[0] ='S';
		outputBuffer[1] ='I';
		outputBuffer[2] ='G';
		outputBuffer[3] ='N';
		outputBuffer[4] ='1';
		
	if (bytesWrote = write(fd, outputBuffer, 5) == -1)
        {
			perror("write");
        }
        else
        {
			printf("Se mando una señal\n\r"); 
			
			
        }
}


int main(void)
{
   	struct sigaction sa;
	sa.sa_handler = hand;
	sa.sa_flags = 0; //SA_RESTART; //0;
	sigemptyset(&sa.sa_mask);

	sigaction(SIGINT,&sa,NULL);	
    char outputBuffer[BUFFER_SIZE];


//----------espero una señal

char s[200];
	
    /* Create named fifo. -1 means already exists so no action if already exists */
    if ( (returnCode = mknod(FIFO_NAME, S_IFIFO | 0666, 0) ) < -1 )
    {
        printf("Error creating named fifo: %d\n", returnCode);
        exit(1);
    }

    /* Open named fifo. Blocks until other process opens it */
	printf("Esperando Lectores...\n");
	if ( (fd = open(FIFO_NAME, O_WRONLY) ) < 0 )
    {
        printf("Error opening named fifo file: %d\n", fd);
        exit(1);
    }
    
    /* open syscalls returned without error -> other process attached to named fifo */
	printf("Tenemos un Lector: puede tipear un texto o mandar una señal\n");

    /* Loop forever */
	while (1)
	{
 
		fgets(outputBuffer, BUFFER_SIZE, stdin);
		outputBuffer2[0] ='D';
		outputBuffer2[1] ='A';
		outputBuffer2[2] ='T';
		outputBuffer2[3] ='A';
		outputBuffer2[4] =':';
		if(f_signal == 0) {printf("tipo de mensaje: DATO - PRESIONE ENTER PARA ENVIAR!\n"); }
		else{
		printf("Se mando una señal con exito!!!\n"); f_signal = 0;
		}
		for(int i=0; i<BUFFER_SIZE; i++)
        	{
		outputBuffer2[i+5] =outputBuffer[i];	

 		}
	if(fgets(s,sizeof(s),stdin) == NULL)
	{
		perror("error en fgets\n");
	}
	else
	{
		printf("ingresaste:%s\n",s);
	}

	if ((bytesWrote = write(fd, outputBuffer2, strlen(outputBuffer2)-1)) == -1)
        {
			perror("write");
        }
        else
        {
			printf("Se mando un Dato de: %d bytes\n", bytesWrote);
			
        }        



	}
	return 0;
}
