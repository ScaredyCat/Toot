#ifndef POST_H
#define POST_H
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
/// @brief Post a status message in the scope marking it as sensitive or not 
/// and uploading any files previously specified.
/// @param status Text for the post
/// @param scope one of local, private, public, unlisted
/// @param topic Text for the topic / content warning
/// @param sensitive Marks uploaded files as sensitive or not. Sensitive images are
/// usually blurred / obscured if this is set to true
/// @param media_ptr pointer to array of media ids for uploaded files.
/// @return 0
int post_status(const char *status, const char *scope,  const char *topic, const bool sensitive, char *media_ptr[] );
#endif 
/* POST_H */
