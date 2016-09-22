
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <sys/time.h>
#include <time.h>

int open_port(const char *devname);
int switch_relay(int relaynum, int state);
void msleep(int msec);
int switch_all_relays(int state);
int close_relay(int relaynum, time_t seconds);

int main(int argc, char **argv)
{
	int fd, nn, ss, ii, jj;
	char buf[1024];

	if(argv[1] && !strcmp(argv[1], "all-off"))
	{
		switch_all_relays(0);
		return 0;
	}
	else if(argv[1] && argv[2] && argv[3] && !strcmp(argv[1], "close"))
	{
		nn = atol(argv[2]);
		ss = atol(argv[3]);
		if((nn < 1) || (nn > 16))
		{
			fprintf(stderr, "relay number %d out of range\n", nn);
			return 1;

		}

		if(ss < 1 || ss > 900) /* outside 1 to 15 minutes */
		{
			fprintf(stderr, "relay close time %d seconds invalid\n", ss);
			return 1;
		}
		close_relay(nn, ss);
	}

	return 0;

#if 0
	switch_all_relays(0); msleep(250);
//	switch_all_relays(1); msleep(250);


	for(ii=0 ; ii<1 ; ii++)
	{
		for(jj=1 ; jj<=16 ; jj++)
		{
//			switch_relay(jj, ii%2);
//			switch_relay(jj, 1);
//			close_relay(jj, 2);
			msleep(250);
		}
	}
#endif

	return 0;
}

int open_port(const char *devname)
{
	int fd;
	struct termios options;

	fd = open(devname, O_RDWR|O_NOCTTY|O_NDELAY);
	if(fd == -1)
	{
		fprintf(stderr, "open_port: Unable to open serial device %s\n%11serror: %s\n", devname, "", strerror(errno));
		return fd;
	}
//	fcntl(fd, F_SETFL, FNDELAY);
	fcntl(fd, F_SETFL, 0);
	
	/* get current port options  */
	tcgetattr(fd, &options);

#if 0
	/* set 9600 baud */
	cfsetispeed(&options, B9600);
	cfsetospeed(&options, B9600);
	options.c_cflag |= (CLOCAL|CREAD);

	/* set 8N1 */
	options.c_cflag &= ~PARENB;
	options.c_cflag &= ~CSTOPB;
	options.c_cflag &= ~CSIZE;
	options.c_cflag |= CS8;

	/* hardware flow control */
//	options.c_cflag |= CRTSCTS; /* Also called CNEW_RTSCTS */
	/* disable hardware flow control */
	options.c_cflag &= ~CRTSCTS;	
#else
	options.c_iflag = 0;
	options.c_oflag = 0;
	options.c_cflag = CS8|CREAD|CLOCAL;
	options.c_lflag = 0;
	options.c_cc[VMIN] = 1;
	options.c_cc[VTIME] = 5;
	cfsetispeed(&options, B9600);
	cfsetospeed(&options, B9600);
#endif

	/* set new port options */
	tcsetattr(fd, TCSANOW, &options);

	return fd;
}

int switch_relay(int relaynum, int state)
{
	int fd, nn;
	char buf[1024], cmd[32];

	if(state) { switch_all_relays(0); }

	if(relaynum > 16 || relaynum < 1) { return -1; }
	sprintf(cmd, "%.2d%c//\r", relaynum, state?'+':'-');
	printf("%s relay %2d\n", state?"closing":"opening", relaynum);

	fd = open_port("/dev/ttyUSB0");
	nn = write(fd, cmd, 6);
	nn = read(fd, buf, 1024); // printf("read %d bytes\n", nn);
	close(fd);
	return 0;
}

int switch_all_relays(int state)
{
	int fd, nn;
	char buf[1024], cmd[32], cst;
	
	fd = open_port("/dev/ttyUSB0");
	if(state)
	{
		nn = write(fd, "on//\r", 5);
	}
	else
	{
		nn = write(fd, "off//\r", 6);
	}
	nn = read(fd, buf, 1024);
	close(fd);

	return 0;	
}

void msleep(int msec)
{
	int ss, mm;
	struct timespec ts, tr;

	mm = msec % 1000;
	ss = ( msec - mm ) / 1000;

	ts.tv_sec  = ss;
	ts.tv_nsec = mm * 1000000;
	
	while(nanosleep(&ts, &tr) != 0);
}

int close_relay(int relaynum, time_t seconds)
{
	printf("closing relay %d for %d seconds\n", relaynum, (int)seconds);
	switch_relay(relaynum, 1);
	if(seconds <= 0) { return 1; }
	msleep(seconds * 1000);
	switch_relay(relaynum, 0);
	return 1;
}

