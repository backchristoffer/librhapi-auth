#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <string.h>

struct Buffer {
	char *data;
	size_t length;
};

size_t write_callback(void *ptr, size_t size, size_t nmemb, struct Buffer *buffer) {
	size_t new_length = size * nmemb;
	buffer->data = realloc(buffer->data, buffer->length + new_length + 1);
	memcpy(buffer->data + buffer->length, ptr, new_length);
	buffer->length += new_length;
	buffer->data[buffer->length] = '\0';

	return new_length;
}


int main() {
	printf("DEBUG: START\n");
	CURL *curl = curl_easy_init();
	
	if (!curl) {
		fprintf(stderr, "curl init failed\n");
		return 1;
	}
	
	struct Buffer response = { .data = malloc(1), .length = 0 };

	curl_easy_setopt(curl, CURLOPT_URL, "https://sso.redhat.com/auth/realms/redhat-external/protocol/openid-connect/token");
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
	CURLcode result = curl_easy_perform(curl);

	if (result != CURLE_OK) {
		fprintf(stderr, "request issue, %s\n", curl_easy_strerror(result));
		return 1;	
	}

	curl_easy_cleanup(curl);

	free(response.data);
	printf("DEBUG: END\n");
	return 0;
}
