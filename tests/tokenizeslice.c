#include <stdio.h>
#include "../include/parsing.h"
#include "../include/slice.h"
#include <string.h>

#define BUF_CAPACITY 256
int main() {
	char tests[7][128] = {
		"Test 1 Side 1\r\nTest 1 Side 2",
		"Test 2 Side 1\r\nTest 2 Side 2\r\nTest 2 Side 3\r\n",
		"Test 3",
		"Test 4\r\n",
		"Test 5\r\n\r\n",
		"\r\n",
		""
	};
	Slice currSlice;
	for (int i = 0; i < 7; i++) {
		bufState buf = {.buffer = NULL, .capacity = BUF_CAPACITY, .unprocessed_offset = 0, .used_bytes = 0};
		int j = 1;
		printf("Test %d:\n", i + 1);
		buf.buffer = tests[i];
		buf.used_bytes = strlen(tests[i]);
		int tokenStatus = TokenizeSlice(&buf, &currSlice, "\r\n", 2);
		while (tokenStatus == 0) {
			printf("Slice %d: ", j);
			fwrite(currSlice.start, 1, currSlice.len, stdout);
			printf("\n");
			j++;
			tokenStatus = TokenizeSlice(&buf, &currSlice, "\r\n", 2);
		}
		printf("\n");
	}
}
