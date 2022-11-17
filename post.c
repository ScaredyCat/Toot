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
#include <stdbool.h>
#include <curl/curl.h>
#include <string.h>
#include <stdlib.h>
#include <json-c/json.h>

#include "util.h"
#include "asprintf.h"

/* This function post "status" with the visibility "visibility" */

int
post_status(const char *status,
		  const char *scope,
		  const char *topic,
		  const bool sensitive,
		  char *media_ptr[])
{
	struct config config;

	if(load_config(&config) < 0) {
		fprintf(stderr, "Error loading config");
		return -1;
	}

	CURL *curl = curl_easy_init();
	if(curl == NULL) {
		fprintf(stderr, "Error creating libcurl thing\n");
		return -1;
	}

	char *api_url = "/api/v1/statuses";
	char *url = NULL;

	dm_asprintf(&url, "%s%s", config.instance, api_url);
	char *header_fmt = "Authorization: Bearer %s";
	char *authorization_header = NULL;
	struct curl_slist *header_list = NULL;

	dm_asprintf(&authorization_header, header_fmt, config.access_token);
	header_list = curl_slist_append(header_list, authorization_header);

	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, header_list);

	/* Write the thing */

	struct memory chunk = { 0 };

	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, cb);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

	curl_mime *mime;
	mime = curl_mime_init(curl);
	curl_mimepart *status_part, *scope_part, *topic_part, *source_part,
		*sensitive_part;

	curl_mimepart *media_curlparts[__MAX_UPLOADS__];

	/* Is there a better way to do this? */

	status_part = curl_mime_addpart(mime);
	topic_part = curl_mime_addpart(mime);
	sensitive_part = curl_mime_addpart(mime);
	source_part = curl_mime_addpart(mime);
	scope_part = curl_mime_addpart(mime);

	/* Status */

	curl_mime_data(status_part, status, CURL_ZERO_TERMINATED);
	curl_mime_name(status_part, "status");

	/* Topic / content warning */

	curl_mime_data(topic_part, topic, CURL_ZERO_TERMINATED);
	curl_mime_name(topic_part, "spoiler_text");

	/* Mark it sensitive if we have a topic */
	if(sensitive)
		curl_mime_data(sensitive_part, "true", CURL_ZERO_TERMINATED);
	else
		curl_mime_data(sensitive_part, "false", CURL_ZERO_TERMINATED);

	curl_mime_name(sensitive_part, "sensitive");

	/* Set souce of post - application */

	curl_mime_data(source_part, __VERSION__, CURL_ZERO_TERMINATED);
	curl_mime_name(source_part, "source");

	/* Visibility */

	curl_mime_data(scope_part, scope, CURL_ZERO_TERMINATED);
	curl_mime_name(scope_part, "visibility");

	/* Media */
	if(media_ptr != NULL) {
		for(int mediaItem = 0; mediaItem < __MAX_UPLOADS__; mediaItem++) {

			if(media_ptr[mediaItem] != NULL) {
				media_curlparts[mediaItem] = curl_mime_addpart(mime);
				curl_mime_data(media_curlparts[mediaItem],
							media_ptr[mediaItem],
							CURL_ZERO_TERMINATED);
				curl_mime_name(media_curlparts[mediaItem], "media_ids[]");
			}
		}
	}
	/* post */

	curl_easy_setopt(curl, CURLOPT_URL, url);
	curl_easy_setopt(curl, CURLOPT_MIMEPOST, mime);
	curl_easy_perform(curl);

	/* free */

	curl_easy_cleanup(curl);
	curl_slist_free_all(header_list);
	curl_mime_free(mime);

	/* The the url */

	struct json_object *parsed_json;
	struct json_object *json_url;
	struct json_object *server_response;

	parsed_json = json_tokener_parse(chunk.response);
	json_object_object_get_ex(parsed_json, "url", &json_url);
	json_object_object_get_ex(parsed_json, "error", &server_response);
	const char *error = json_object_get_string(server_response);
	if(json_url == NULL) {
		fprintf(stderr, "Error posting, server said: \"%s\"\n", error);
		return -1;
	}
	puts(json_object_get_string(json_url));

	free(url);
	free(authorization_header);
	free_response(&chunk);
	json_object_put(parsed_json);

	return 0;
}
