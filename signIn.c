#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <sqlite3.h>
#include "global_variables.h"


isLoggedIn = 0;
char loggedInId[50] = "";
//char tmp_str[30] = "asdf";
//strcpy(loggedInId, tmp_str);


// 회원 정보를 담는 구조체 정의
typedef struct {
    char id[50];
    char pw[50];
} Member;

// 사용자로부터 입력을 받는 함수
void getInput(char* buffer, int bufferSize) {
    fgets(buffer, bufferSize, stdin); // 한 줄을 입력 받음
    buffer[strcspn(buffer, "\n")] = '\0'; // 개행 문자 제거
}

// 입력된 문자열에 공백이 있는지 확인하는 함수
int hasSpaces(const char* str) {
    while (*str) {
        if (isspace((unsigned char)*str))
            return 1;
        str++;
    }
    return 0;
}

// 비밀번호를 보이지 않도록 입력받는 함수
void getPassword(char* pw, int maxSize) {
    int i = 0;
    char ch;
    while (1) {
        ch = getch(); // 키 입력 받음
        if (ch == '\r' || ch == '\n') { // Enter 키가 입력되면 종료
            pw[i] = '\0';
            printf("\n");
            break;
        }
        else if (ch == '\b' && i > 0) { // 백스페이스 키가 입력되면 이전 문자 삭제
            printf("\b \b");
            i--;
        }
        else if (ch != '\b' && i < maxSize - 1) { // 백스페이스가 아니고 최대 크기보다 작으면 입력 받은 문자를 출력하고 저장
            printf("*");
            pw[i] = ch;
            i++;
        }
    }
}



// 로그인 함수
void signIn_in(sqlite3* db, Member* member) {
    char id[50], pw[50];
    int rc;
    sqlite3_stmt* stmt;

    while (!isLoggedIn) {
        printf("아이디를 입력하세요: ");
        getInput(id, sizeof(id)); // 사용자로부터 아이디 입력 받음

        // 입력된 문자열에 공백이 있는지 확인
        if (hasSpaces(id)) {
            printf("잘못된 입력입니다. 다시 입력해주세요.\n");
            continue;
        }

        printf("비밀번호를 입력하세요: ");
        getPassword(pw, sizeof(pw)); // 사용자로부터 비밀번호 입력 받음

        // 입력된 문자열에 공백이 있는지 확인
        if (hasSpaces(pw)) {
            printf("잘못된 입력입니다. 다시 입력해주세요.\n");
            continue;
        }

        char sql[100];
        sprintf(sql, "SELECT * FROM member WHERE member_id='%s' AND pw='%s';", id, pw);

        rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
        if (rc != SQLITE_OK) {
            fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
            return;
        }

        rc = sqlite3_step(stmt);
        if (rc == SQLITE_ROW) {
            printf("로그인 성공!\n");
            strcpy(loggedInId, id); // 현재 로그인한 사용자의 아이디 저장
            isLoggedIn = 1; // 로그인 상태로 변경
        }
        else {
            printf("아이디 또는 비밀번호가 잘못되었습니다. 다시 입력해주세요.\n");
        }

        sqlite3_finalize(stmt);
    }
}

void signIn() {
    sqlite3* db;
    int rc = sqlite3_open("test.db", &db);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "데이터베이스를 열 수 없습니다: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return ;
    }

    Member member;
    signIn_in(db, &member);

    sqlite3_close(db);
    
}
