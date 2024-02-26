// recomByCategory
// 카테고리별로 맛집을 추천받습니다.
#include <sqlite3.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define DBNAME "test.db"

struct Restaurant {
	int restaurant_no;
	const unsigned char* nameAdr;
	char name[100];
	double rating_avg;
	const unsigned char* categoryAdr;
	char category[50];
};

void selectJoinReview(sqlite3* db, const char* category);
struct Restaurant* selectFromRestaurant(sqlite3* db, const char* category);

void recomByCategory(char * userId) {
	char* id = userId; //로그인한 유저의 아이디를 받는다고 가정

	sqlite3* db = openDataBase(DBNAME);
	int condition = 1;

	while (condition) {
		printf("카테고리별로 맛집을 추천해 드릴게요!\n");
		printf("1.한식 2양식 3중식 4.일식\n");
		printf("원하는 카테고리를 숫자로 입력해 주세요: ");
		struct Restaurant* selectedRestaurant;
		int input;
		if (scanf("%d", &input) != 1 || input < 0 || input >4) {
			printf("\t입력이 잘못되었습니다. 카테고리를 다시 입력해주세요.\n\n");
			while (getchar() != '\n');
		}
		else {
			switch (input) {
			case 1:
				selectedRestaurant = selectFromRestaurant(db, "한식");
				enterStoreRating(selectedRestaurant, id);
				return;
			case 2:
				selectedRestaurant = selectFromRestaurant(db, "양식");
				enterStoreRating(selectedRestaurant, id);
				return;
			case 3:
				selectedRestaurant = selectFromRestaurant(db, "중식");
				enterStoreRating(selectedRestaurant, id);
				return;
			case 4:
				selectedRestaurant = selectFromRestaurant(db, "일식");
				enterStoreRating(selectedRestaurant, id);
				return;
			}
		}
	}
	sqlite3_close(db);
}

void selectJoinReview(sqlite3* db, const char* category) {

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

	int rc = sqlite3_prepare_v2(db, select_query, -1, &stmt, 0);

	if (rc != SQLITE_OK) {
		printf("구문 준비에 실패했습니다.");
		fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
		return;
	}
	//stmt: 쿼리문, 파라미터 인덱스는 1부터 시작(? 순서),바인딩값,-1은 길이자동계산,바인딩타입)
	sqlite3_bind_text(stmt, 1, category, -1, SQLITE_STATIC); // 파라미터 바인딩

	printf("\n%s 카테고리의 최다리뷰 및 평균 별점 순 데이터입니다.\n", category);

	rc = sqlite3_step(stmt);
	if (rc == SQLITE_ROW) {
		while (rc == SQLITE_ROW) {
			const unsigned char* name = sqlite3_column_text(stmt, 0);
			double avg_rating = sqlite3_column_double(stmt, 1);
			const unsigned char* category = sqlite3_column_text(stmt, 2);
			printf("%s\t%.2lf\t%s\n", name, avg_rating, category);
			rc = sqlite3_step(stmt);
		}
	}
	else if (rc == SQLITE_DONE) {
		printf("결과가 없습니다.\n");
	}
	else {
		fprintf(stderr, "데이터를 가져오는 데 실패했습니다: %s\n", sqlite3_errmsg(db));
	}

	printf("\n");
	sqlite3_finalize(stmt);
}

struct Restaurant* selectFromRestaurant(sqlite3* db, const char* category) {

	sqlite3_stmt* stmt;

	// 그냥 초기 데이터 기반으로 restaurant 테이블만 조회하는 쿼리
	const char* select_query2 =
		"SELECT restaurant_no, name, rating_avg, category "
		"FROM restaurant "
		"WHERE category = ? "
		"ORDER BY rating_avg DESC, name ASC "
		"LIMIT 5;";

	int rc = sqlite3_prepare_v2(db, select_query2, -1, &stmt, 0);
	if (rc != SQLITE_OK) {
		printf("구문 준비에 실패했습니다.");
		fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
		return;
	}

