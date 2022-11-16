#ifndef POST_H
#define POST_H
/*
  This file is part of shellpost.

  shellpost is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  shellpost is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with shellpost.  If not, see <https://www.gnu.org/licenses/>.
*/

int
post_status(const char *status, const char *scope,  const char *topic, const bool sensitive, char *media_ptr[] );
#endif /* POST_H */
