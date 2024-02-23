#include <sqlite3.h>
#include <stdio.h>
#include <string.h>

void signUp(void)
{
    sqlite3* db;
    char* err_msg = 0;

    int rc = sqlite3_open("test.db", &db);

    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);

        return 1;
    }
    int valid_input = 0;
    char response = ' ';
    while (!valid_input) {
        printf("회원 가입 하시겠습니까?  ( Y / N ): ");
        scanf(" %c", &response);

        // 입력값이 Y, N, y, n 중 하나인지 확인
        if (response == 'Y' || response == 'y') {
            valid_input = 1; // 올바른 입력일 경우 루프를 빠져나감
        }
        else if (response == 'N' || response == 'n')
        {
            printf("회원 가입을 종료합니다. ");

        }
        else {
            printf("잘못된 입력입니다. Y 또는 N을 입력하세요.\n");
        }

        // 입력 버퍼 비우기
        while (getchar() != '\n');
    }

    char ID[31] = "";
    char PW[31] = "";
    char name[31] = "";
    int age = 0;
    char position[31] = "";



    // 일단 예외처리 안하고 구현부터
    // 중복 확인 로직 추가 요망
    printf(" ID 입력  \n");
    gets(ID);
    printf(" PW 입력  \n");
    gets(PW);
    printf(" name 입력  \n");
    gets(name);
    printf(" age 입력  \n");
    scanf("%d%*c", &age);
    printf(" position 입력  \n");
    gets(position);



    char sql[300];
    sprintf(sql, "INSERT INTO member (ID, PW, NAME, AGE, POSITION) VALUES ('%s', '%s', '%s', %d, '%s');", ID, PW, name, &age, position);

    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);

    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "SQL error: %s\n", err_msg);

        sqlite3_free(err_msg);
        sqlite3_close(db);
        return 1;
    }
    sqlite3_close(db);
    return 0;
}