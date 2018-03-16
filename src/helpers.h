#include <node_api.h>
#include <Windows.h>
#include <psapi.h>

typedef unsigned long DWORD;

#ifndef HELPERS
class Helpers
{
  public:
    static void char_to_napi_value(napi_env env, char *in, napi_value *out);
    static void napi_value_to_char(napi_env env, napi_value in, char *out);
    static void uint32_t_to_napi_value(napi_env env, uint32_t in, napi_value *out);
    static void napi_value_to_uint32_t(napi_env env, napi_value in, uint32_t *out);
    static void create_function(napi_env env, napi_value *exports, napi_callback fn, char *funcName);

  private:
    Helpers() {}
};
#endif