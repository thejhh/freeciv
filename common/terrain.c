/********************************************************************** 
 Freeciv - Copyright (C) 2003 - The Freeciv Project
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
***********************************************************************/

#ifdef HAVE_CONFIG_H
#include <fc_config.h>
#endif

/* utility */
#include "fcintl.h"
#include "log.h"                /* fc_assert */
#include "mem.h"                /* free */
#include "rand.h"
#include "shared.h"
#include "string_vector.h"
#include "support.h"

/* common */
#include "game.h"
#include "map.h"
#include "rgbcolor.h"
#include "road.h"

#include "terrain.h"

static struct terrain civ_terrains[MAX_NUM_TERRAINS];
static struct resource civ_resources[MAX_NUM_RESOURCES];

enum tile_special_type infrastructure_specials[] = {
  S_ROAD,
  S_RAILROAD,
  S_IRRIGATION,
  S_FARMLAND,
  S_MINE,
  S_OLD_FORTRESS,
  S_OLD_AIRBASE,
  S_LAST
};

/****************************************************************************
  Initialize terrain and resource structures.
****************************************************************************/
void terrains_init(void)
{
  int i;

  for (i = 0; i < ARRAY_SIZE(civ_terrains); i++) {
    /* Can't use terrain_by_number here because it does a bounds check. */
    civ_terrains[i].item_number = i;
    civ_terrains[i].rgb = NULL;
  }
  for (i = 0; i < ARRAY_SIZE(civ_resources); i++) {
    civ_resources[i].item_number = i;
  }
}

/****************************************************************************
  Free memory which is associated with terrain types.
****************************************************************************/
void terrains_free(void)
{
  terrain_type_iterate(pterrain) {
    if (NULL != pterrain->helptext) {
      strvec_destroy(pterrain->helptext);
      pterrain->helptext = NULL;
    }
    if (pterrain->resources != NULL) {
      /* Server allocates this on ruleset loading, client when
       * ruleset packet is received. */
      free(pterrain->resources);
      pterrain->resources = NULL;
    }
    if (pterrain->rgb != NULL) {
      /* Server allocates this on ruleset loading, client when
       * ruleset packet is received. */
      rgbcolor_destroy(pterrain->rgb);
      pterrain->rgb = NULL;
    }
  } terrain_type_iterate_end;
}

/**************************************************************************
  Return the first item of terrains.
**************************************************************************/
struct terrain *terrain_array_first(void)
{
  if (game.control.terrain_count > 0) {
    return civ_terrains;
  }
  return NULL;
}

/**************************************************************************
  Return the last item of terrains.
**************************************************************************/
const struct terrain *terrain_array_last(void)
{
  if (game.control.terrain_count > 0) {
    return &civ_terrains[game.control.terrain_count - 1];
  }
  return NULL;
}

/**************************************************************************
  Return the number of terrains.
**************************************************************************/
Terrain_type_id terrain_count(void)
{
  return game.control.terrain_count;
}

/**************************************************************************
  Return the terrain identifier.
**************************************************************************/
char terrain_identifier(const struct terrain *pterrain)
{
  fc_assert_ret_val(pterrain, '\0');
  return pterrain->identifier;
}

/**************************************************************************
  Return the terrain index.

  Currently same as terrain_number(), paired with terrain_count()
  indicates use as an array index.
**************************************************************************/
Terrain_type_id terrain_index(const struct terrain *pterrain)
{
  fc_assert_ret_val(pterrain, -1);
  return pterrain - civ_terrains;
}

/**************************************************************************
  Return the terrain index.
**************************************************************************/
Terrain_type_id terrain_number(const struct terrain *pterrain)
{
  fc_assert_ret_val(pterrain, -1);
  return pterrain->item_number;
}

/****************************************************************************
  Return the terrain for the given terrain index.
****************************************************************************/
struct terrain *terrain_by_number(const Terrain_type_id type)
{
  if (type < 0 || type >= game.control.terrain_count) {
    /* This isn't an error; some T_UNKNOWN callers depend on it. */
    return NULL;
  }
  return &civ_terrains[type];
}

