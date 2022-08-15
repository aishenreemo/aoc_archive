#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include "../../include/load_file.h"
#include "../../include/md5.h"

// https://www.devcoons.com/convert-uint8_t-array-to-char-hex-representation-in-c/
static char hex[] = "0123456789ABCDEF";
void convert_to_hex_str(char *str, uint8_t *val) {
	for (size_t i = 0; i < 16; i++) {
		str[(i * 2) + 0] = hex[((val[i] & 0xF0) >> 4)];
		str[(i * 2) + 1] = hex[((val[i] & 0x0F) >> 0)];
	}
}

int main() {
	char *input = load_file("2015/day_04/input.txt");
	char *line = strtok(input, "\n");

	uint32_t i;
	for (i = 0; i < UINT32_MAX; i++) {
		char *str = malloc(20);
		snprintf(str, 20, "%s%d", line, i);

		char *md5_string = malloc(32);
		uint8_t *md5_result = md5String(str);


		convert_to_hex_str(md5_string, md5_result);
		printf("%s -> %s\n", str, md5_string);

		int starts_with = strncmp("000000", md5_string, 6);

		free(str);
		free(md5_string);
		free(md5_result);

		if (starts_with == 0) break;
	}

	printf("%d\n", i);

	free(input);
	return 0;
}
