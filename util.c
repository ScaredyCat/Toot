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
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include "util.h"
#include <wordexp.h>


static const char *get_config_filename(void)
{
	static char *ret = NULL;

	if(ret)
		return ret;

	ret = getenv("TOOTRC");

	if(!ret) {
		ret = "~/.config/.tootrc";
	}

	return ret;
}

int
config_exists(void)
{
	const char *ret = get_config_filename();
	int fileRes = -1;

 	wordexp_t result;
	wordexp( ret, &result, 0 );

	fileRes =  access(result.we_wordv[0], R_OK | W_OK);

	wordfree(&result);
	
	return fileRes;
}

int
store_config(const struct config *config)
{
	const char *ret = get_config_filename();

 	wordexp_t result;
	wordexp( ret, &result, 0 );

	FILE *fp = fopen(result.we_wordv[0], "w+");

	wordfree(&result);

	if(!fp)
		return -1;

	fprintf(fp, "instance=%s\n", config->instance);
	fprintf(fp, "client_id=%s\n", config->client_id);
	fprintf(fp, "client_secret=%s\n", config->client_secret);
	fprintf(fp, "access_token=%s\n", config->access_token);
	fclose(fp);

	return 0;
}

static int
validate_config(const struct config *config)
{
	int ret = 0;

	if(!config->instance[0]) {
		fprintf(stderr, "Error: missing instance key\n");
		ret = -1;
	}

	if(!config->client_id[0]) {
		fprintf(stderr, "Error: missing client_id key\n");
		ret = -1;
	}

	if(!config->client_secret[0]) {
		fprintf(stderr, "Error: missing client_secret key\n");
		ret = -1;
	}

	if(!config->access_token[0]) {
		fprintf(stderr, "Error: missing access_token key\n");
		ret = -1;
	}

	return ret;
}

int
load_config(struct config *config)
{

	int i;
	char line[256];
	const char *ret = get_config_filename();

 	wordexp_t result;
	wordexp( ret, &result, 0 );

	FILE *fp = fopen(result.we_wordv[0], "r");

	memset(config, 0, sizeof(*config));

	for(i = 1; fgets(line, sizeof(line), fp); i++) {
		char *key, *value;

		/* TODO: cleanup all whitespaces! */

		value = strchr(line, '=');

		if(!value) {
			fprintf(stderr, "Parse error at %i\n", i);
			fclose(fp);
			wordfree(&result);

			return -1;
		}

		key = line;
		*value = 0;
		value++;

		/* remove trailing newline */
		if (strlen(value)>0)
			value[strlen(value) - 1] = 0;

		if(!strcmp(key, "instance")) {
			dm_strncpy(config->instance, value, sizeof(config->instance));
		} else if(!strcmp(key, "client_id")) {
			dm_strncpy(config->client_id, value, sizeof(config->client_id));
		} else if(!strcmp(key, "client_secret")) {
			dm_strncpy(config->client_secret,
					 value,
					 sizeof(config->client_secret));
		} else if(!strcmp(key, "access_token")) {
			dm_strncpy(
				config->access_token, value, sizeof(config->access_token));
		} else {
			fprintf(stderr, "Unknown key %s at %i\n", key, i);
			continue;
		}
	}

	fclose(fp);
	wordfree(&result);

 	return validate_config(config);
}

size_t
cb(void *data, size_t size, size_t nmemb, void *userp)
{
	size_t realsize = size * nmemb;
	struct memory *mem = (struct memory *)userp;

	char *ptr = realloc(mem->response, mem->size + realsize + 1);
	if(ptr == NULL)
		return 0; /* out of memory! */

	mem->response = ptr;
	memcpy(&(mem->response[mem->size]), data, realsize);
	mem->size += realsize;
	mem->response[mem->size] = 0;

	return realsize;
}

/* like puts() but writes to stderr */

void
eputs(const char *s)
{
	fputs(s,stderr);
}