/****************************************************************************
  Return the terrain type matching the identifier, or T_UNKNOWN if none matches.
****************************************************************************/
struct terrain *terrain_by_identifier(const char identifier)
{
  if (TERRAIN_UNKNOWN_IDENTIFIER == identifier) {
    return T_UNKNOWN;
  }
  terrain_type_iterate(pterrain) {
    if (pterrain->identifier == identifier) {
      return pterrain;
    }
  } terrain_type_iterate_end;

  return T_UNKNOWN;
}

/****************************************************************************
  Return the terrain type matching the name, or T_UNKNOWN if none matches.
****************************************************************************/
struct terrain *terrain_by_rule_name(const char *name)
{
  const char *qname = Qn_(name);

  terrain_type_iterate(pterrain) {
    if (0 == fc_strcasecmp(terrain_rule_name(pterrain), qname)) {
      return pterrain;
    }
  } terrain_type_iterate_end;

  return T_UNKNOWN;
}

/****************************************************************************
  Return the terrain type matching the name, or T_UNKNOWN if none matches.
****************************************************************************/
struct terrain *terrain_by_translated_name(const char *name)
{
  terrain_type_iterate(pterrain) {
    if (0 == strcmp(terrain_name_translation(pterrain), name)) {
      return pterrain;
    }
  } terrain_type_iterate_end;

  return T_UNKNOWN;
}

/****************************************************************************
  Return terrain having the flag. If several terrains have the flag,
  random one is returned.
****************************************************************************/
struct terrain *rand_terrain_by_flag(enum terrain_flag_id flag)
{
  int num = 0;
  struct terrain *terr = NULL;

  terrain_type_iterate(pterr) {
    if (terrain_has_flag(pterr, flag)) {
      num++;
      if (fc_rand(num) == 1) {
        terr = pterr;
      }
    }
  } terrain_type_iterate_end;

  return terr;
}

/****************************************************************************
  Fill terrain with flag to buffer. Returns number of terrains found.
  Return value can be greater than size of buffer.
****************************************************************************/
int terrains_by_flag(enum terrain_flag_id flag, struct terrain **buffer, int bufsize)
{
  int num = 0;

  terrain_type_iterate(pterr) {
    if (terrain_has_flag(pterr, flag)) {
      if (num < bufsize) {
        buffer[num] = pterr;
      }
      num++;
    }
  } terrain_type_iterate_end;

  return num;
}

/****************************************************************************
  Return the (translated) name of the terrain.
  You don't have to free the return pointer.
****************************************************************************/
const char *terrain_name_translation(const struct terrain *pterrain)
{
  return name_translation(&pterrain->name);
}

/**************************************************************************
  Return the (untranslated) rule name of the terrain.
  You don't have to free the return pointer.
**************************************************************************/
const char *terrain_rule_name(const struct terrain *pterrain)
{
  return rule_name(&pterrain->name);
}

/****************************************************************************
  Check for resource in terrain resources list.
****************************************************************************/
bool terrain_has_resource(const struct terrain *pterrain,
                          const struct resource *presource)
{
  struct resource **r = pterrain->resources;

  while (NULL != *r) {
    if (*r == presource) {
      return TRUE;
    }
    r++;
  }
  return FALSE;
}

/**************************************************************************
  Return the first item of resources.
**************************************************************************/
struct resource *resource_array_first(void)
{
  if (game.control.resource_count > 0) {
    return civ_resources;
  }
  return NULL;
}

/**************************************************************************
  Return the last item of resources.
**************************************************************************/
const struct resource *resource_array_last(void)
{
  if (game.control.resource_count > 0) {
    return &civ_resources[game.control.resource_count - 1];
  }
  return NULL;
}

/**************************************************************************
  Return the resource count.
**************************************************************************/
Resource_type_id resource_count(void)
{
  return game.control.resource_count;
}

