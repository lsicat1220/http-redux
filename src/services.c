#include <stdio.h>
#include <unistd.h>
#include "../include/receiving.h"
#include "../include/services.h"
#include "../include/slice.h"
#include "../include/map.h"
#include "../include/parsing.h"
#define BUFFER_SIZE 8192

void HandleConnection(int clientSocket) {
	int keepAlive = 1;
	printf("\n\n----- CONNECTION ACCEPTED -----\n\n");
	char readBuffer[BUFFER_SIZE];
	char responseBuffer[BUFFER_SIZE];
	while (keepAlive) {
		bufState buffer_state = {readBuffer, BUFFER_SIZE, 0, 0};
		MapNode list[128];
		MapState map = {.list = list, .len = 128};
		RequestLine req;
		Slice request;
		int readStatus = ReadUntil(clientSocket, &buffer_state, "\r\n\r\n", 4);
		if (readStatus < 0) {
			HandleReadError(readStatus);
			break;
		}
		// by now the buffer should have all of the headers
		if (TokenizeSlice(&buffer_state, &request, "\r\n", 2)) {
			break;
		}
		if (ParseRequestLine(&req, &request)) {
			break;
		};	
		printf("Request: \n");
		fwrite(request.start, request.len, sizeof(char), stdout);
		
		keepAlive = 0;
	}
	snprintf(responseBuffer, BUFFER_SIZE, "HTTP/1.0 200 OK\r\n\r\n");
	write(clientSocket, responseBuffer, 19);
	printf("\n\n----- CONNECTION CLOSED -----\n\n");
	close(clientSocket);
	snprintf(responseBuffer, BUFFER_SIZE, "HTTP/1.0 200 OK\r\n\r\n");
	write(clientSocket, responseBuffer, 19);
	close(clientSocket);
}

int ParseAllHeaders(bufState* buf, MapState* map) {
	Slice currSlice;
	if (TokenizeSlice(buf, &currSlice, "\r\n", 2)) {
		fprintf(stderr, "ERROR: Creating slice failed");
		return -1;
	}
	while (currSlice.len != 0) {

	}	
	return 0;
}
