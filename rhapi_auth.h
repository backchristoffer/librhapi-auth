#ifndef RHAPI_AUTH_H
#define RHAPI_AUTH_H

// get access token
char* rhapi_get_token(void);


// use access token for red hat api endpoints
char* rhapi_fetch_data(const char *url, const char *token);

#endif
