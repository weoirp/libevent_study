#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <event.h>
#include <evhttp.h>
#include <signal.h>

#define MYHTTPD_SIGNATURE   "myhttpd v 0.0.1"

void http_handler(struct evhttp_request *req, void *arg)
{
	char output[2048] = "\0";
	char tmp[1024];

	const char *uri;
	uri = evhttp_request_uri(req);
	sprintf(tmp, "uri=%s\n", uri);
	strcat(output, tmp);

	sprintf(tmp, "uri=%s\n", req->uri);
	strcat(output, tmp);

	char *decoded_uri;
	decoded_uri = evhttp_decode_uri(uri);
	sprintf(tmp, "decoded_uri=%s\n", decoded_uri);
	strcat(output, tmp);

	struct evkeyvalq params;
	evhttp_parse_query(decoded_uri, &params);

	sprintf(tmp, "q=%s\n", evhttp_find_header(&params, "q"));
	strcat(output, tmp);

	sprintf(tmp, "s=%s\n", evhttp_find_header(&params, "s"));
	strcat(output, tmp);

	free(decoded_uri);

	evhttp_add_header(req->output_headers, "Server", MYHTTPD_SIGNATURE);
	evhttp_add_header(req->output_headers, "Content-Type", "text/plain; charset=UTF-8");
	evhttp_add_header(req->output_headers, "Connection", "close");

	struct evbuffer *buf;
	buf = evbuffer_new();
	evbuffer_add_printf(buf, "It works!\n%s\n", output);
	evhttp_send_reply(req, HTTP_OK, "OK", buf);
	evbuffer_free(buf);
}

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
