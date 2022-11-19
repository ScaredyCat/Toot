#ifndef FETCH_H
#define FETCH_H

#include <stdbool.h>

char* do_api_request(char*api_url, struct config *config, bool post);

#endif