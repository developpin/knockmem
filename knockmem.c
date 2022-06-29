#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>

int readmem(size_t* len, char* data, off_t* page_offset)
{

    return 0;
}


int main(int argc, char *argv[])
{
char *access_type;
off_t offset;
size_t len;
int err;
unsigned long change_data;

	/*
	 for (int i = 0; i < argc; i++)
	{
		printf("argv[%d] = %s \n",i,argv[i]);
	}
	*/
	access_type = argv[1];
	if (strcmp(access_type, "-h") == 0) 
    {
     printf("Usage: man knockmem \n"
     		"SYNOPSIS: sudo knockmem <flag> <phys_addr> <size> <change> \n"
     		"[flag] \n"
             "      -r   read memory data\n" 
             "      -w   write memory data\n"
             "      -h   help\n"
             "[phys_addr] Start point address \n"
             "[size]      Number of bytes to operate\n"
             "{change}    New byte value\n");
     return 0;
    }	
	
	if (argc < 4)
	{
		printf("Wrong command: %s  SYNOPSIS: sudo knockmem <-flag> <phys_addr> <size>  \n", argv[0]);
		return -1;
	}
	else if (argc > 5)
	{
		printf("Wrong command: %s  SYNOPSIS: sudo knockmem <-flag> <phys_addr> <size> <change>\n", argv[0]);
	  	return -1;
	} 
	
	offset = strtoul(argv[2], NULL, 0);
	len = strtoul(argv[3], NULL, 0);
	size_t pagesize = sysconf(_SC_PAGE_SIZE);
	off_t page_base = (offset / pagesize) * pagesize;
	off_t page_offset = offset - page_base;	
	
	int fd = open("/dev/mem", O_RDWR | O_SYNC);
	unsigned char *data = mmap(NULL, page_offset + len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, page_base);
	
	if (data == MAP_FAILED)
	{
		perror("Can't map memory \n");
		return -1;
	}
	
    
	if (strcmp(access_type, "-r") == 0) 
	{
		/*Read memory */
		size_t i;
		for (i = 0; i < len; i++)
		{
			printf("%x ", (int)data[page_offset + i]);
		}
		printf("\n");
		return 0;
	} 
	else if (strcmp(access_type, "-w") == 0)
	{	
		/*Write data in memory */
		
		if (argc != 5)
		{
			printf("Wrong command: %s  SYNOPSIS: sudo knockmem -w <phys_addr> <size> <change> \n", argv[0]);
			return -1;
		}
		change_data = strtoul(argv[4], NULL, 0);
		size_t i;
		for (i = 0; i < len; i++)
		{
			data[page_offset+i] = change_data;
		}
		printf("Data was writed from  %p to %p Data: %d \n", (char*)offset, (char*)(offset+len-1) ,data[page_offset+len-1]);
		return 0;
		
	}
	else 
	{
		printf("ERROR access_type: %s \n", access_type);
		printf("Wrong flag: %s Use (w -write) or (r -read)\n", argv[0]);	
		return -1;
	}

	

	

	
	err=munmap(data, pagesize + len);
	if ( err == -1) 
	{
		printf("ERROR, unmap memory CODE:  %d", err);
	}
	close(fd);
	return 0;

}




