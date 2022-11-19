#ifndef ACCOUNTS_H
#define ACCOUNTS_H

char *request_account_id(char *api_parameter,struct config *config);
char *follow_account(char *id, char action, struct config *config);

#endif