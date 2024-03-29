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
#ifndef FC__ROAD_H
#define FC__ROAD_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

enum eroad { ROAD_ROAD = 0, ROAD_RAILROAD, ROAD_LAST };

struct road_type {
  int id;
  const char *name;
  enum unit_activity act;
  enum tile_special_type special;
};

/* General road type accessor functions. */
Road_type_id road_count(void);
Road_type_id road_index(const struct road_type *proad);
Road_type_id road_number(const struct road_type *proad);

struct road_type *road_by_number(Road_type_id id);

enum unit_activity road_activity(struct road_type *road);
struct road_type *road_by_activity(enum unit_activity act);

enum tile_special_type road_special(struct road_type *road);
struct road_type *road_by_special(enum tile_special_type spe);

const char *road_name_translation(struct road_type *road);
const char *road_rule_name(struct road_type *road);

#define road_type_iterate(_p)                    \
{                                                \
  int _i_;                                       \
  for (_i_ = 0; _i_ < ROAD_LAST ; _i_++) {       \
    struct road_type *_p = road_by_number(_i_);

#define road_type_iterate_end                    \
  }}


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif  /* FC__ROAD_H */
