/*
** Lua binding: fcdb
** Generated automatically by tolua 5.1.4 on Sat Apr 27 14:41:08 2013.
*/

#include "tolua.h"

#ifndef __cplusplus
#include <stdlib.h>
#endif
#ifdef __cplusplus
 extern "C" int tolua_bnd_takeownership (lua_State* L); // from tolua_map.c
#else
 int tolua_bnd_takeownership (lua_State* L); /* from tolua_map.c */
#endif
#include <string.h>

/* Exported function */
TOLUA_API int tolua_fcdb_open (lua_State* tolua_S);
LUALIB_API int luaopen_fcdb (lua_State* tolua_S);

#ifdef HAVE_CONFIG_H
#include <fc_config.h>
#endif
#include "luascript_types.h"
#include "api_fcdb_auth.h"
#include "api_fcdb_base.h"
#include "script_fcdb.h"

/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"Connection");
}

/* function: api_auth_get_username */
static int tolua_fcdb_auth_get_username00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Connection",0,&tolua_err) || 
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  lua_State* L =  tolua_S;
  Connection* pconn = ((Connection*)  tolua_tousertype(tolua_S,1,0));
 {
  const char* tolua_ret = (const char*)  api_auth_get_username(L,pconn);
 tolua_pushstring(tolua_S,(const char*)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_username'.",&tolua_err);
 return 0;
#endif
}

/* function: api_auth_get_ipaddr */
static int tolua_fcdb_auth_get_ipaddr00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Connection",0,&tolua_err) || 
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  lua_State* L =  tolua_S;
  Connection* pconn = ((Connection*)  tolua_tousertype(tolua_S,1,0));
 {
  const char* tolua_ret = (const char*)  api_auth_get_ipaddr(L,pconn);
 tolua_pushstring(tolua_S,(const char*)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_ipaddr'.",&tolua_err);
 return 0;
#endif
}

/* function: api_auth_set_password */
static int tolua_fcdb_auth_set_password00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Connection",0,&tolua_err) || 
 !tolua_isstring(tolua_S,2,0,&tolua_err) || 
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  lua_State* L =  tolua_S;
  Connection* pconn = ((Connection*)  tolua_tousertype(tolua_S,1,0));
  const char* password = ((const char*)  tolua_tostring(tolua_S,2,0));
 {
  bool tolua_ret = (bool)  api_auth_set_password(L,pconn,password);
 tolua_pushboolean(tolua_S,(bool)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'set_password'.",&tolua_err);
 return 0;
#endif
}

/* function: api_auth_get_password */
static int tolua_fcdb_auth_get_password00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Connection",0,&tolua_err) || 
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  lua_State* L =  tolua_S;
  Connection* pconn = ((Connection*)  tolua_tousertype(tolua_S,1,0));
 {
  const char* tolua_ret = (const char*)  api_auth_get_password(L,pconn);
 tolua_pushstring(tolua_S,(const char*)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_password'.",&tolua_err);
 return 0;
#endif
}

/* function: api_fcdb_option */
static int tolua_fcdb_fcdb_option00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isnumber(tolua_S,1,0,&tolua_err) || 
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  lua_State* L =  tolua_S;
  int type = ((int)  tolua_tonumber(tolua_S,1,0));
 {
  const char* tolua_ret = (const char*)  api_fcdb_option(L,type);
 tolua_pushstring(tolua_S,(const char*)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'option'.",&tolua_err);
 return 0;
#endif
}

/* Open lib function */
LUALIB_API int luaopen_fcdb (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 tolua_reg_types(tolua_S);
 tolua_module(tolua_S,NULL,0);
 tolua_beginmodule(tolua_S,NULL);
 tolua_module(tolua_S,"auth",0);
 tolua_beginmodule(tolua_S,"auth");
 tolua_function(tolua_S,"get_username",tolua_fcdb_auth_get_username00);
 tolua_function(tolua_S,"get_ipaddr",tolua_fcdb_auth_get_ipaddr00);
 tolua_function(tolua_S,"set_password",tolua_fcdb_auth_set_password00);
 tolua_function(tolua_S,"get_password",tolua_fcdb_auth_get_password00);
 tolua_endmodule(tolua_S);
 tolua_module(tolua_S,"fcdb",0);
 tolua_beginmodule(tolua_S,"fcdb");
 tolua_module(tolua_S,"param",0);
 tolua_beginmodule(tolua_S,"param");
 tolua_constant(tolua_S,"HOST",FCDB_OPTION_TYPE_HOST);
 tolua_constant(tolua_S,"USER",FCDB_OPTION_TYPE_USER);
 tolua_constant(tolua_S,"PORT",FCDB_OPTION_TYPE_PORT);
 tolua_constant(tolua_S,"PASSWORD",FCDB_OPTION_TYPE_PASSWORD);
 tolua_constant(tolua_S,"DATABASE",FCDB_OPTION_TYPE_DATABASE);
 tolua_constant(tolua_S,"TABLE_USER",FCDB_OPTION_TYPE_TABLE_USER);
 tolua_constant(tolua_S,"TABLE_LOG",FCDB_OPTION_TYPE_TABLE_LOG);
 tolua_constant(tolua_S,"BACKEND",FCDB_OPTION_TYPE_BACKEND);
 tolua_endmodule(tolua_S);
 tolua_module(tolua_S,"status",0);
 tolua_beginmodule(tolua_S,"status");
 tolua_constant(tolua_S,"ERROR",FCDB_ERROR);
 tolua_constant(tolua_S,"TRUE",FCDB_SUCCESS_TRUE);
 tolua_constant(tolua_S,"FALSE",FCDB_SUCCESS_FALSE);
 tolua_endmodule(tolua_S);
 tolua_function(tolua_S,"option",tolua_fcdb_fcdb_option00);
 tolua_endmodule(tolua_S);
 tolua_endmodule(tolua_S);
 return 1;
}
/* Open tolua function */
TOLUA_API int tolua_fcdb_open (lua_State* tolua_S)
{
 lua_pushcfunction(tolua_S, luaopen_fcdb);
 lua_pushstring(tolua_S, "fcdb");
 lua_call(tolua_S, 1, 0);
 return 1;
}
