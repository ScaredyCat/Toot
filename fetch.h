#ifndef FETCH_H
#define FETCH_H

#include "util.h"
#include <stdbool.h>

/// @brief Generic api call shared by anything requesting data takes destination
/// url, config and true for post, false for get
/// @param api_url
/// @param config
/// @param post
/// @return parsed json
char *do_api_request(char *api_url, const struct config *config, bool post);

#endif