	sqlite3_bind_text(stmt, 1, category, -1, SQLITE_STATIC);

	printf("\n%s 카테고리의 별점 순 추천리스트 입니다.\n", category);

	rc = sqlite3_step(stmt);
	int row_count = 0;

	struct Restaurant* restaurants = malloc(5 * sizeof(struct Restaurant));
	if (!restaurants) {
		fprintf(stderr, "출력값 임시배열 할당에 실패했습니다.");
		sqlite3_finalize(stmt);
	}

	if (rc == SQLITE_ROW && row_count < 5) {
		int printNum = 1;
		while (rc == SQLITE_ROW) {
			restaurants[row_count].restaurant_no = sqlite3_column_int(stmt, 0);
			restaurants[row_count].nameAdr = sqlite3_column_text(stmt, 1);
			strcpy(restaurants[row_count].name, restaurants[row_count].nameAdr);

			restaurants[row_count].rating_avg = sqlite3_column_double(stmt, 2);
			restaurants[row_count].categoryAdr = sqlite3_column_text(stmt, 3);
			strcpy(restaurants[row_count].category, restaurants[row_count].categoryAdr);
			printf("%d: %d, %s, %lf, %s\n",printNum, restaurants[row_count].restaurant_no, restaurants[row_count].nameAdr, restaurants[row_count].rating_avg, restaurants[row_count].categoryAdr);
			printNum++;
			rc = sqlite3_step(stmt);
			row_count++;
		}
	}
	else if (rc == SQLITE_DONE || row_count ==0) {
		printf("결과가 없습니다.\n");
		free(restaurants);
	}
	else {
		fprintf(stderr, "데이터를 가져오는 데 실패했습니다: %s\n", sqlite3_errmsg(db));
	}
	printf("\n");
	sqlite3_finalize(stmt);

	while (1) {
		printf("방문하실 식당 번호를 입력해 주세요: ");
		int selection;
		if (scanf("%d", &selection) != 1 || selection <= 0 || selection > row_count) {
			printf("\t입력이 잘못되었습니다. 번호를 다시 입력해 주세요.\n\n");
			while (getchar() != '\n');
		}
		else {
			struct Restaurant* selectedRestaurant = malloc(sizeof(struct Restaurant));

			selectedRestaurant->restaurant_no = restaurants[selection - 1].restaurant_no;
			selectedRestaurant->nameAdr = restaurants[selection - 1].nameAdr;
			strcpy(selectedRestaurant->name, restaurants[selection - 1].name);
			selectedRestaurant->rating_avg = restaurants[selection - 1].rating_avg;
			selectedRestaurant->categoryAdr = restaurants[selection - 1].categoryAdr;
			strcpy(selectedRestaurant->category, restaurants[selection - 1].category);

			return selectedRestaurant;
		}
	}
}


//testSelectRestaurant(sqlite3* db) {
//	sqlite3_stmt* stmt;
//	const char* select_query = "SELECT * FROM restaurant;";
//
//	int rc = sqlite3_prepare_v2(db, select_query, -1, &stmt, 0);
//
//	if (rc != SQLITE_OK) {
//		fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
//		return rc;
//	}
//
//	printf("restaurant\tNAME\tRATING\tCATEGORY\n");
//
//	while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
//		int restaurant_no = sqlite3_column_int(stmt, 0);
//		const unsigned char* name = sqlite3_column_text(stmt, 1);
//		int rating = sqlite3_column_int(stmt, 2);
//		const unsigned char* category = sqlite3_column_text(stmt, 3);
//
//		printf("%d\t%s\t%d\t%s\n", restaurant_no, name, rating, category);
//	}
//
//	sqlite3_finalize(stmt);
//
//	if (rc != SQLITE_DONE) {
//		fprintf(stderr, "Failed to fetch data: %s\n", sqlite3_errmsg(db));
//		return rc;
//	}
//
//}