#include "askr-utils.h"

UINT32 strlen(const char* str) {
    UINT32 length = 0;
    while(str[length]) length++;
    return length;
}

UINT32 digitcount(int num) {
    UINT32 count = 0;
    if (num == 0)
        return 1;
    while (num > 0) {
        count++;
        num = num/10;
    }
    return count;
}

void itoa(int num, char *number) {
    int dgcount = digitcount(num);
    int index = dgcount - 1;
    char x;
    if (num == 0 && dgcount == 1) {
        number[0] = '0';
        number[1] = '\0';
    } else {
        while (num != 0) {
            x = num % 10;
            number[index] = x + '0';
            index--;
            num = num / 10;
        }
        number[dgcount] = '\0';
    }
}