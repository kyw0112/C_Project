#include <sqlite3.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "global_variables.h"
// 데이터베이스 열기 함수
int openDatabase(sqlite3** db) {
    int rc = sqlite3_open("test.db", db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "데이터베이스를 열 수 없습니다: %s\n", sqlite3_errmsg(*db));
        sqlite3_close(*db);
        return -1; // 에러 코드 반환
    }
    return 0; // 성공 코드 반환
}

// 테이블 생성 함수
int createTable(sqlite3* db) {
    char* err_msg = 0;
    const char* create_table_sql = "CREATE TABLE IF NOT EXISTS member (ID TEXT, PW TEXT, NAME TEXT, AGE INT, POSITION TEXT);";
    int rc = sqlite3_exec(db, create_table_sql, 0, 0, &err_msg);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL 오류: %s\n", err_msg);
        sqlite3_free(err_msg);
        sqlite3_close(db);
        return -1; // 에러 코드 반환
    }
    return 0; // 성공 코드 반환
}

// ID 중복 확인
int isIdDuplicated(sqlite3* db, const char* id) {
    char sql[300];
    sprintf(sql, "SELECT MEMBER_ID FROM member WHERE MEMBER_ID = '%s';", id);

    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL 오류: %s\n", sqlite3_errmsg(db));
        return -1; // 에러 코드 반환
    }

    int step = sqlite3_step(stmt);

    sqlite3_finalize(stmt);

    if (step == SQLITE_ROW) {
        return 1; // 중복된 ID가 존재함
    }

    return 0; // 중복된 ID가 없음
}

// 회원 가입 함수
int signUp_in(void) {
    sqlite3* db;
    char ID[310] = "";
    char PW[310] = "";
    char name[310] = "";
    int age = 0;

    if (openDatabase(&db) == -1) {
        return -1; // 데이터베이스 열기 실패
    }

    if (createTable(db) == -1) {
        return -1; // 테이블 생성 실패
    }

    int valid_input = 0;
    char response;

    while (!valid_input) {
        printf("회원가입 하시겠습니까? ( Y / N ): ");
        scanf(" %c", &response);

        if (response == 'Y' || response == 'y') {
            valid_input = 1;
        }
        else if (response == 'N' || response == 'n') {
            printf("회원 가입을 취소합니다. ");
            sqlite3_close(db);
            return 0; // 취소 코드 반환
        }
        else {
            printf("잘못된 입력입니다. Y 또는 N을 입력하십시오.\n");
        }
    }

    // 중복 확인 및 추가
    int isDuplicated = 1;
    char* tmp_id;

    while (isDuplicated) {
        printf(" 아이디 입력: ");
        scanf("%s", ID);

        tmp_id = ID;
        int chk = 1;
        while (*tmp_id != '\0') {
            if (*tmp_id == ' ') {
                printf("공백은 포함할 수 없습니다. 다시 입력하세요.\n");
                chk = 0;
                break;
            }
            tmp_id++;
        }
        if (chk) {
            break;
        }
    }

    isDuplicated = isIdDuplicated(db, ID);

    if (isDuplicated == -1) {
        printf("중복 확인이 실패했습니다.\n");
        sqlite3_close(db);
        return -1; // 에러 코드 반환
    }

    if (isDuplicated) {
        printf("이미 존재하는 ID입니다. 다른 ID를 입력하세요.\n");
        sqlite3_close(db);
        return 0;
    }

    printf(" 비밀번호 입력: ");
    scanf("%s", PW);

    printf(" 이름 입력: ");
    scanf("%s", name);

    while (1) {
        printf(" 나이 입력: ");
        if (scanf("%d", &age) != 1) {
            printf("정수를 입력하세요.\n");
            while (getchar() != '\n'); // 입력 버퍼 비우기
        }
        else {
            while (getchar() != '\n'); // 입력 버퍼 비우기
            break;
        }
    }
    int position = 0;
    while (1) {
        printf("직책 입력: (1: 임원, 2: 책임, 3: 선임, 4: 사원) ");
        if (scanf("%d%*c", &position) != 1) { // 정수 이외의 값이 입력되었을 때
            printf("잘못된 입력입니다. 다시 입력하세요.\n");
            while (getchar() != '\n'); // 입력 버퍼 비우기
            continue;
        }
        // 입력 값이 1부터 4 사이의 정수인지 확인
        if (position < 1 || position > 4) {
            printf("1부터 4 사이의 숫자를 입력하세요.\n");
            continue;
        }
        break; // 올바른 입력이면 반복문 탈출
    }

    




    char sql[300];
    sprintf(sql, "INSERT INTO member (MEMBER_ID, PW, NAME, AGE, POSITION) VALUES ('%s', '%s', '%s', %d, '%d');", ID, PW, name, age, position);

    char* err_msg = 0;
    int rc = sqlite3_exec(db, sql, 0, 0, &err_msg);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL 오류: %s\n", err_msg);
        sqlite3_free(err_msg);
        sqlite3_close(db);
        return -1; // 에러 코드 반환
    }
    strcpy(loggedInId, ID); // 현재 로그인한 사용자의 아이디 저장
    isLoggedIn = 1; // 로그인 상태로 변경
    sqlite3_close(db);
     // 성공 코드 반환
}

// 회원 정보 조회 함수
void viewMembers(sqlite3* db) {
    char sql[100] = "SELECT * FROM member;";

    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL 오류: %s\n", sqlite3_errmsg(db));
        return; // 에러 코드 반환
    }

    printf("\n===== 회원 목록 =====\n");
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        printf("ID: %s, PW: %s, NAME: %s, AGE: %d, POSITION: %d \n",
            sqlite3_column_text(stmt, 0),
            sqlite3_column_text(stmt, 1),
            sqlite3_column_text(stmt, 2),
            sqlite3_column_int(stmt, 3),
            sqlite3_column_int(stmt, 4));
    }
    printf("=====================\n");

    sqlite3_finalize(stmt);
}

void signUp() {
    
    sqlite3* db;
    int result = openDatabase(&db);

    if (result == -1) {
        printf("데이터베이스 연결을 실패했습니다.\n");
        return 1; // 에러 코드 반환
    }

    result = signUp_in();

    if (result == -1) {
        printf("회원 가입 중 오류가 발생했습니다.\n");
    }
    else {
        printf("회원 가입이 완료되었습니다.\n");
        
        
        viewMembers(db);
        printf("얘는 개발용 개발 끝나면 지워야함");

        

    }

    sqlite3_close(db);

    return 0; // 성공 코드 반환
}
