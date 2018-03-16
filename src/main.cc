#include "helpers.h"

#define MAX_PATH 1024

/**
 * Private
 * 
 * Get process name from PID
 */
char *_getProcessName(napi_env env, uint32_t pid)
{
    char szProcessName[MAX_PATH];

    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);

    if (hProcess != NULL)
    {
        HMODULE hModule;
        DWORD cbNeeded;

        if (EnumProcessModules(hProcess, &hModule, sizeof(hModule), &cbNeeded))
        {
            GetModuleBaseName(hProcess, hModule, szProcessName, MAX_PATH);

            return szProcessName;
        }
    }

    return NULL;
}

/**
 * Public
 * 
 * Get process name from PID
 */
napi_value getProcessName(napi_env env, napi_callback_info info)
{
    size_t argc = 1;
    napi_value argv[1];
    napi_status status;

    // Get arguments
    status = napi_get_cb_info(env, info, &argc, argv, NULL, NULL);

    if (status != napi_ok)
        napi_throw_error(env, NULL, "Failed to get napi_callback_info");

    // Convert first argument to uint32_t
    uint32_t pid;
    Helpers::napi_value_to_uint32_t(env, argv[0], &pid);

    // Get the processes name
    char *name = _getProcessName(env, pid);

    // Convert name to napi_value
    napi_value value;
    Helpers::char_to_napi_value(env, _getProcessName(env, pid), &value);

    return value;
}

/**
 * Public
 * 
 * List all processes names
 */
napi_value getProcesses(napi_env env, napi_callback_info info)
{
    napi_status status;
    DWORD aProcesses[1024], cbNeeded, cProcesses;

    // Get all processes
    if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded))
        napi_throw_error(env, NULL, "Unable to enumerate processes");

    // Number of total processes
    cProcesses = cbNeeded / sizeof(DWORD);

    // Create a javascript array
    napi_value arr;
    status = napi_create_array_with_length(env, cProcesses, &arr);

    if (status != napi_ok)
        napi_throw_error(env, NULL, "Failed to create array");

    // Loop over all processes and get their name
    for (int i = 0; i < 1024; i++)
    {
        char *name = _getProcessName(env, aProcesses[i]);

        if (name == NULL)
            continue;

        // Conver PID to napi_value
        napi_value process_id;
        Helpers::uint32_t_to_napi_value(env, (uint32_t)aProcesses[i], &process_id);

        // Convert the processes name to napi_value
        napi_value info;
        Helpers::char_to_napi_value(env, name, &info);

        // Create info array
        napi_value proc_info;
        status = napi_create_object(env, &proc_info);

        napi_value process_id_key;
        napi_create_string_utf8(env, "PID", 3, &process_id_key);

        napi_value process_name_key;
        napi_create_string_utf8(env, "Process Name", 12, &process_name_key);

        napi_set_property(env, proc_info, process_id_key, process_id);
        napi_set_property(env, proc_info, process_name_key, info);

        // Put the name into the javascript array
        status = napi_set_element(env, arr, i, proc_info);

        if (status != napi_ok)
            napi_throw_error(env, NULL, "Failed to set element of array");
    }

    return arr;
}

napi_value Init(napi_env env, napi_value exports)
{
    Helpers::create_function(env, &exports, getProcesses, "getProcesses");
    Helpers::create_function(env, &exports, getProcessName, "getProcessName");

    return exports;
}

NAPI_MODULE(NODE_GYP_MODULE_NAME, Init);