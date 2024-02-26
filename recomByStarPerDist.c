#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sqlite3.h>

#define DB_PATH "test.db" // 데이터베이스 파일 경로 정의

// 평점과 거리를 이용하여 거성비 점수 계산
double calculateScore(double rating, double distance) {
    if (distance == 0) return rating; // 거리가 0이면 평점 그대로 반환
    return rating / log(distance + 1); // 거리에 로그를 취하여 거성비 점수 계산
}

// 두 변수의 값을 교환하는 함수
void swap(double* a, double* b) {
    double temp = *a;
    *a = *b;
    *b = temp;
}

// 버블 정렬을 사용하여 점수 배열을 내림차순으로 정렬하는 함수
void bubbleSort(double scores[], char* names[], double ratings[], double distances[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (scores[j] < scores[j + 1]) { // 점수가 내림차순이 되도록 조건 검사
                // 점수 배열과 그에 따른 다른 배열들을 교환하여 정렬
                swap(&scores[j], &scores[j + 1]);
                char* temp_name = names[j];
                names[j] = names[j + 1];
                names[j + 1] = temp_name;
                swap(&ratings[j], &ratings[j + 1]);
                swap(&distances[j], &distances[j + 1]);
            }
        }
    }
}

// 별점 대 거리에 따른 추천 함수
void recomByStarPerDist() {
    sqlite3* db;
    sqlite3_stmt* stmt;
    int rc;

    rc = sqlite3_open(DB_PATH, &db); // 데이터베이스 열기
    if (rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return;
    }

    const char* sql = "SELECT name, rating, distance FROM restaurant;"; // SQL 쿼리

    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL); // SQL 쿼리 실행 준비
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    // 배열 초기화
    char* names[100]; // 문자열 포인터 배열로 수정
    double scores[100];
    double ratings[100];
    double distances[100];
    int count = 0;

    // 쿼리 결과 가져오기
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        // 문자열 복사
        const unsigned char* name_from_db = sqlite3_column_text(stmt, 0);
        names[count] = (char*)malloc(strlen(name_from_db) + 1); // 메모리 할당
        strcpy(names[count], (const char*)name_from_db); // 복사
        ratings[count] = sqlite3_column_double(stmt, 1);
        distances[count] = sqlite3_column_double(stmt, 2);
        double score = calculateScore(ratings[count], distances[count]);
        scores[count] = score;
        count++;
    }

    // 점수에 따라 배열 정렬
    bubbleSort(scores, names, ratings, distances, count);

    // 정렬된 결과 출력
    printf("식당 이름\t평점\t거리\t거성비 점수\n");
    printf("-----------------------------------------\n");
    for (int i = 0; i < count; i++) {
        printf("%s\t\t%.2f\t%.2f\t%.2f\n", names[i], ratings[i], distances[i], scores[i]);
        free(names[i]); // 메모리 해제
    }

    sqlite3_finalize(stmt); // SQL 문 종료
    sqlite3_close(db); // 데이터베이스 종료
}

// 메인 함수
int main() {
    recomByStarPerDist(); // 함수 호출
    return 0;
}