/**************************************************************************
  Return the resource index.

  Currently same as resource_number(), paired with resource_count()
  indicates use as an array index.
**************************************************************************/
Resource_type_id resource_index(const struct resource *presource)
{
  fc_assert_ret_val(NULL != presource, -1);
  return presource - civ_resources;
}

/**************************************************************************
  Return the resource index.
**************************************************************************/
Resource_type_id resource_number(const struct resource *presource)
{
  fc_assert_ret_val(NULL != presource, -1);
  return presource->item_number;
}

/****************************************************************************
  Return the resource for the given resource index.
****************************************************************************/
struct resource *resource_by_number(const Resource_type_id type)
{
  if (type < 0 || type >= game.control.resource_count) {
    /* This isn't an error; some callers depend on it. */
    return NULL;
  }
  return &civ_resources[type];
}

/****************************************************************************
  Return the resource type matching the identifier, or NULL when none matches.
****************************************************************************/
struct resource *resource_by_identifier(const char identifier)
{
  resource_type_iterate(presource) {
    if (presource->identifier == identifier) {
      return presource;
    }
  } resource_type_iterate_end;

  return NULL;
}

/****************************************************************************
  Return the resource type matching the name, or NULL when none matches.
****************************************************************************/
struct resource *resource_by_rule_name(const char *name)
{
  const char *qname = Qn_(name);

  resource_type_iterate(presource) {
    if (0 == fc_strcasecmp(resource_rule_name(presource), qname)) {
      return presource;
    }
  } resource_type_iterate_end;

  return NULL;
}

/****************************************************************************
  Return the (translated) name of the resource.
  You don't have to free the return pointer.
****************************************************************************/
const char *resource_name_translation(const struct resource *presource)
{
  return name_translation(&presource->name);
}

/**************************************************************************
  Return the (untranslated) rule name of the resource.
  You don't have to free the return pointer.
**************************************************************************/
const char *resource_rule_name(const struct resource *presource)
{
  return rule_name(&presource->name);
}


/****************************************************************************
  This iterator behaves like adjc_iterate or cardinal_adjc_iterate depending
  on the value of card_only.
****************************************************************************/
#define variable_adjc_iterate(center_tile, _tile, card_only)		\
{									\
  enum direction8 *_tile##_list;					\
  int _tile##_count;							\
									\
  if (card_only) {							\
    _tile##_list = map.cardinal_dirs;					\
    _tile##_count = map.num_cardinal_dirs;				\
  } else {								\
    _tile##_list = map.valid_dirs;					\
    _tile##_count = map.num_valid_dirs;					\
  }									\
  adjc_dirlist_iterate(center_tile, _tile, _tile##_dir,			\
		       _tile##_list, _tile##_count) {

#define variable_adjc_iterate_end					\
  } adjc_dirlist_iterate_end;						\
}

/****************************************************************************
  Returns TRUE iff any cardinally adjacent tile contains the given terrain.
****************************************************************************/
bool is_terrain_card_near(const struct tile *ptile,
			  const struct terrain *pterrain,
                          bool check_self)
{
  if (!pterrain) {
    return FALSE;
  }

  cardinal_adjc_iterate(ptile, adjc_tile) {
    if (tile_terrain(adjc_tile) == pterrain) {
      return TRUE;
    }
  } cardinal_adjc_iterate_end;

  return check_self && ptile->terrain == pterrain;
}

/****************************************************************************
  Returns TRUE iff any adjacent tile contains the given terrain.
****************************************************************************/
bool is_terrain_near_tile(const struct tile *ptile,
			  const struct terrain *pterrain,
                          bool check_self)
{
  if (!pterrain) {
    return FALSE;
  }

  adjc_iterate(ptile, adjc_tile) {
    if (tile_terrain(adjc_tile) == pterrain) {
      return TRUE;
    }
  } adjc_iterate_end;

  return check_self && ptile->terrain == pterrain;
}

