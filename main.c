#include <sqlite3.h>
#include <stdio.h>

int main(void)
{
    printf("%s\n", sqlite3_libversion());
    signUp();
    return 0;
}