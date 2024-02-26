#include <sqlite3.h>
#include <stdio.h>

#define MEMBERID "test1"

int main(void)
{
    printf("%s\n", sqlite3_libversion());
    //signUp();
    //restInput();
    recomByCategory("test1");
    //recomByCategory(MEMBERID);
    return 0;
}