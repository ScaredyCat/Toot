/*
  This file is part of toot.

  toot is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  toot is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with toot.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <curl/curl.h>
#include <json-c/json.h>
#include <string.h>
#include "util.h"
#include "asprintf.h"
#include <stdbool.h>



char *
do_api_request(char *api_url, struct config *config, bool post)
{
	CURL *curl = curl_easy_init();
	if(curl == NULL) {
		fprintf(stderr, "Error creating libcurl thing\n");
		return NULL;
	}

	struct json_object *parsed_json;

	char *header_fmt = "Authorization: Bearer %s";
	char *authorization_header = NULL;
	struct curl_slist *header_list = NULL;

	dm_asprintf(&authorization_header, header_fmt, config->access_token);
	header_list = curl_slist_append(header_list, authorization_header);

	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, header_list);
	
	if (post) {
 		curl_easy_setopt(curl, CURLOPT_POST, 1L);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "");
	}

 	curl_easy_setopt(curl, CURLOPT_TIMEOUT, 5L);
	curl_easy_setopt(curl, CURLOPT_URL, api_url);
	/* Create buffer for the result */

	struct memory chunk = { 0 };

	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, cb);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

	/* free */

	CURLcode cret = curl_easy_perform(curl);
	curl_easy_cleanup(curl);
	free(api_url);
	free(authorization_header);
	curl_slist_free_all(header_list);

	/* parse the thing */

	if(cret == CURLE_HTTP_RETURNED_ERROR) {
		/* an HTTP response error problem */
		puts("An error occured fetching the page. HTTP error");
		return NULL;
	}

	if(cret == CURLE_OPERATION_TIMEDOUT) {
		/* an HTTP response error problem */
		puts("An error occured fetching the page, timed out.");
		return NULL;
	}
	if(cret == CURLE_COULDNT_CONNECT) {
		/* an HTTP response error problem */
		puts("An error occured fetching the page, could not connect.");
		return NULL;
	}

	if(cret != CURLE_OK) {
		/* response error problem */
		printf("Error code %d  occured whem fetching the page.\n", cret);
		return NULL;
	}
 
	parsed_json = json_tokener_parse(chunk.response);

	if(NULL == parsed_json) {
		puts("An error occured fetching, invalid data returned");
		return NULL;
	}

	// return json
	char *return_str = malloc(strlen(chunk.response + 1));
	strcpy(return_str, chunk.response); // strdup() segfaults for some reason

	json_object_put(parsed_json);
	free_response(&chunk);

	return return_str;

}

