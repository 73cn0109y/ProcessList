#include "helpers.h"

// char* -> napi_value
void Helpers::char_to_napi_value(napi_env env, char *in, napi_value *out)
{
    napi_status status;

    status = napi_create_string_utf8(env, in, strlen(in), out);

    if (status != napi_ok)
        napi_throw_error(env, NULL, "Failed to cast char* to napi_value");
}

// napi_value -> char*
void Helpers::napi_value_to_char(napi_env env, napi_value in, char *out)
{
    napi_status status;
    size_t str_len;

    status = napi_get_value_string_utf8(env, in, NULL, NULL, &str_len);

    if (status != napi_ok)
        napi_throw_error(env, NULL, "Failed to get length of napi_value");

    status = napi_get_value_string_utf8(env, in, out, str_len, &str_len);

    if (status != napi_ok)
        napi_throw_error(env, NULL, "Failed to cast napi_value to char*");
}

// uint32_t -> napi_value
void Helpers::uint32_t_to_napi_value(napi_env env, uint32_t in, napi_value *out)
{
    napi_status status;

    status = napi_create_uint32(env, in, out);

    if (status != napi_ok)
        napi_throw_error(env, NULL, "Failed to cast DWORD to napi_value");
}

// napi_value -> uint32_t
void Helpers::napi_value_to_uint32_t(napi_env env, napi_value in, uint32_t *out)
{
    napi_status status;

    status = napi_get_value_uint32(env, in, out);

    if (status != napi_ok)
        napi_throw_error(env, NULL, "Failed to cast napi_value to DWORD");
}

// Expose a function to js
void Helpers::create_function(napi_env env, napi_value *exports, napi_callback fn, char *funcName)
{
    napi_status status;
    napi_value function;

    status = napi_create_function(env, NULL, 0, fn, NULL, &function);

    if (status != napi_ok)
        napi_throw_error(env, NULL, "Unable to wrap native function");

    status = napi_set_named_property(env, *exports, funcName, function);

    if (status != napi_ok)
        napi_throw_error(env, NULL, "Unable to populate exports");
}