#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <string.h>
#include "../include/receiving.h"
#include "../include/utils.h"

#define NODEBUG

// This function edits a buf_state struct, which includes the buffer itself,
// the total amount of the bytes read, and the length of the most recent section received.

int ReadUntil(int fd, bufState* buf_state, char* target, int target_size) {
	char* buffer_start = buf_state->buffer;
	int* used_bytes = &(buf_state->used_bytes);
	const size_t capacity = buf_state->capacity;
	char* search_start = buffer_start + buf_state->unprocessed_offset;
	char* occurrence = TheMemmem(target, search_start, target_size, *used_bytes - (search_start - buffer_start));

	while (!occurrence && *used_bytes < capacity) {
		int newly_read_bytes = read(fd, buffer_start + *used_bytes, capacity - *used_bytes);

		if (newly_read_bytes < 0) {
			return ERR_SOCKET_FAILED; 
		} 
		if (newly_read_bytes == 0) {
			return *used_bytes ? ERR_READ_INTERRUPTED : ERR_NO_DATA;
		}

		*used_bytes += newly_read_bytes;

		// Start reading a few bytes back to catch cases where the target is cut off between reads
		int memmem_offset = target_size - 1;
		if (search_start - buffer_start < target_size - 1) {
			memmem_offset = search_start - buffer_start;
		} 		
		occurrence = TheMemmem(target, search_start - memmem_offset, target_size, newly_read_bytes + memmem_offset);
		search_start = buffer_start + *used_bytes;
	} 
	return (!occurrence) ? ERR_BUFFER_FULL : 0;
}

void HandleReadError(enum receiveStatus status) {
	if (status == -1) {
		perror("ERROR");
	} else if (status == -2) {
		fprintf(stderr, "ERROR: Request interrupted or ended prematurely\n");
	} else if (status == -3) {
		fprintf(stderr,"ERROR: Read ended with no data\n");
	} else if (status == -4) {
		fprintf(stderr, "ERROR: Ran out of space with incomplete headers\n");
	} else {
		fprintf(stderr, "ERROR: Unknown error");
	}
}


// This is a simple function that takes data stored in bufState to move the most recent section to a new string
int MoveSection(bufState* bufState, void* dest, size_t dest_len) {
	int sectionLen = bufState->unprocessed_offset - 1;
	if (sectionLen > dest_len) {
		fputs("ERROR: Attempt to move data to a smaller buffer", stderr);
		return 0;
	}
	memcpy(dest, bufState->buffer, sectionLen);
	return sectionLen;
}

// This function shifts the buffer such that the beginning of the buffer is now 
// the position refered to by bufState.offset
int CompactBuffer(bufState* buf) {
	int len = buf->used_bytes -= buf->unprocessed_offset;
	memmove(buf->buffer, buf->buffer + buf->unprocessed_offset, len);
	buf->used_bytes = len;
	buf->unprocessed_offset = 0;	
	memset(buf->buffer + len, 0, buf->capacity - len);
	return 0;
}
