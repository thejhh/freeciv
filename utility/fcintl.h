/********************************************************************** 
 Freeciv - Copyright (C) 1996 - A Kjeldberg, L Gregersen, P Unold
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
***********************************************************************/
#ifndef FC__FCINTL_H
#define FC__FCINTL_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifdef HAVE_CONFIG_H
#ifndef FC_CONFIG_H             /* this should be defined in fc_config.h */
#error Files including fcintl.h should also include fc_config.h directly
#endif
#endif

#ifdef HAVE_LOCALE_H
#include <locale.h>
#endif

#include "shared.h" /* bool */

#ifdef ENABLE_NLS

/* Include libintl.h only if nls enabled.
 * It defines some wrapper macros that
 * we don't want defined when nls is disabled. */
#ifdef HAVE_LIBINTL_H
#include <libintl.h>
#endif

#define _(String) gettext(String)
#define N_(String) String
#define Q_(String) skip_intl_qualifier_prefix(gettext(String))
#define PL_(String1, String2, n) ngettext((String1), (String2), (n))

#else  /* ENABLE_NLS */

#define _(String) (String)
#define N_(String) String
#define Q_(String) skip_intl_qualifier_prefix(String)
#define PL_(String1, String2, n) ((n) == 1 ? (String1) : (String2))
#define C_(String) capitalized_string(String)

#undef textdomain
#undef bindtextdomain

#define textdomain(Domain)
#define bindtextdomain(Package, Directory)

#endif /* ENABLE_NLS */

/* This provides an untranslated version of Q_ that allows the caller to
 * get access to the original string.  This may be needed for comparisons,
 * for instance. */
#define Qn_(String) skip_intl_qualifier_prefix(String)

const char *skip_intl_qualifier_prefix(const char *str);
char *capitalized_string(const char *str);
void free_capitalized(char *str);
void capitalization_opt_in(void);
bool is_capitalization_enabled(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif  /* FC__FCINTL_H */
