#ifndef ASPRINTF_H
#define ASPRINTF_H
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

#include <stdarg.h>

int
dm_asprintf(char **restrict strp, const char *restrict fmt, ...);

int
dm_vasprintf(char **restrict strp, const char *restrict fmt, va_list ap);
#endif /* ASPRINTF_H */
