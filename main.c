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

#include <stdio.h>
#include <readline/readline.h>
#include <stdlib.h>
#include <getopt.h>
#include <libgen.h>
#include <stdbool.h>
#include <unistd.h>
#include "util.h"

#include "login.h"
#include "post.h"
#include "upload_file.h"
#include "fetch.h"

#include "accounts.h"
#include "version.h"

/* prints usage */

void
usage(const char *progname)
{
	printf("Usage: %s -s=status [-v=visibility] [-D=description] [-F=filename] [-t=topic | -c=content-warning]\n", progname);
	return;
}

void
help()
{
	puts("\n");
	puts(__TOOT_VERSION__);

	puts("\nPost messages and images to an ActivityPub service, e.g. Pleroma, Mastodon.");
	puts("Parameters");
	puts("\n-s, --status: status text to post");

	puts("\nMultiple descriptions and files can be uploaded but each one should");
	puts("be added with a -D followed by -F. You should either omit the -D parameter or");
	puts("make sure these is one for each file being uploaded.\n");
	puts("-D, --description: description of any file uploaded. ** Put before "
		"-F or --filename **");
	puts("-F, --filename: filename to be attatched with the post.\n");
	puts("Topic -t will not mark this content as sensitive but -c will.");
	puts("This only affects attachements. If, for example, an image is attached and the post is marked as sensitive, it should be blurred/obscured by the instance. If the post is not marked as sensitive the image would not be obscurred.\n");

	puts("-t, --topic: Topic or content warning text");
	puts("-c, --content-warning: Topic or content warning text");

	puts("-v, --visibility: The visibility which the post will have, can "
		"be: local, public unlisted, private and direct");
	puts("-f, --follow: Follow an account");
	puts("-u, --unfollow: Unfollow an account");
	puts("-U, --usage: prints usage message");
	puts("-V, --version: prints current version");
	puts("-h, --help: prints this help message\n\n");

	puts("Example:\n");
	puts("toot -v private -t \"Some title text\" -D \"File 1 alt test/description\" -F file1.png -D \"File 2 alt text/description\" -F file2.png -s \"Here is the body of the text #toot\"\n");
	
}

/* prints a string to stderr */

int
main(int argc, char **argv)
{
	if(config_exists() != 0 && setup() != 0) {
		eputs("Login failed");
	}
	int c;
	char *status = NULL;
	char *filedesc = NULL;
	char *topic = NULL;
	char *visibility = NULL;
	char *media_ptr[__MAX_UPLOADS__];

	bool sensitive = false;
	bool abort = false;

	for(int a=0;a<__MAX_UPLOADS__; a++)
		media_ptr[a] = NULL;

	unsigned int idc = 0;

	if(!isatty(0)) {
		eputs("Not a terminal, reading from stdin");
		char status[5000]; /* TODO get max from instance */
		fread(status, 5000, 1, stdin);
		post_status(status, NULL, NULL,false, NULL);
		return 0;
	}

	if(argc == 1) {
		usage(argv[0]);
		return -1;
	}

	struct config config;

	if(load_config(&config) < 0) {
		fprintf(stderr, "Error loading config");
		return -1;
	}


	int option_index = 0;
	static struct option long_options[] = {
		{ "status", required_argument, 0, 's' },
		{ "search", required_argument, 0, 'S' },		
		{ "topic", required_argument, 0, 't' },
		{ "content-warning", required_argument, 0, 'c' },
		{ "description", required_argument, 0, 'D' },
		{ "zee", required_argument, 0, 'Z' },
		{ "filename", required_argument, 0, 'F' },
		{ "visibility", required_argument, 0, 'v' },
		{ "follow", required_argument, 0, 'f' },
		{ "unfollow", required_argument, 0, 'u' },
		{ "usage", no_argument, 0, 'U' },
		{ "version", no_argument, 0, 'V'},
		{ "help", no_argument, 0, 'h' },
		{ 0, 0, 0, 0 }
	};

	while((c = getopt_long(
			  argc, argv, "s:v:Z:D:F:f:t:c:u:S:VhU", long_options, &option_index)) !=
		 -1 && abort == false) {
		switch(c) {
			case 'S':
				request_account_id(optarg, &config);
				return 0;
				break;
			case 's':
				status = optarg;
				break;
			case 'v':
				visibility = optarg;
				break;
			case 'D':
				filedesc = optarg;
				break;
			case 'F':
				if (idc < __MAX_UPLOADS__) {
					if (upload_file(optarg, filedesc, &media_ptr[idc]) == 0)
						idc++;
					else {
						abort = true;
					}
				}
				break;
			case 't':
				topic = optarg;
				sensitive=false;
				break;	
            case 'c':
                topic = optarg;
				sensitive=true;
                break;				
			case 'f':
				// Follow
				puts(follow_account(optarg, 'f',&config));			
				return 0;
				break;
			case 'u':
				// Unfollow		
				puts(follow_account(optarg, 'u',&config));
				return 0;
			case 'h':
				help();
				return 0;
				break;
			case 'U':
				usage(argv[0]);
				return 0;
				break;
			case 'V':
				puts(__TOOT_VERSION__);
				return 0;
				break;
			case 'Z':
				request_account_id(optarg, &config);
				return 0;
				break;
			case '?':
				break;
			default:
				fprintf(stderr, "Unrecognized option: %c\n", c);
				return 0;
				break;
		}
	}

	if(status == NULL && false == abort) {
		eputs("\nMust have status text (use -s \"Your status\")\n");
		return -1;
	}


	if(visibility == NULL) {
		visibility = "public";
	}

	if (false == abort)
		post_status(status, visibility, topic, sensitive, media_ptr);
	else
		puts("Something went wrong, message post aborted.");
	return 0;
}
