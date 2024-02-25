// recomByCategory
// 카테고리별로 맛집을 추천받습니다.
#include <sqlite3.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define DBNAME "test.db"

void selectRestaurantByCategory(sqlite3* db, const char* category);

void recomByCategory() {
	sqlite3* db = openDataBase(DBNAME);


	while (1) {
		printf("카테고리별로 맛집을 추천해 드릴게요!\n");
		printf("1.한식 2양식 3중식 4.일식\n");
		printf("원하는 카테고리를 숫자로 입력해 주세요: ");

		int input;
		if (scanf("%d", &input) != 1 || input < 0 || input >4) {
			printf("\t입력이 잘못되었습니다. 카테고리를 다시 입력해주세요.\n\n");
			while (getchar() != '\n');
		}
		else {
			switch (input) {
			case 1:
				//한식 select
                selectRestaurantByCategory(db, "한식");
				break;
			case 2:
				//양식 select
				testSelectRestaurant(db);
                break;
			case 3:
				//중식 select
                break;
			case 4:
				//일식 select
                break;
			}
		}
	}
}

void selectRestaurantByCategory(sqlite3* db, const char* category) {

    sqlite3_stmt* stmt;
	//1. 리뷰를 조회 해서 출력하는 쿼리
    const char* select_query =
        "SELECT r.name, AVG(rr.rating) AS avg_rating, r.category "
        "FROM restaurant AS r "
        "INNER JOIN restaurant_review AS rr ON r.restaurant = rr.restaurant_no "
        "WHERE r.category = ? "
        "GROUP BY r.restaurant, r.name "
        "ORDER BY COUNT(rr.restaurant_no) DESC, avg_rating DESC "
        "LIMIT 5;";

	// 그냥 초기 데이터 기반으로 restaurant 테이블만 조회하는 쿼리
	const char* select_query2 =
		"SELECT name, rating, category "
		"FROM restaurant "
		"WHERE category = ? "
		"ORDER BY rating DESC, name ASC "
		"LIMIT 5;";



    //int rc = sqlite3_prepare_v2(db, select_query, -1, &stmt, 0);
    int rc = sqlite3_prepare_v2(db, select_query2, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        printf("구문 준비에 실패했습니다.");
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return;
    }
	//stmt: 쿼리문, 파라미터 인덱스는 1부터 시작(? 순서),바인딩값,-1은 길이자동계산,바인딩타입)
    sqlite3_bind_text(stmt, 1, category, -1, SQLITE_STATIC); // 파라미터 바인딩

    //printf("restaurant\tNAME\tAVERAGE RATING\n");
	printf("%s 카테고리의 최다리뷰 및 평균 별점 순 데이터입니다.\n",category);

    rc = sqlite3_step(stmt);
    //if (rc == SQLITE_ROW) {
    //    while (rc == SQLITE_ROW) {
    //        const unsigned char* name = sqlite3_column_text(stmt, 0);
    //        double avg_rating = sqlite3_column_double(stmt, 1);
    //        const unsigned char* category = sqlite3_column_text(stmt, 2);
    //        printf("%s\t%.2lf\t%s\n", name, avg_rating, category);
    //        rc = sqlite3_step(stmt);
    //    }
    //} else if (rc == SQLITE_DONE) {
    //    printf("결과가 없습니다.\n");
    //}
    //else {
    //    fprintf(stderr, "데이터를 가져오는 데 실패했습니다: %s\n", sqlite3_errmsg(db));
    //}

	if (rc == SQLITE_ROW) {
		while (rc == SQLITE_ROW) {
			const unsigned char* name = sqlite3_column_text(stmt, 0);
			double rating = sqlite3_column_double(stmt, 1);
			const unsigned char* category = sqlite3_column_text(stmt, 2);
			printf("%s\t%.2lf\t%s\n", name, rating, category);
			rc = sqlite3_step(stmt);
		}
	}
	else if (rc == SQLITE_DONE) {
		printf("결과가 없습니다.\n");
	}
	else {
		fprintf(stderr, "데이터를 가져오는 데 실패했습니다: %s\n", sqlite3_errmsg(db));
	}





    sqlite3_finalize(stmt);
}

testSelectRestaurant(sqlite3* db) {
	sqlite3_stmt* stmt;
	const char* select_query = "SELECT * FROM restaurant;";

	int rc = sqlite3_prepare_v2(db, select_query, -1, &stmt, 0);

	if (rc != SQLITE_OK) {
		fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
		return rc;
	}

	printf("restaurant\tNAME\tRATING\tCATEGORY\n");

	while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
		int restaurant_no = sqlite3_column_int(stmt, 0);
		const unsigned char* name = sqlite3_column_text(stmt, 1);
		int rating = sqlite3_column_int(stmt, 2);
		const unsigned char* category = sqlite3_column_text(stmt, 3);

		printf("%d\t%s\t%d\t%s\n", restaurant_no, name, rating, category);
	}

	sqlite3_finalize(stmt);

	if (rc != SQLITE_DONE) {
		fprintf(stderr, "Failed to fetch data: %s\n", sqlite3_errmsg(db));
		return rc;
	}

}