/****************************************************************************
  Return the number of adjacent tiles that have the given terrain.
****************************************************************************/
int count_terrain_near_tile(const struct tile *ptile,
			    bool cardinal_only, bool percentage,
			    const struct terrain *pterrain)
{
  int count = 0, total = 0;

  variable_adjc_iterate(ptile, adjc_tile, cardinal_only) {
    if (pterrain && tile_terrain(adjc_tile) == pterrain) {
      count++;
    }
    total++;
  } variable_adjc_iterate_end;

  if (percentage) {
    count = count * 100 / total;
  }
  return count;
}

/****************************************************************************
  Return the number of adjacent tiles that have the given terrain property.
****************************************************************************/
int count_terrain_property_near_tile(const struct tile *ptile,
				     bool cardinal_only, bool percentage,
				     enum mapgen_terrain_property prop)
{
  int count = 0, total = 0;

  variable_adjc_iterate(ptile, adjc_tile, cardinal_only) {
    struct terrain *pterrain = tile_terrain(adjc_tile);
    if (pterrain->property[prop] > 0) {
      count++;
    }
    total++;
  } variable_adjc_iterate_end;

  if (percentage) {
    count = count * 100 / total;
  }
  return count;
}

/* Names of specials.
 * (These must correspond to enum tile_special_type.)
 */
static const char *tile_special_type_names[] =
{
  NULL,
  N_("Irrigation"),
  NULL,
  N_("Mine"),
  N_("Pollution"),
  N_("Hut"),
  N_("Fortress"), /* Obsolete, placeholder for backward compatibility */
  N_("River"),
  N_("Farmland"),
  N_("Airbase"),  /* Obsolete, placeholder for backward compatibility */
  N_("Fallout")
};

/****************************************************************************
  Return the special with the given name, or S_LAST.
****************************************************************************/
enum tile_special_type special_by_rule_name(const char *name)
{
  fc_assert_ret_val(ARRAY_SIZE(tile_special_type_names) == S_LAST, S_LAST);

  tile_special_type_iterate(i) {
    if (tile_special_type_names[i] != NULL
        && 0 == strcmp(tile_special_type_names[i], name)) {
      return i;
    }
  } tile_special_type_iterate_end;

  road_type_iterate(road) {
    if (0 == strcmp(road_rule_name(road), name)) {
      return road_special(road);
    }
  } road_type_iterate_end;

  return S_LAST;
}

/****************************************************************************
  Return the translated name of the given special.
****************************************************************************/
const char *special_name_translation(enum tile_special_type type)
{
  struct road_type *road;

  fc_assert_ret_val(ARRAY_SIZE(tile_special_type_names) == S_LAST, NULL);
  fc_assert_ret_val(type >= 0 && type < S_LAST, NULL);

  road = road_by_special(type);
  if (road != NULL) {
    return road_name_translation(road);
  }

  return _(tile_special_type_names[type]);
}

/****************************************************************************
  Return the untranslated name of the given special.
****************************************************************************/
const char *special_rule_name(enum tile_special_type type)
{
  struct road_type *road;

  fc_assert_ret_val(ARRAY_SIZE(tile_special_type_names) == S_LAST, NULL);
  fc_assert_ret_val(type >= 0 && type < S_LAST, NULL);

  road = road_by_special(type);
  if (road != NULL) {
    return road_rule_name(road);
  }

  return tile_special_type_names[type];
}

/****************************************************************************
  Add the given special to the set.
****************************************************************************/
void set_special(bv_special *set, enum tile_special_type to_set)
{
  fc_assert_ret(to_set >= 0 && to_set < S_LAST);
  BV_SET(*set, to_set);
}

/****************************************************************************
  Remove the given special from the set.
****************************************************************************/
void clear_special(bv_special *set, enum tile_special_type to_clear)
{
  fc_assert_ret(to_clear >= 0 && to_clear < S_LAST);
  BV_CLR(*set, to_clear);
}

/****************************************************************************
  Clear all specials from the set.
****************************************************************************/
void clear_all_specials(bv_special *set)
{
  BV_CLR_ALL(*set);
}

