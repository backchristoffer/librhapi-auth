#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <string.h>
#include <json-c/json.h>


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
	CURL *curl = curl_easy_init();
	
	if (!curl) {
		fprintf(stderr, "curl init failed\n");
		return 1;
	}
	
	
	char *offline_token = getenv("RH_OFFLINE_TOKEN");
	if (offline_token == NULL) {
		fprintf(stderr, "missing env RH_OFFLINE_TOKEN\n");
		return 1;
	}

	char post_fields[4096];
	snprintf(post_fields, sizeof(post_fields), "grant_type=refresh_token&client_id=rhsm-api&refresh_token=%s", offline_token);
	
	struct Buffer response = { .data = malloc(1), .length = 0 };
	


	curl_easy_setopt(curl, CURLOPT_URL, "https://sso.redhat.com/auth/realms/redhat-external/protocol/openid-connect/token");
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_fields);
	CURLcode result = curl_easy_perform(curl);

	if (result != CURLE_OK) {
		fprintf(stderr, "request issue, %s\n", curl_easy_strerror(result));
		return 1;	
	}else{
		struct json_object *parsed_json = json_tokener_parse(response.data);
		struct json_object *access_token;

		if (json_object_object_get_ex(parsed_json, "access_token", &access_token)) {
			printf("%s\n", json_object_get_string(access_token));
			
		}else{
			fprintf(stderr, "Error: Access token not found\n");
		}
		json_object_put(parsed_json);
	}

	curl_easy_cleanup(curl);

	free(response.data);
	return 0;
}
