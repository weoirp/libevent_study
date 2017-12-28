#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

#include <event.h>

int main(int argc, char **argv)
{
	struct evbuffer *buff = NULL;
	char c, c2[3] = {0};

	buff = evbuffer_new();
	if (buff == NULL)
	{
		return -1;
	}

	evbuffer_add(buff, "1", 1);
	evbuffer_add(buff, "2", 1);
	evbuffer_add(buff, "3", 1);
	evbuffer_add_printf(buff, "%d%d", 4, 5);
	printf("evbuffer length: %lu\n", EVBUFFER_LENGTH(buff));

	evbuffer_remove(buff, &c, sizeof(char));
	assert(c == '1');
	evbuffer_remove(buff, &c, sizeof(char));
	assert(c == '2');
	evbuffer_remove(buff, &c, sizeof(char));
	assert(c == '3');

	printf("evbuffer length: %lu\n", EVBUFFER_LENGTH(buff));

	evbuffer_remove(buff, c2, 2);
	assert(strcmp(c2, "45") == 0);

	printf("evbuffer length: %lu\n", EVBUFFER_LENGTH(buff));

	evbuffer_add(buff, "test\r\n", 6);
	printf("evbuffer length: %lu\n", EVBUFFER_LENGTH(buff));
	char *line = evbuffer_readline(buff);
	assert(strcmp(line, "test") == 0);
	free(line);
	line = NULL;

	evbuffer_free(buff);
	buff = NULL;
	printf("ok\n");

	return 0;
}