/****************************************************************************
 Returns TRUE iff the given special is found in the given set.
****************************************************************************/
bool contains_special(bv_special set,
		      enum tile_special_type to_test_for)
{
  fc_assert_ret_val(to_test_for >= 0 && to_test_for < S_LAST, FALSE);
  return BV_ISSET(set, to_test_for);
}

/****************************************************************************
 Returns TRUE iff any specials are set on the tile.
****************************************************************************/
bool contains_any_specials(bv_special set)
{
  return BV_ISSET_ANY(set);
}

/****************************************************************************
  Returns TRUE iff the special can be supported by the terrain type.
****************************************************************************/
bool is_native_terrain_to_special(enum tile_special_type special,
                                  const struct terrain *pterrain)
{
  /* FIXME: The special definition should be moved into the ruleset. */
  switch (special) {
  case S_ROAD:
    return (terrain_control.may_road
            && 0 != pterrain->road_time);
  case S_IRRIGATION:
    return (terrain_control.may_irrigate
            && pterrain == pterrain->irrigation_result);
  case S_RAILROAD:
    return (terrain_control.may_road
            && 0 != pterrain->road_time);
  case S_MINE:
    return (terrain_control.may_mine
            && pterrain == pterrain->mining_result);
  case S_POLLUTION:
    return !terrain_has_flag(pterrain, TER_NO_POLLUTION);
  case S_HUT:
    return TRUE;
  case S_RIVER:
    return terrain_has_flag(pterrain, TER_CAN_HAVE_RIVER);
  case S_FARMLAND:
    return (terrain_control.may_irrigate
            && pterrain == pterrain->irrigation_result);
  case S_FALLOUT:
    return !terrain_has_flag(pterrain, TER_NO_POLLUTION);
  case S_OLD_FORTRESS:
  case S_OLD_AIRBASE:
  case S_LAST:
    break;
  }

  return FALSE;
}

/****************************************************************************
  Returns TRUE iff the special can be supported by the terrain type of the
  tile.
****************************************************************************/
bool is_native_tile_to_special(enum tile_special_type special,
                               const struct tile *ptile)
{
  return is_native_terrain_to_special(special, tile_terrain(ptile));
}

/****************************************************************************
  Returns TRUE iff any cardinally tile adjacent to (map_x,map_y) has the
  given special.
****************************************************************************/
bool is_special_card_near(const struct tile *ptile, enum tile_special_type spe,
                          bool check_self)
{
  cardinal_adjc_iterate(ptile, adjc_tile) {
    if (tile_has_special(adjc_tile, spe)) {
      return TRUE;
    }
  } cardinal_adjc_iterate_end;

  return check_self && tile_has_special(ptile, spe);
}

/****************************************************************************
  Returns TRUE iff any tile adjacent to (map_x,map_y) has the given special.
****************************************************************************/
bool is_special_near_tile(const struct tile *ptile, enum tile_special_type spe,
                          bool check_self)
{
  adjc_iterate(ptile, adjc_tile) {
    if (tile_has_special(adjc_tile, spe)) {
      return TRUE;
    }
  } adjc_iterate_end;

  return check_self && tile_has_special(ptile, spe);
}

/****************************************************************************
  Returns the number of adjacent tiles that have the given map special.
****************************************************************************/
int count_special_near_tile(const struct tile *ptile,
			    bool cardinal_only, bool percentage,
			    enum tile_special_type spe)
{
  int count = 0, total = 0;

  variable_adjc_iterate(ptile, adjc_tile, cardinal_only) {
    if (tile_has_special(adjc_tile, spe)) {
      count++;
    }
    total++;
  } variable_adjc_iterate_end;

  if (percentage) {
    count = count * 100 / total;
  }
  return count;
}

