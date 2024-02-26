#include <sqlite3.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h> // wchar_t 및 wide character 함수를 사용하기 위해 추가
#include <locale.h>
#include <stdlib.h> // mbstowcs와 wcstombs 함수 사용을 위해 추가
#include <windows.h>


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
    sprintf(sql, "SELECT ID FROM member WHERE ID = '%s';", id);

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
int signUp(void) {

    setlocale(LC_ALL, "ko_KR.UTF-8");
    sqlite3* db;
    wchar_t ID[310] = L""; // wchar_t 타입으로 변경
    wchar_t PW[310] = L""; // wchar_t 타입으로 변경
    wchar_t name[310] = L""; // wchar_t 타입으로 변경
    int age = 0;
    wchar_t position[310] = L""; // wchar_t 타입으로 변경

    if (openDatabase(&db) == -1) {
        return -1; // 데이터베이스 열기 실패
    }

    if (createTable(db) == -1) {
        return -1; // 테이블 생성 실패
    }

    int valid_input = 0;
    wint_t response;

    while (!valid_input) {
        setlocale(LC_ALL, "ko_KR.UTF-8");
        printf("회원가입 하시겠습니까? ( Y / N ): ");
        response = getwchar();

        // 입력값이 Y, N, y, n 중 하나인지 확인
        if (response == 'Y' || response == 'y') {
            valid_input = 1; // 올바른 입력이면 반복문 탈출
        }
        else if (response == 'N' || response == 'n') {
            printf("회원 가입을 취소합니다. ");
            sqlite3_close(db);
            return 0; // 취소 코드 반환
        }
        else {
            printf("잘못된 입력입니다. Y 또는 N을 입력하십시오.\n");
        }

        // 입력 버퍼 비우기
        while (getchar() != '\n');
    }

    // 중복 확인 및 추가
    int isDuplicated = 1;
    char* tmp_id;


    char tmppp[100] = " 이렇겐 되는거지 ? ";
    while (isDuplicated) {

        while (1) {

            setlocale(LC_ALL, ".UTF8");

            wprintf(L" 아이디 입력: ");
            printf("%s", tmppp);

            fgetws(ID, (sizeof(ID) / sizeof(wchar_t)), stdin); // fgetws 사용

            wprintf(L"%ls  퍼에스\n", ID);
            if (ID[wcslen(ID) - 1] == L'\n') {
                ID[wcslen(ID) - 1] = L'\0'; // 개행 문자 제거
            }

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

        char ID_mb[31]; // 멀티바이트 문자열 버전의 ID
        wcstombs(ID_mb, ID, sizeof(ID_mb)); // wchar_t를 char*로 변환
        isDuplicated = isIdDuplicated(db, ID_mb);

        if (isDuplicated == -1) {
            printf("중복 확인이 실패했습니다.\n");
            sqlite3_close(db);
            return -1; // 에러 코드 반환
        }

        if (isDuplicated) {
            printf("이미 존재하는 ID입니다. 다른 ID를 입력하세요.\n");
        }
    }



    while (getchar() != '\n')
    wprintf(L" 비밀번호 입력: ");
    fgetws(PW, sizeof(PW) / sizeof(wchar_t), stdin); // fgetws 사용
    if (PW[wcslen(PW) - 1] == L'\n') {
        PW[wcslen(PW) - 1] = L'\0'; // 개행 문자 제거
    }

    // wprintf로 출력
    wprintf(L"%ls  퍼에스\n", PW);


    printf(" 이름 입력: ");
    fgets(name, sizeof(name), stdin); printf("%s  퍼에스\n", strtok(PW, "\n"));
    if (name[strlen(name) - 1] == '\n') {
        name[strlen(name) - 1] = '\0'; // 개행 문자 제거
    }

    while (1) {
        printf(" 나이 입력: ");
        if (scanf("%d", &age) != 1) {
            printf("정수를 입력하세요.\n");
            // 입력 버퍼 비우기
            while (getchar() != '\n');
        }
        else {
            // 입력 버퍼 비우기
            while (getchar() != '\n');
            break;
        }
    }

    printf(" 직책 입력: ");
    fgets(position, sizeof(position), stdin);
    if (position[strlen(position) - 1] == '\n') {
        position[strlen(position) - 1] = '\0'; // 개행 문자 제거
    }

    char sql[300];
    sprintf(sql, "INSERT INTO member (ID, PW, NAME, AGE, POSITION) VALUES ('%ls', '%ls', '%ls', %d, '%ls');", ID, PW, name, age, position);

    char* err_msg = 0;
    int rc = sqlite3_exec(db, sql, 0, 0, &err_msg);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL 오류: %s\n", err_msg);
        sqlite3_free(err_msg);
        sqlite3_close(db);
        return -1; // 에러 코드 반환
    }

    sqlite3_close(db);
    return 0; // 성공 코드 반환
}

// 회원 정보 조회 함수
void viewMembers(sqlite3* db) {
    char sql[100] = "SELECT * FROM member;";
    setlocale(LC_ALL, "ko_KR.UTF-8");

    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL 오류: %s\n", sqlite3_errmsg(db));
        return; // 에러 코드 반환
    }

    // 조회 결과 출력
    printf("\n===== 회원 목록 =====\n");
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        printf("ID: %s, PW: %s, NAME: %s, AGE: %d, POSITION: %s \n",
            sqlite3_column_text(stmt, 0),
            sqlite3_column_text(stmt, 1),
            sqlite3_column_text(stmt, 2),
            sqlite3_column_int(stmt, 3),
            sqlite3_column_text(stmt, 4));
    }
    printf("=====================\n");

    sqlite3_finalize(stmt);
}

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);



    setlocale(LC_ALL, "ko_KR.UTF-8");
    
    

    
    
    sqlite3* db;
    int result = openDatabase(&db);

    if (result == -1) {
        printf("데이터베이스 연결을 실패했습니다.\n");
        return 1; // 에러 코드 반환
    }

    result = signUp();

    if (result == -1) {
        printf("회원 가입 중 오류가 발생했습니다.\n");
    }
    else {
        printf("회원 가입이 완료되었습니다.\n");
        viewMembers(db);
    }

    // 데이터베이스 닫기
    sqlite3_close(db);

    return 0; // 성공 코드 반환
}


