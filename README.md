# librhapi-auth

A modular C library designed for authenticated interaction with Red Hat REST APIs. This module handles OAuth2 token retrieval via Red Hat SSO and provides a generic interface for executing authenticated HTTP GET requests.

## Architecture

The library is split into two primary functional components:

1. **Authentication (rhapi_get_token)**: Handles the POST request to the Red Hat SSO OIDC endpoint to exchange an offline token for a short-lived access token.
2. **Data Retrieval (rhapi_fetch_data)**: A generic wrapper around libcurl that injects the required Authorization: Bearer header for interacting with protected API endpoints.

Both functions utilize a shared internal write_callback to manage dynamic memory allocation for HTTP response buffers.

---

## Function Reference

### char* rhapi_get_token(void)
Retrieves a Bearer Access Token using the RH_OFFLINE_TOKEN environment variable.
* **Returns**: A dynamically allocated string containing the access token. 
* **Memory Management**: The caller must free() the returned pointer.
* **Dependencies**: Requires RH_OFFLINE_TOKEN to be set in the environment.

### char* rhapi_fetch_data(const char *url, const char *token)
Executes an authenticated HTTP GET request to a specified Red Hat API endpoint.
* **Parameters**:
    * url: The full URI of the REST endpoint.
    * token: A valid Bearer token.
* **Returns**: A dynamically allocated string containing the raw JSON response.
* **Memory Management**: The caller must free() the returned pointer.

---

## Building and Dependencies

### Prerequisites
* libcurl
* json-c
* pkg-config (recommended for dependency resolution)

### Build Instructions
The project includes a Makefile to handle object file compilation and linking.

```bash
# Compile library and test suite
make

# Remove object files and binaries
make clean
```

---

## Project Integration

To integrate this module into an existing C project:

1. Include rhapi_auth.h in your source code.
2. Link rhapi_auth.c during your build process.
3. Ensure your linker is provided with the necessary flags for libcurl and json-c.

**Manual Compilation Example**:
```bash
gcc -o my_app main.c rhapi_auth.c $(pkg-config --cflags --libs libcurl json-c)
```

---

## Usage Example

The following example demonstrates the standard workflow for retrieving system inventory data.

```c
#include <stdio.h>
#include <stdlib.h>
#include "rhapi_auth.h"

int main() {
    // 1. Authenticate with SSO
    char *token = rhapi_get_token();
    if (!token) {
        fprintf(stderr, "Authentication failed.\n");
        return 1;
    }

    // 2. Execute authenticated request
    const char *endpoint = "[https://console.redhat.com/api/inventory/v1/hosts](https://console.redhat.com/api/inventory/v1/hosts)";
    char *response = rhapi_fetch_data(endpoint, token);

    if (response) {
        printf("Response Data:\n%s\n", response);
        free(response);
    }

    // 3. Resource Cleanup
    free(token);
    return 0;
}
```

---

## Implementation Notes

* **Memory Safety**: All functions that return char* use strdup() to ensure the caller has ownership of the memory.
* **Header Management**: rhapi_fetch_data uses curl_slist to manage HTTP headers; these are freed internally after the request completes.
* **Error Handling**: Errors are reported to stderr, and functions will return NULL if the request fails or memory cannot be allocated.
