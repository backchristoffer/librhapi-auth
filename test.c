#include <stdio.h>
#include <stdlib.h>
#include "rhapi_auth.h"
#include <json-c/json.h>


int main() {
    // 1. Get the Bearer Token
    char *token = rhapi_get_token();
    if (!token) {
        printf("Failed to get token.\n");
        return 1;
    }

    // 2. Use that token to get some data!
    // This is the endpoint for Red Hat Insights Inventory
    const char *url = "https://console.redhat.com/api/inventory/v1/hosts";
    printf("Fetching data from: %s\n", url);

    char *data = rhapi_fetch_data(url, token);

    if (data) {
    	struct json_object *pretty_json = json_tokener_parse(data);

	printf("--- PRETTY PRINTING ---\n%s\n", json_object_to_json_string_ext(pretty_json, JSON_C_TO_STRING_PRETTY));

	json_object_put(pretty_json);
	free(data);
    }

    // Cleanup the token
    free(token);
    return 0;
}
