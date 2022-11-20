#ifndef ACCOUNTS_H
#define ACCOUNTS_H

/// @brief Takes \@User\@Domain and config and returns the associated user id
/// @param api_parameter \@User\@Domain
/// @param config Pre-read config struct
/// @return account id
char *request_account_id(char *api_parameter, const struct config *config);

/// @brief Takes the user id from request_account_id and either u(nfollow) or
/// f(ollow) and config
/// @param id
/// @param action
/// @param config
/// @return true if  following, false if no longer following or a warning error.
char *follow_account(char *id, char action, const struct config *config);

#endif