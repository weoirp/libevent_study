#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <event.h>
#include <event2/util.h>
#include <event2/listener.h>

const char MESSAGE[] = "HELLO WORLD\n";
const int PORT = 8080;

static void listener_cb(struct evconnlistener *, evutil_socket_t, struct sockaddr *, int socklen, void *);
static void conn_readcb(struct bufferevent *, void *);
static void conn_writecb(struct bufferevent *, void *);
static void conn_eventcb(struct bufferevent *, short events, void *);
static void signal_cb(evutil_socket_t, short, void *);


int main(int argc, char **argv)
{
	struct event_base *base = event_base_new();
	if (!base)
	{
		fprintf(stderr, "Could not initialize libevent!\n");
		return 1;
	}

	struct sockaddr_in sin;
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_port = htons(PORT);

	struct evconnlistener *listener = evconnlistener_new_bind(base, listener_cb, (void *)base,
		LEV_OPT_REUSEABLE | LEV_OPT_CLOSE_ON_FREE, -1,
		(struct sockaddr*)&sin, sizeof(sin));

	if (!listener)
	{
		fprintf(stderr, "Could not create a listener!\n");
		return 1;
	}

	struct event *signal_event;
	signal_event = evsignal_new(base, SIGINT, signal_cb, (void *)base);
	if (!signal || event_add(signal_event, NULL) < 0)
	{
		fprintf(stderr, "Could not create/add a signal event!\n");
		return 1;
	}

	event_base_dispatch(base);
	evconnlistener_free(listener);
	event_free(signal_event);
	event_base_free(base);

	printf("done!!!\n");
	return 0;
}

static void listener_cb(struct evconnlistener *listener, evutil_socket_t fd, 
	struct sockaddr *sa, int socklen, void *user_data)
{
	struct event_base *base = (event_base *)user_data;
	struct bufferevent *bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);

	if (!bev)
	{
		fprintf(stderr, "Error constructing bufferevent\n");
		return;
	}
	bufferevent_setcb(bev, conn_readcb, conn_writecb, conn_eventcb, NULL);
	bufferevent_enable(bev, EV_WRITE | EV_READ);

	printf("sb. connected!!!\n");
	// bufferevent_write(bev, MESSAGE, strlen(MESSAGE));
}

static void conn_readcb(struct bufferevent *bev, void *user_data)
{
	printf("read ---------------- cb\n");
	struct evbuffer *input = bufferevent_get_input(bev);
	char buf[1024];
	int n;
	while((n = evbuffer_remove(input, buf, sizeof(buf))) > 0) {
	    fwrite(buf, 1, n, stdout);
	}
	bufferevent_write(bev, MESSAGE, strlen(MESSAGE));
}

static void conn_writecb(struct bufferevent *bev, void *user_data)
{
	printf("write --------------- cb\n");
}

static void conn_eventcb(struct bufferevent *bev, short events, void *user_data)
{
	printf("event --------------- cb\n");
	if (events & BEV_EVENT_CONNECTED)
	{
		printf("connected event.....\n");
	}
	else
	{
		if (events & BEV_EVENT_EOF)
		{
			printf("Connection closed. \n");
		}
		else if (events & BEV_EVENT_ERROR)
		{
			printf("Got an error on the connection: %s\n", strerror(errno));
		}
		bufferevent_free(bev);		
	} 

}

static void signal_cb(evutil_socket_t sig,  short events, void *user_data)
{
	struct event_base *base = (struct event_base *)user_data;
	struct timeval delay = {2, 0};

	printf("Caught an interrupt signal; exiting cleanly in two seconds.\n");
	event_base_loopexit(base, &delay);
}