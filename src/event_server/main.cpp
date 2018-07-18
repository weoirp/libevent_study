#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <event.h>
#include <evhttp.h>
#include <signal.h>
#include <errno.h>


void signal_handler(int sig)
{
	switch(sig)
	{
	case SIGTERM:
	case SIGHUP:
	case SIGQUIT:
	case SIGINT:
		event_loopbreak();
		break;
	}
}

static void fifo_read(evutil_socket_t fd, short event, void *arg)
{
	char buff[255];
	int len;
	struct event *ev = (struct event *)arg;

	event_add(ev, NULL);
	fprintf(stderr, "fifo_read called with fd: %d, event: %d, arg: %p\n", 
		(int)fd, event, arg);
	len = read(fd, buff, sizeof(buff) - 1);

	if (len == -1)
	{
		perror("read");
		return;
	}
	else if (len == 0)
	{
		fprintf(stderr, "Connection closed\n");
		return;
	}

	buff[len] = '\0';
	fprintf(stdout, "Read: %s\n", buff);
}

int main(int argc, char **argv)
{
	signal(SIGTERM, signal_handler);
	signal(SIGHUP, signal_handler);
	signal(SIGQUIT, signal_handler);
	signal(SIGINT, signal_handler);


	short http_port = 8081;
	const char *http_addr = "0.0.0.0";
	int http_timeout = 120;
	struct evhttp *http_server = NULL;

	event_init();
	http_server = evhttp_start(http_addr, http_port);
	evhttp_set_timeout(http_server, http_timeout);
	evhttp_set_gencb(http_server, http_handler, NULL);

	fprintf(stderr, "Server started on port %d\n", http_port);
	event_dispatch();

	evhttp_free(http_server);

	fprintf(stderr, "exit!!!\n");

	return 0;
}
