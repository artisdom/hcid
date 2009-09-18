#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <syslog.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAXFILE 65535

int main(int argc, char *argv[])
{
	pid_t p;
	int i, fd, len;
	char *buf = "This is a daemon\n";

	len = strlen(buf);
	p = fork();
	if (p < 0) {
		fprintf(stderr, "fork error:%s (%d)\n",
				strerror(errno), errno);
	} else if (p > 0)
		exit(0);
	setsid();
	chdir("/");
	umask(0);
	for (i = 0; i < MAXFILE; i++)
		close(i);

	while (1) {
		if ( (fd = open("/tmp/daemon.log", O_CREAT|O_WRONLY|O_APPEND, 0600)) < 0) {
			fprintf(stderr, "open log error:%s (%d)\n",
					strerror(errno), errno);
		}
		write(fd, buf, len + 1);
		close(fd);
		sleep(3);
	}

}
