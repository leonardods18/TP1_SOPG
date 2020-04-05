#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdint.h>

#define FIFO_NAME "myfifo"
#define BUFFER_SIZE 300

int main(void)
{
	uint8_t inputBuffer[BUFFER_SIZE];
	int32_t bytesRead, returnCode, fd;
    
    /* Create named fifo. -1 means already exists so no action if already exists */
    if ( (returnCode = mknod(FIFO_NAME, S_IFIFO | 0666, 0) ) < -1  )
    {
        printf("Error creating named fifo: %d\n", returnCode);
        exit(1);
    }
    
    /* Open named fifo. Blocks until other process opens it */
	printf("Espero a Escritores...\n");
	if ( (fd = open(FIFO_NAME, O_RDONLY) ) < 0 )
    {
        printf("Error opening named fifo file: %d\n", fd);
        exit(1);
    }
    
    /* open syscalls returned without error -> other process attached to named fifo */
	printf("Tenemos un Escritor\n");

    /* Loop until read syscall returns a value <= 0 */
	do
	{
        /* read data into local buffer */
		if ((bytesRead = read(fd, inputBuffer, BUFFER_SIZE)) == -1)
        {
			
			perror("Leer");
        }
        else
		{
			for(int i=0; i<bytesRead; i++)
			{			
				if(inputBuffer[i] == 'D') 
				{ 							
			
				inputBuffer[bytesRead] = '\0';
				printf("%s \n", inputBuffer);
				FILE *f_log = fopen("log.txt", "wt");
				fputs(inputBuffer, f_log);			
				fclose(f_log);
				}
				if(inputBuffer[i] == 'S') 
				{ 							
			
				inputBuffer[bytesRead] = '\0';
				printf("%s \n", inputBuffer);
				FILE *signals = fopen("signals.txt", "wt");
				fputs(inputBuffer, signals);			
				fclose(signals);
				}
		       }
		}
	}	
	while (bytesRead > 0);

	return 0;
}