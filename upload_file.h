#ifndef UPLOAD_FILE_H
#define UPLOAD_FILE_H
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

/// @brief Upload a file, optionally, with description and get a meda id
/// @param path of file being uploaded
/// @param description description of the file, used as alt text
/// @param id_ptr array pointer used to attach 0 or more files to a status post.
/// @return 0
int upload_file(const char *path, const char *description, char **id_ptr);
#endif /* UPLOAD_FILE_H */