/****************************************************************************
  Returns TRUE iff any cardinally adjacent tile contains terrain with the
  given flag.
****************************************************************************/
bool is_terrain_flag_card_near(const struct tile *ptile,
			       enum terrain_flag_id flag)
{
  cardinal_adjc_iterate(ptile, adjc_tile) {
    struct terrain* pterrain = tile_terrain(adjc_tile);
    if (T_UNKNOWN != pterrain
	&& terrain_has_flag(pterrain, flag)) {
      return TRUE;
    }
  } cardinal_adjc_iterate_end;

  return FALSE;
}

/****************************************************************************
  Returns TRUE iff any adjacent tile contains terrain with the given flag.
****************************************************************************/
bool is_terrain_flag_near_tile(const struct tile *ptile,
			       enum terrain_flag_id flag)
{
  adjc_iterate(ptile, adjc_tile) {
    struct terrain* pterrain = tile_terrain(adjc_tile);
    if (T_UNKNOWN != pterrain
	&& terrain_has_flag(pterrain, flag)) {
      return TRUE;
    }
  } adjc_iterate_end;

  return FALSE;
}

/****************************************************************************
  Return the number of adjacent tiles that have terrain with the given flag.
****************************************************************************/
int count_terrain_flag_near_tile(const struct tile *ptile,
				 bool cardinal_only, bool percentage,
				 enum terrain_flag_id flag)
{
  int count = 0, total = 0;

  variable_adjc_iterate(ptile, adjc_tile, cardinal_only) {
    struct terrain *pterrain = tile_terrain(adjc_tile);
    if (T_UNKNOWN != pterrain
	&& terrain_has_flag(pterrain, flag)) {
      count++;
    }
    total++;
  } variable_adjc_iterate_end;

  if (percentage) {
    count = count * 100 / total;
  }
  return count;
}

/****************************************************************************
  Return a (static) string with special(s) name(s):
    eg: "Mine"
    eg: "Road/Farmland"
  This only includes "infrastructure", i.e., man-made specials.
****************************************************************************/
const char *get_infrastructure_text(bv_special spe, bv_bases bases)
{
  static char s[256];
  char *p;
  enum eroad road = ROAD_LAST;

  s[0] = '\0';

  /* Since railroad requires road, Road/Railroad is redundant */
  if (contains_special(spe, S_RAILROAD)) {
    road = ROAD_RAILROAD;
  } else if (contains_special(spe, S_ROAD)) {
    road = ROAD_ROAD;
  }

  if (road != ROAD_LAST) {
    cat_snprintf(s, sizeof(s), "%s/",
                 road_name_translation(road_by_number(road)));
  }

  /* Likewise for farmland on irrigation */
  if (contains_special(spe, S_FARMLAND)) {
    cat_snprintf(s, sizeof(s), "%s/", _("Farmland"));
  } else if (contains_special(spe, S_IRRIGATION)) {
    cat_snprintf(s, sizeof(s), "%s/", _("Irrigation"));
  }

  if (contains_special(spe, S_MINE)) {
    cat_snprintf(s, sizeof(s), "%s/", _("Mine"));
  }

  base_type_iterate(pbase) {
    if (BV_ISSET(bases, base_index(pbase))) {
      cat_snprintf(s, sizeof(s), "%s/", base_name_translation(pbase));
    }
  } base_type_iterate_end;

  p = s + strlen(s) - 1;
  if (*p == '/') {
    *p = '\0';
  }

  return s;
}

/****************************************************************************
  Return the prerequesites needed before building the given infrastructure.
****************************************************************************/
enum tile_special_type get_infrastructure_prereq(enum tile_special_type spe)
{
  if (spe == S_RAILROAD) {
    return S_ROAD;
  } else if (spe == S_FARMLAND) {
    return S_IRRIGATION;
  } else {
    return S_LAST;
  }
}

