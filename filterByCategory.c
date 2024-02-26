#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>

// 음식 카테고리 선택 함수
void selectFoodCategory(sqlite3* db, int categoryNum) {
    char* category;
    switch (categoryNum) {
    case 1:
        category = "한식";
        break;
    case 2:
        category = "일식";
        break;
    case 3:
        category = "중식";
        break;
    case 4:
        category = "서양식";
        break;
    case 5:
        category = "베이커리";
        break;
    case 6:
        category = "동남아시아";
        break;
    case 7:
        category = "브런치";
        break;
    case 8:
        category = "피자치킨";
        break;
    case 9:
        category = "해물생선";
        break;
    case 10:
        category = "고기";
        break;
    case 11:
        category = "햄버거";
        break;
    case 12:
        category = "분식";
        break;
    default:
        printf("잘못된 선택입니다.\n");
        return;
    }

    char sql[1000];
    sqlite3_stmt* stmt;

    // 카테고리에 따른 음식을 선택하는 SQL 쿼리
    sprintf(sql, "SELECT * FROM restaurant WHERE category='%s';", category);

    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL 오류: %s\n", sqlite3_errmsg(db));
        return;
    }

    printf("선택한 카테고리의 음식:\n");
    // 결과 출력
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        printf("식당 이름: %s, 주소: %s\n",
            sqlite3_column_text(stmt, 1),
            sqlite3_column_text(stmt, 7));
            
        
    }

    sqlite3_finalize(stmt);
}

void filterByCategory() {
    sqlite3* db;
    int rc = sqlite3_open("test.db", &db);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "데이터베이스를 열 수 없습니다: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }

    int categoryNum;
    printf("음식 카테고리를 선택하세요 (1. 한식, 2. 일식, 3. 중식, 4. 서양식, 5. 베이커리, 6. 동남아시아, 7. 브런치, 8. 피자치킨, 9. 해물생선, 10. 고기, 11. 햄버거, 12. 분식): ");   scanf("%d", &categoryNum);

    selectFoodCategory(db, categoryNum);

    sqlite3_close(db);
}
