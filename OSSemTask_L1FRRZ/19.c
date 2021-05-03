#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main()
{
    int child;

    mkfifo("fifo",S_IWUSR | S_IRUSR);

    child = fork();

    if (child == 0)
    {
	int fd = open("be",O_RDONLY);

	char string[6];
	int length = read(fd,string,sizeof(string)-1);
	string[length] = '\0';
	close(fd);

	int p = open("fifo",O_WRONLY);

	write(p,string,sizeof(string));
	close(p);
	return 0;
    }
    else if (child < 0) { perror("Error\n\n"); return -1; } else
    {
	int p = open("fifo",O_RDONLY);

	char input[6];
	int length = read(p,input,sizeof(input)-1);
	close(p);
	input[length] = '\0';

	int i=0;
	int haromszog[3];

	printf("\nOlvasott értékek: ");
	while (i<=4)
	{
	    haromszog[i/2] = atoi(&input[i]);
	    printf("%d ",haromszog[i/2]);
	    i += 2;
	}

	int fd = open("ki",O_WRONLY);

	write(fd,input,sizeof(input));
	if ((haromszog[0]<haromszog[1]+haromszog[2]) && (haromszog[1]<haromszog[2]+haromszog[0]) && (haromszog[2]<haromszog[1]+haromszog[0]))
	{
	    printf("\n\nSzerkesztheto haromszog!\n\n");
	    write(fd," 1",2);
	}
	else
	{
	    printf("\n\nNem szerkesztheto haromszog!\n\n");
	    write(fd," 0",2);
	}
	write(fd,"\n",1);
	close(fd);
    }
    unlink("fifo");
    return 0;
}