/****************************************************************************
  Returns the highest-priority (best) infrastructure (man-made special) to
  be pillaged from the terrain set.  May return S_LAST if nothing
  better is available.
  Bases are encoded as numbers beyond S_LAST.
****************************************************************************/
int get_preferred_pillage(bv_special pset,
                          bv_bases bases)
{
  if (contains_special(pset, S_FARMLAND)) {
    return S_FARMLAND;
  }
  if (contains_special(pset, S_IRRIGATION)) {
    return S_IRRIGATION;
  }
  if (contains_special(pset, S_MINE)) {
    return S_MINE;
  }
  base_type_iterate(pbase) {
    if (BV_ISSET(bases, base_index(pbase))) {
      return S_LAST + base_index(pbase) + 1;
    }
  } base_type_iterate_end;
  if (contains_special(pset, S_RAILROAD)) {
    return S_RAILROAD;
  }
  if (contains_special(pset, S_ROAD)) {
    return S_ROAD;
  }
  return S_LAST;
}

/****************************************************************************
  Does terrain type belong to terrain class?
****************************************************************************/
bool terrain_belongs_to_class(const struct terrain *pterrain,
                              enum terrain_class class)
{
  switch(class) {
   case TC_LAND:
     return !is_ocean(pterrain);
   case TC_OCEAN:
     return is_ocean(pterrain);
  }

  fc_assert(FALSE);
  return FALSE;
}

/****************************************************************************
  Is there terrain of the given class cardinally near tile?
****************************************************************************/
bool is_terrain_class_card_near(const struct tile *ptile, enum terrain_class class)
{
  switch(class) {
   case TC_LAND:
     cardinal_adjc_iterate(ptile, adjc_tile) {
       struct terrain* pterrain = tile_terrain(adjc_tile);
       if (T_UNKNOWN != pterrain
           && !terrain_has_flag(pterrain, TER_OCEANIC)) {
         return TRUE;
       }
     } cardinal_adjc_iterate_end;
     return FALSE;
   case TC_OCEAN:
     return is_ocean_card_near(ptile);
  }

  fc_assert(FALSE);
  return FALSE;
}

/****************************************************************************
  Is there terrain of the given class near tile?
****************************************************************************/
bool is_terrain_class_near_tile(const struct tile *ptile, enum terrain_class class)
{
  switch(class) {
   case TC_LAND:
     adjc_iterate(ptile, adjc_tile) {
       struct terrain* pterrain = tile_terrain(adjc_tile);
       if (T_UNKNOWN != pterrain
           && !terrain_has_flag(pterrain, TER_OCEANIC)) {
         return TRUE;
       }
     } adjc_iterate_end;
     return FALSE;
   case TC_OCEAN:
     return is_ocean_near_tile(ptile);
  }

  fc_assert(FALSE);
  return FALSE;
}

/****************************************************************************
  Return the (translated) name of the given terrain class.
  You don't have to free the return pointer.
****************************************************************************/
const char *terrain_class_name_translation(enum terrain_class tclass)
{
  if (!terrain_class_is_valid(tclass)) {
    return NULL;
  }

  return _(terrain_class_name(tclass));
}

/****************************************************************************
  Can terrain support given infrastructure?
****************************************************************************/
bool terrain_can_support_alteration(const struct terrain *pterrain,
                                    enum terrain_alteration alter)
{
  switch (alter) {
   case TA_CAN_IRRIGATE:
     return (terrain_control.may_irrigate
          && (pterrain == pterrain->irrigation_result));
   case TA_CAN_MINE:
     return (terrain_control.may_mine
          && (pterrain == pterrain->mining_result));
   case TA_CAN_ROAD:
     return (terrain_control.may_road && (pterrain->road_time > 0));
   default:
     break;
  }

  fc_assert(FALSE);
  return FALSE;
}

/****************************************************************************
  Return the (translated) name of the infrastructure (e.g., "Irrigation").
  You don't have to free the return pointer.
****************************************************************************/
const char *terrain_alteration_name_translation(enum terrain_alteration talter)
{
  switch(talter) {
   case TA_CAN_IRRIGATE:
     return special_name_translation(S_IRRIGATION);
   case TA_CAN_MINE:
     return special_name_translation(S_MINE);
   case TA_CAN_ROAD:
     return special_name_translation(S_ROAD);
   default:
     return NULL;
  }
}
