#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PAGE_SZ 20
#define REST_CNT 2000

// 음식 카테고리 선택 함수
void selectFoodCategory(sqlite3* db, int categoryNum) {
    char* category;
    int i = 0;
    int actual_cnt;
    char tmp[100];

    struct RESTAURANT {
        char name[30];
        char addr[100];
        int addit_spaces;
    } REST[REST_CNT];

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
    /*while (sqlite3_step(stmt) == SQLITE_ROW) {
        printf("식당 이름: %-20s, 주소: %s\n",
            sqlite3_column_text(stmt, 1),
            sqlite3_column_text(stmt, 7));


    }*/

    // 쿼리 결과를 구조체에 저장
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        const char* restaurant_name = (const char*)sqlite3_column_text(stmt, 1);
        const char* address = (const char*)sqlite3_column_text(stmt, 7);
        int name_length = strlen(restaurant_name);
        int additional_spaces = 28 - (name_length / 3 * 2);
        if (additional_spaces < 0) {
            additional_spaces = 0; // 음수가 되지 않도록 보정
        }

        strcpy(REST[i].name, restaurant_name);
        strcpy(REST[i].addr, address);
        REST[i].addit_spaces = additional_spaces;

        i++;
        //printf("식당 이름: %s%*s 주소: %s \n",
            //restaurant_name,
            //additional_spaces, "", // 추가적인 공백 출력
            //address
        //);
    }

    actual_cnt = i;

    sqlite3_finalize(stmt);


    //음식점 목록을 페이지양식으로 출력
    int crrnt_page = 1;
    int page_ctrl;

    while (1)
    {
        printf("=========================================================================== \n");
        printf("             음식점                주소  \n");
        printf("============================================================================ \n");
        if (i-20 > actual_cnt)
            break;
        
        for (i = (crrnt_page - 1) * PAGE_SZ; (i < crrnt_page * PAGE_SZ) && (i <= actual_cnt); i++) {
            printf("식당 이름: %s%*s 주소: %s \n",
                REST[i].name,
                REST[i].addit_spaces, "", // 추가적인 공백 출력
                REST[i].addr
            );
        }
        printf("============================================================================ \n");
        printf("%d page \n\n", crrnt_page);

        do {
            
            printf("이전 페이지 이동: 1 \n");
            printf("다음 페이지 이동: 2 \n");
            printf("종료: 3 \n\n");
            gets(tmp);
            page_ctrl = atoi(tmp);
        } while (strcspn(tmp, "\n") != 1 || page_ctrl < 1 || page_ctrl > 3);

        if (page_ctrl == 1) {
            if (crrnt_page == 1) {
                printf("\n\n첫 번째 페이지입니다. \n");
            }
            else {
                crrnt_page--;
            }
        }

        else if (page_ctrl == 2) {
            if (crrnt_page * PAGE_SZ >= actual_cnt) {
                printf("\n\n마지막 페이지입니다. \n");
            }
            else {
                crrnt_page++;
            }
        }

        else if (page_ctrl == 3) {
            break;
        }

        else
        {
            printf("잘못 입력했습니다. \n");
        }


    }



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
    printf("음식 카테고리를 선택하세요 (1. 한식, 2. 일식, 3. 중식, 4. 서양식, 5. 베이커리, 6. 동남아시아, 7. 브런치, 8. 피자치킨, 9. 해물생선, 10. 고기, 11. 햄버거, 12. 분식): ");   scanf("%d%*c", &categoryNum);

    selectFoodCategory(db, categoryNum);

    sqlite3_close(db);
}