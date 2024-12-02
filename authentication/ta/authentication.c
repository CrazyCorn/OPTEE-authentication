#include <string.h>
#include <tee_api.h>
#include <tee_internal_api.h>
#include <authentication_ta.h>


TEE_Result TA_CreateEntryPoint(void)
{
    DMSG("has been called");

    return TEE_SUCCESS;
}

void TA_DestroyEntryPoint(void)
{
    DMSG("has been called");
}

TEE_Result TA_OpenSessionEntryPoint(uint32_t param_types,
                                    TEE_Param __unused params[4],
                                    void __unused **sess_ctx)
{
    if (param_types != TEE_PARAM_TYPES(TEE_PARAM_TYPE_NONE,
                                       TEE_PARAM_TYPE_NONE,
                                       TEE_PARAM_TYPE_NONE,
                                       TEE_PARAM_TYPE_NONE))
        return TEE_ERROR_BAD_PARAMETERS;

    // 在此处进行会话打开时的操作，如果有需要的话
    return TEE_SUCCESS;
}

void TA_CloseSessionEntryPoint(void __unused *sess_ctx)
{
    // 在此处进行会话关闭时的操作，如果有需要的话
}

static bool compare_device_code(const char *input_code)
{
    const char *device_code = "ABC123";
    return strcmp(input_code, device_code) == 0;
}

TEE_Result TA_InvokeCommandEntryPoint(void __unused *sess_ctx,
                                      uint32_t cmd_id,
                                      uint32_t param_types,
                                      TEE_Param params[4])
{
    if (param_types != TEE_PARAM_TYPES(TEE_PARAM_TYPE_MEMREF_INPUT,
                                       TEE_PARAM_TYPE_VALUE_INOUT,
                                       TEE_PARAM_TYPE_NONE,
                                       TEE_PARAM_TYPE_NONE))
        return TEE_ERROR_BAD_PARAMETERS;

    switch (cmd_id) {
        case TA_AUTHENTICATION_CMD_COMPARE_STRING:
        {
            bool result = compare_device_code((const char*)params[0].memref.buffer);
            params[1].value.a = result ? 1 : 0;
            return TEE_SUCCESS;
        }
        default:
            return TEE_ERROR_BAD_PARAMETERS;
    }
}
