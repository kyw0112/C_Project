#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sqlite3.h>

#define DB_PATH "test.db" // 데이터베이스 파일 경로 정의

// 평점과 거리를 이용하여 거성비 점수 계산
double calculateScore(double rating, double distance) {
    if (distance == 0) return rating; // 거리가 0이면 평점 그대로 반환
    return rating * rating / log(distance + 1); // 거리에 로그를 취하여 거성비 점수 계산
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
void recomByStarPerDist(char* userId) {
    char tmp[100]; // 입력을 받을 변수
    int rest_choice; // 선택된 식당의 번호를 저장할 변수
    char* id = userId;

    struct Restaurant {
        int restaurant_no;
        char name[100];
        double rating_avg;
    };

    struct Restaurant* selectedRestaurant = malloc(sizeof(struct Restaurant)); // 메모리 할당

    sqlite3* db;
    sqlite3_stmt* stmt;
    int rc;

    rc = sqlite3_open(DB_PATH, &db); // 데이터베이스 열기
    if (rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return;
    }

    const char* sql = "SELECT name, rating_avg, distance FROM restaurant;"; // SQL 쿼리

    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL); // SQL 쿼리 실행 준비
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    // 배열 초기화
    char* names[1000]; // 문자열 포인터 배열로 수정
    double scores[1000];
    double ratings[1000];
    double distances[1000];
    int count = 0;

    // 쿼리 결과 가져오기
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        // 문자열 복사
        const unsigned char* name_from_db = sqlite3_column_text(stmt, 0);
        names[count] = (char*)malloc((strlen((const char*)name_from_db) + 1) * sizeof(char)); // 메모리 할당
        strcpy(names[count], (const char*)name_from_db); // 복사
        ratings[count] = sqlite3_column_double(stmt, 1);
        distances[count] = sqlite3_column_double(stmt, 2);
        double score = calculateScore(ratings[count], distances[count]);
        scores[count] = score;
        count++;
    }

    // 점수에 따라 배열 정렬
    bubbleSort(scores, names, ratings, distances, count);

    // 정렬된 결과 출력 (상위 5개만)
    printf("번호\t식당 이름\t      평점     거리   거성비 점수\n");
    printf("------------------------------------------------\n");

    for (int i = 0; i < count && i < 5; i++) {
        // 추가적인 공백 계산
        int name_length = strlen(names[i]);
        int additional_spaces = 20 - (name_length / 3 * 2);
        if (additional_spaces < 0) {
            additional_spaces = 0; // 음수가 되지 않도록 보정
        }

        printf("%d\t%s%*s\t%.1f\t%.0f\t%.2f\n",
            i + 1, names[i], additional_spaces, "", // 추가적인 공백 출력
            ratings[i], distances[i], scores[i]);

    }

    // 사용자로부터 식당 선택 받기
    while (1) {
        printf("방문하실 식당 번호(1~5)를 입력해 주세요: ");
        fgets(tmp, sizeof(tmp), stdin);
        sscanf(tmp, "%d", &rest_choice);

        if (rest_choice < 1 || rest_choice > 5) {
            printf("잘못된 입력입니다. 번호를 다시 입력해 주세요.\n");
        }
        else {
            selectedRestaurant->restaurant_no = rest_choice; // 선택된 번호를 할당
            strcpy(selectedRestaurant->name, names[rest_choice - 1]); // 이름 복사
            selectedRestaurant->rating_avg = ratings[rest_choice - 1]; // 평점 복사

            break;
        }
    }

    // 여기서 선택된 식당 정보를 사용하여 다음 단계로 진행하면 됩니다.

    // 선택된 식당 정보 출력
    printf("선택된 식당 번호: %d, 이름: %s, 평점: %.1f\n", selectedRestaurant->restaurant_no, selectedRestaurant->name, selectedRestaurant->rating_avg);
    enterStoreRating(selectedRestaurant, id);
    // 메모리 해제
    free(selectedRestaurant);
    selectedRestaurant = NULL;


    sqlite3_finalize(stmt); // SQL 문 종료
    sqlite3_close(db); // 데이터베이스 종료
}

// 메인 함수

