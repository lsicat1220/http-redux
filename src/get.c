#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "../include/get.h"
#include "../include/parsing.h"
#include "../include/map.h"
#include "../include/utils.h"

#include "../include/defs.h"

enum contentType {
	HTML,
	CSS,
	JS,
	TXT,
	JSON,
	ICO,
	PNG,
	JPG,
	UNSUPPORTED,
	FORBIDDEN,
};

void WriteContent(int socket, FILE* file, long size, char* buffer, int buf_size) {
	rewind(file);
	long remaining_bytes = size - ftell(file);
	long sent_bytes;
	while (remaining_bytes > BUF_SIZE) {
		printf("Remaining bytes: %ld\n", remaining_bytes);
		fread(buffer, 1, BUF_SIZE, file);
		write(socket, buffer, BUF_SIZE);
		remaining_bytes = size - ftell(file);
	} 
	fread(buffer, 1, remaining_bytes, file);
	write(socket, buffer, remaining_bytes);
	return;
}

int CleanPath(char* file, size_t max_len) {
	char working_dir[128];
	char true_path[128];
	getcwd(working_dir, 128);
	printf("Working dir: %s\n", working_dir);
	printf("Path: %s\n", file);
	realpath(file, true_path);
	printf("True path: %s\n", true_path);
	if (strncmp(working_dir, true_path, strlen(true_path)) == 0) {
		memcpy(file, "index.html", 11);
	} else if (strncmp(working_dir, true_path, strlen(working_dir)) != 0) {
		return -1;
	} else if (strlen(true_path) > max_len) {
		return -1;
	} else {
		memcpy(file, true_path, strlen(true_path));
	}
	return 0;
}

int RespondGet(RequestLine* req, MapState* map, int socket) {
	char path[PATH_SIZE] = ".";
	char buffer[BUF_SIZE];
	char* message;
	int errcode;
	enum contentType type;
	printf("Path in request: %.*s\n", req->path.len, req->path.start);
	if (req->path.len >= PATH_SIZE - 1) {
		fputs("ERROR: Path size too long\n", stderr);
		errcode = 400;
		goto error;
	}
	memcpy(path + 1, req->path.start, req->path.len);
	path[req->path.len + 1] = 0;
	if (CleanPath(path, PATH_SIZE)) {
		fputs("ERROR: File not found\n", stderr);
		errcode = 404;
		goto error;
	}
	FILE* file = fopen(path, "r");
	if (file == NULL) {
		fputs("ERROR: Opening file failed\n", stderr);
		errcode = 404;
		goto error;
	}
	long size = GetFileSize(file);
	if (size < 0) {
		fputs("ERROR: failed to get file size\n", stderr);
		errcode = 404;
		goto error;
	}
	message = "HTTP/1.1 200 OK\r\n";
	write(socket, message, strlen(message));
	snprintf(buffer, BUF_SIZE, "Content-length: %ld\r\n\r\n", size);
	write(socket, buffer, strlen(buffer));
	WriteContent(socket, file, size, buffer, BUF_SIZE);
	return 0;

error:
	switch(errcode) {
		case 400:
			message = "HTTP/1.1 400 Bad Request\r\n\r\n";
			break;
		case 404:
			message = "HTTP/1.1 404 Not Found\r\n\r\n";
			break;
	}
	write(socket, message, strlen(message));
	return -1;
}

