#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/ioctl.h>
#include "../ioctl.h"

int main() {
	int fd;
	int set_data = 2023;
	int get_data = -1; // If success, it will be changed to 2023.

	if((fd = open("/dev/scull0", O_RDWR)) < 0) {
		printf("Cannot open /dev/scull0.\n");

		return -ENXIO;
	}
	
	// User Space -> Kernel Space
	if(ioctl(fd, SCULL_IOCSQUANTUM, &set_data) < 0) {
		printf("Error in SCULL_IOSTQUANTUM statement.\n");

		return -EPERM;
	}

	// Kernel Space -> User Space
	if(ioctl(fd, SCULL_IOCGQUANTUM, &get_data) < 0) {
		printf("Error in SCULL_IOCGQUANTUM statement.\n");

		return -EPERM;
	}
	
	printf("get_data : %d\n", get_data);

	if(close(fd) != 0) {
		printf("Cannot close.\n");
		
		return -ENXIO;
	}

	return 0;
}

