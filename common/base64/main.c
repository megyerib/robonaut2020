#include <stdio.h>
#include "base64.h"
#include <string.h>

typedef struct
{
    char decoded[30];
    size_t decoded_len;

    char encoded[30];
    size_t encoded_len;
}
TEST_CASE;

// TODO write tests

int main()
{
    char str[] = "Hello World!";

    char encoded[30];
    size_t encoded_len = 0;

    base64_encode((uint8_t*)str, (uint8_t*)encoded, strlen(str), &encoded_len);

    printf("%s\nLen: %d\n", (char*)encoded, (int)encoded_len);

    char decoded[30];
    size_t decoded_len = 0;

    base64_decode((uint8_t*)encoded, (uint8_t*)decoded, encoded_len, &decoded_len);

    printf("%s\nLen: %d\n", (char*)decoded, (int)decoded_len);

    return 0;
}
