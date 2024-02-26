#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
    char NAME[100];
    char CATEGORY[100];
} Restaurant;

// 데이터베이스 열기 함수
int openDatabase(sqlite3** db) {
    int rc = sqlite3_open("test.db", db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "데이터베이스를 열 수 없습니다: %s\n", sqlite3_errmsg(*db));
        sqlite3_close(*db);
        return -1;
    }
    return 0;
}

// 테이블 생성 함수 ERD 클라우드에 맞춰서 수정 필요
int createTable(sqlite3* db) {
    char* err_msg = 0;
    const char* create_table_sql = "CREATE TABLE IF NOT EXISTS restaurant ("
        "restaurant_no INTEGER NOT NULL PRIMARY KEY,"
        "name TEXT NOT NULL,"
        "rating_sum INT,"
        "people INT, "
        "rating_avg INT,"
        "CATEGORY TEXT NOT NULL);";
    int rc = sqlite3_exec(db, create_table_sql, 0, 0, &err_msg);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL 오류: %s\n", err_msg);
        sqlite3_free(err_msg);
        sqlite3_close(db);
        return -1;
    }
    return 0;
}

// 랜덤 평점 생성 함수 (1~5)
int getRandomRating() {
    /*return rand() % 5 + 1;*/
    return 1;
}

// CSV 파일에서 데이터 읽기 함수
char* readCSVFile(const char* fileName) {
    FILE* file = fopen(fileName, "r");
    if (!file) {
        perror("파일 열기 오류");
        exit(1);
    }

    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    // 파일 크기가 음수인지 확인
    if (fileSize < 0) {
        fclose(file);
        perror("파일 크기 가져오기 오류");
        exit(1);
    }

    char* content = (char*)malloc(fileSize + 1);
    if (!content) {
        fclose(file);
        perror("메모리 할당 오류");
        exit(1);
    }

    fread(content, 1, fileSize, file);
    fclose(file);

    content[fileSize] = '\0'; // 내용을 널 종단

    return content;
}

// 중복 확인 함수
int isDuplicate(sqlite3* db, const char* name, const char* category) {
    char sql[300];
    sprintf(sql, "SELECT * FROM restaurant WHERE NAME = '%s' AND CATEGORY = '%s';", name, category);

    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "문장 실행 실패: %s\n", sqlite3_errmsg(db));
        return -1;
    }

    rc = sqlite3_step(stmt);

    if (rc == SQLITE_ROW) {
        // 이미 동일한 항목이 있음
        sqlite3_finalize(stmt);
        return 1;
    }
    else if (rc == SQLITE_DONE) {
        // 중복 없음
        sqlite3_finalize(stmt);
        return 0;
    }
    else {
        fprintf(stderr, "데이터 가져오기 실패: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return -1;
    }
}

// CSV 데이터 파싱 및 삽입 함수
void parseAndInsertData(sqlite3* db, const char* csvData) {
    char* token;
    char* rest = csvData;
    int restaurantId = 1;

    while ((token = strtok_s(rest, "\n", &rest))) {
        Restaurant restaurant;

        strcpy(restaurant.NAME, strtok(token, ","));
        strcpy(restaurant.CATEGORY, strtok(NULL, ","));

        // 중복 확인
        if (isDuplicate(db, restaurant.NAME, restaurant.CATEGORY) == 1) {
            printf("중복된 항목 추가: %s, %s\n", restaurant.NAME, restaurant.CATEGORY);
            continue;
        }

        // 랜덤 평점 생성
        int rating = getRandomRating();


        float dummy_lf = 0.00;
        // 데이터 삽입
        char sql[300];
        sprintf(sql, "INSERT INTO restaurant ( NAME, rating_sum, CATEGORY, people, rating_avg) VALUES ( %s, %d, %s, %d, %lf);",
            restaurant.NAME, rating, restaurant.CATEGORY, rating, dummy_lf );

        char* err_msg = 0;
        int rc = sqlite3_exec(db, sql, 0, 0, &err_msg);

        if (rc != SQLITE_OK) {
            fprintf(stderr, "SQL 오류: %s\n", err_msg);
            sqlite3_free(err_msg);
            sqlite3_close(db);
            exit(1);
        }
    }
}

int main() {
    sqlite3* db;
    int result = openDatabase(&db);

    if (result == -1) {
        printf("데이터베이스를 열 수 없습니다.\n");
        return 1;
    }

    result = createTable(db);

    if (result == -1) {
        printf("테이블을 생성할 수 없습니다.\n");
        return 1;
    }

    // CSV 파일 경로
    const char* csvFilePath = "rest3.csv";

    // CSV 파일에서 데이터 읽기
    char* csvData = readCSVFile(csvFilePath);

    // 데이터 파싱 및 삽입
    parseAndInsertData(db, csvData);

    free(csvData);

    printf("데이터 삽입이 완료되었습니다.\n");

    // 데이터베이스 닫기
    sqlite3_close(db);

    return 0;
}
