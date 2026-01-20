//test generated with gemini
#include <stdio.h>
#include <stdlib.h>
#include "rhapi_auth.h" // We are importing your custom tool!

int main() {
    printf("--- Red Hat Auth Module Test ---\n");

    // 1. Call your new function
    char *token = rhapi_get_token();

    // 2. Check if it worked
    if (token != NULL) {
        printf("SUCCESS!\n");
        printf("Your Access Token starts with: %.20s...\n", token);
        
        // 3. IMPORTANT: Since your module used strdup, you MUST free it here
        free(token);
    } else {
        printf("FAILURE: Could not get token. Check your .env and internet.\n");
    }

    return 0;
}
