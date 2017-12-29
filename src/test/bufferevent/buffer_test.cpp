#include <event2/event.h>
#include <event2/bufferevent.h>
#include <sys/socket.h>
#include <string.h>
#include <signal.h>


static void signal_cb(evutil_socket_t, short, void *);

void eventcb(struct bufferevent *bev, short events, void *ptr)
{
    if (events & BEV_EVENT_CONNECTED) {
    	printf("CCConnected!!!\n");
         /* We're connected to 127.0.0.1:8080.   Ordinarily we'd do
            something here, like start reading or writing. */
    } else if (events & BEV_EVENT_ERROR) {
         /* An error occured while connecting. */
    }
}

int main()
{
	struct event_base *base;
    struct bufferevent *bev;
    struct sockaddr_in sin;

    base = event_base_new();

    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = htonl(0x7f000001); /* 127.0.0.1 */
    sin.sin_port = htons(8080); /* Port 8080 */

    bev = bufferevent_socket_new(base, -1, BEV_OPT_CLOSE_ON_FREE);

    bufferevent_setcb(bev, NULL, NULL, eventcb, NULL);

    if (bufferevent_socket_connect(bev,
        (struct sockaddr *)&sin, sizeof(sin)) < 0) {
        /* Error starting connection */
    	printf("connect failed!!!\n");
        bufferevent_free(bev);
        return -1;
    }

    struct event *signal_event;
	signal_event = evsignal_new(base, SIGINT, signal_cb, (void *)base);
	if (!signal || event_add(signal_event, NULL) < 0)
	{
		fprintf(stderr, "Could not create/add a signal event!\n");
		return 1;
	}

    event_base_dispatch(base);
    bufferevent_free(bev);
    event_free(signal_event);
	event_base_free(base);
	printf("done!!!\n");
    return 0;
}

static void signal_cb(evutil_socket_t sig,  short events, void *user_data)
{
	struct event_base *base = (struct event_base *)user_data;
	struct timeval delay = {2, 0};

	printf("Caught an interrupt signal; exiting cleanly in two seconds.\n");
	event_base_loopexit(base, &delay);
}