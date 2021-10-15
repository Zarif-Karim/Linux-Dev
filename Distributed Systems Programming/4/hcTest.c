#include"http-client-c.h"

int main( int argc, char* argv[])
{
	struct http_response* r = http_get(argv[1],NULL);
	printf("BODY:\n\n%s\n\n", r->body);

	return 0;
}
