#include <err.h>
#include <stdio.h>
#include <string.h>

#include <tee_client_api.h>
#include <authentication_ta.h>

int main(void)
{
    TEEC_Result res;
    TEEC_Context ctx;
    TEEC_Session sess;
    TEEC_Operation op;
    TEEC_UUID uuid = TA_AUTHENTICATION_UUID;
    uint32_t err_origin;

    res = TEEC_InitializeContext(NULL, &ctx);
    if (res != TEEC_SUCCESS) {
        errx(1, "TEEC_InitializeContext failed with code 0x%x", res);
    }

    res = TEEC_OpenSession(&ctx, &sess, &uuid, TEEC_LOGIN_PUBLIC, NULL, NULL, &err_origin);
    if (res != TEEC_SUCCESS) {
        errx(1, "TEEC_OpenSession failed with code 0x%x origin 0x%x", res, err_origin);
    }

    const char* inputString;
    char buffer[256];
    printf("Enter a string: ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    inputString = buffer;

    memset(&op, 0, sizeof(op));
    op.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_INPUT, TEEC_VALUE_INOUT, TEEC_NONE, TEEC_NONE);
    op.params[0].tmpref.buffer = inputString;
    op.params[0].tmpref.size = strlen(inputString) + 1;
    op.params[1].value.a = 0;

    printf("初始化认证状态 %d\n", op.params[1].value.a);

    res = TEEC_InvokeCommand(&sess, TA_AUTHENTICATION_CMD_COMPARE_STRING, &op, &err_origin);
    if (res != TEEC_SUCCESS) {
        errx(1, "TEEC_InvokeCommand failed with code 0x%x origin 0x%x", res, err_origin);
    }

    printf("认证结果 %d\n", op.params[1].value.a);

    if (op.params[1].value.a == 1) {
        printf("Authentication successful\n");
        // 在此处执行认证成功的操作
    } else {
        printf("Authentication failed\n");
        // 在此处执行认证失败的操作
    }

    TEEC_CloseSession(&sess);
    TEEC_FinalizeContext(&ctx);

    return 0;
}
