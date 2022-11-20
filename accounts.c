/*
  This file is part of Toot.

  Toot is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Toot is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Toot.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <curl/curl.h>
#include <json-c/json.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "asprintf.h"
#include "fetch.h"
#include "util.h"
// get the account id from @User@Domain

char *
request_account_id(char *api_parameter, const struct config *config)
{

	char *returned_data = NULL;

	struct json_object *parsed_json;
	struct json_object *accountObj;
	struct json_object *account_id;

	char *api_url_fmt = "%s/api/v1/accounts/search?q=%s&limit=1";
	char *api_url = NULL;

	if(api_parameter[0] == '@')
		api_parameter++;

	dm_asprintf(&api_url, api_url_fmt, config->instance, api_parameter);

	returned_data = do_api_request(api_url, config, false);

	if(NULL == returned_data) {
		return NULL;
	}

	parsed_json = json_tokener_parse(returned_data);

	if(NULL == parsed_json) {
		puts("An error occured fetching, invalid data returned");
		return NULL;
	}

	accountObj = json_object_array_get_idx(parsed_json, 0);

	if(json_object_object_get_ex(accountObj, "id", &account_id)) {

		/* keep freeing */
		const char *str = json_object_get_string(account_id);
		char *return_str = malloc(strlen(str + 1));

		strcpy(return_str, str); // strdup() segfaults for some reason
		json_object_put(parsed_json);

		// puts(return_str);
		return return_str;
	}

	json_object_put(parsed_json);
	return NULL;
}

// follow /unfollow
// We need to do a search, limited to 1 user
// Then use the id from that for the follow/unfollow.

char * 
follow_account(char *id, char action, const struct config *config)
{
	char *returned_data = NULL;
	char *returned_request_data = NULL;

	struct json_object *parsed_json;
	struct json_object *follow_state;

	char *api_url_fmt = "%s/api/v1/accounts/%s/follow";
	char *api_url = NULL;

	returned_data = request_account_id(id, config);

	if(NULL == returned_data) {
		return NULL;
	}

	if(action == 'f')
		api_url_fmt = "%s/api/v1/accounts/%s/follow";
	else if(action == 'u')
		api_url_fmt = "%s/api/v1/accounts/%s/unfollow";

	dm_asprintf(&api_url, api_url_fmt, config->instance, returned_data);

	returned_request_data = do_api_request(api_url, config, true);

	parsed_json = json_tokener_parse(returned_request_data);

	if(NULL == parsed_json) {
		puts("An error occured fetching, invalid data returned");
		return NULL;
	}

	if(json_object_object_get_ex(parsed_json, "error", &follow_state)) {
		const char *str = json_object_get_string(follow_state);
		char *return_str = malloc(strlen(str + 1));

		strcpy(return_str, str); // strdup() segfaults for some reason
		json_object_put(parsed_json);
		return return_str;
	}

	if(json_object_object_get_ex(parsed_json, "following", &follow_state)) {

		const char *str = json_object_get_string(follow_state);
		char *return_str = malloc(strlen(str + 1));

		strcpy(return_str, str); // strdup() segfaults for some reason
		json_object_put(parsed_json);
		return return_str;
	}

	json_object_put(parsed_json);
	return NULL;
}