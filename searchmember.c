#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>

sqlite3* db;

// 함수 프로토타입 선언
char* searchmember();
char* get_member_id_by_rownum(int rownum); // 반환 타입을 const char*에서 char*로 변경

// 멤버 리스트를 출력하고 사용자에게 입력받은 번호에 해당하는 멤버의 아이디를 반환하는 함수
char* searchmember() { // 반환 타입을 const char*에서 char*로 변경
    int rc = sqlite3_open("test.db", &db);
    if (rc) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        exit(1);
    }

    printf("멤버 리스트:\n");
    printf("번호          이름\n");

    const char* sql = "SELECT ROW_NUMBER() OVER () as rownum, NAME FROM member;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int rownum = sqlite3_column_int(stmt, 0);
            const char* name = (const char*)sqlite3_column_text(stmt, 1);
            printf("%d\t%s\n", rownum, name);
        }
    }
    sqlite3_finalize(stmt);

    int selected_rownum;
    printf("조회할 멤버의 번호를 입력하세요: ");
    scanf("%d%*c", &selected_rownum);


    char* member_id = get_member_id_by_rownum(selected_rownum);
    

    if (member_id != NULL) {
        printf(" 리턴 직전 : 멤버아이디 : _  %p\n", member_id);
    }
    else {
        printf("입력한 번호에 해당하는 멤버가 없습니다.\n");
    }

    sqlite3_close(db);
    
    return member_id; // 동적 할당된 메모리 주소 반환
}

char* get_member_id_by_rownum(int rownum) {
    char* member_id = NULL;
    char* sql = sqlite3_mprintf("SELECT MEMBER_ID FROM (SELECT ROW_NUMBER() OVER () as rownum, MEMBER_ID FROM member) WHERE rownum = %d;", rownum);
    sqlite3_stmt* stmt;
    printf("**********************************************");

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            const unsigned char* text = sqlite3_column_text(stmt, 0);
            if (text != NULL) {
                size_t len = strlen((const char*)text);
                member_id = (char*)malloc(len + 1); // 메모리 할당
                if (member_id != NULL) {
                    strcpy(member_id, (const char*)text); // 데이터 복사
                }
            }
        }
    }

    else {
        fprintf(stderr, "SQL 오류: %s\n", sqlite3_errmsg(db));
    }
    printf("**********************************************");

    sqlite3_finalize(stmt);
    sqlite3_free(sql); // sql 메모리 해제
    printf("멤버 아이디1 :____ %p", member_id);
    return member_id; // 동적 할당된 메모리 주소 반환
}
