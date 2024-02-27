// recomByCategory
// 카테고리별로 맛집을 추천받습니다.
#include <sqlite3.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define DBNAME "test.db"

sqlite3* openDataBase(const char* filename);
void recomByCategory(char* userId);
struct Restaurant* selectFromRestaurant(sqlite3* db, const char* category);
void getRecomCategoryNum(int* switchNum);
void getVisitRestNum(int* selection);

struct Restaurant {
	int restaurant_no;
	const unsigned char* nameAdr;
	char name[100];
	double rating_avg;
	const unsigned char* categoryAdr;
	char category[50];
};


void recomByCategory(char* userId) {
	char* id = userId; //로그인한 유저의 아이디를 받습니다.
	sqlite3* db = openDataBase(DBNAME);

	int switchNum;
	struct Restaurant* selectedRestaurant;

	getRecomCategoryNum(&switchNum);

	switch (switchNum) {
	case 1:
		selectedRestaurant = selectFromRestaurant(db, "한식");
		break;
	case 2:
		selectedRestaurant = selectFromRestaurant(db, "일식");
		break;
	case 3:
		selectedRestaurant = selectFromRestaurant(db, "중식");
		break;
	case 4:
		selectedRestaurant = selectFromRestaurant(db, "서양식");
		break;
	case 5:
		selectedRestaurant = selectFromRestaurant(db, "베이커리");
		break;
	case 6:
		selectedRestaurant = selectFromRestaurant(db, "동남아시아");
		break;
	case 7:
		selectedRestaurant = selectFromRestaurant(db, "브런치");
		break;
	case 8:
		selectedRestaurant = selectFromRestaurant(db, "피자치킨");
		break;
	case 9:
		selectedRestaurant = selectFromRestaurant(db, "해물생선");
		break;
	case 10:
		selectedRestaurant = selectFromRestaurant(db, "고기");
		break;
	case 11:
		selectedRestaurant = selectFromRestaurant(db, "햄버거");
		break;
	case 12:
		selectedRestaurant = selectFromRestaurant(db, "분식");
		break;
	}

	//식당 선택 후 평가 남기는 함수를 호출합니다. 
	enterStoreRating(selectedRestaurant, id);

	sqlite3_close(db);
	return;
}

struct Restaurant* selectFromRestaurant(sqlite3* db, const char* category) {

	sqlite3_stmt* stmt;
	int maxSelectRows = 10;
	// 그냥 초기 데이터 기반으로 restaurant 테이블만 조회하는 쿼리
	const char* select_query2 =
		"SELECT restaurant_no, name, rating_avg, category "
		"FROM restaurant "
		"WHERE category = ? "
		"ORDER BY rating_avg DESC, name ASC "
		"LIMIT 10;";

	int rc = sqlite3_prepare_v2(db, select_query2, -1, &stmt, 0);
	if (rc != SQLITE_OK) {
		printf("구문 준비에 실패했습니다.");
		fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
		return;
	}
	sqlite3_bind_text(stmt, 1, category, -1, SQLITE_STATIC);
	printf("\n%s 카테고리의 평점순 추천 리스트 입니다!\n\n", category);
	printf("---------------------------------------------------\n");
	printf("번호|           식당이름             | 평점 | 분류\n");
	printf("---------------------------------------------------\n");

	rc = sqlite3_step(stmt);
	int row_count = 0;

	struct Restaurant* restaurants = malloc(maxSelectRows * sizeof(struct Restaurant));
	if (!restaurants) {
		fprintf(stderr, "출력값 임시배열 할당에 실패했습니다.");
		sqlite3_finalize(stmt);
	}

	if (rc == SQLITE_ROW && row_count < maxSelectRows) {
		int printNum = 1;
		while (rc == SQLITE_ROW) {
			restaurants[row_count].restaurant_no = sqlite3_column_int(stmt, 0);
			restaurants[row_count].nameAdr = sqlite3_column_text(stmt, 1);
			strcpy(restaurants[row_count].name, restaurants[row_count].nameAdr);

			restaurants[row_count].rating_avg = sqlite3_column_double(stmt, 2);
			restaurants[row_count].categoryAdr = sqlite3_column_text(stmt, 3);
			strcpy(restaurants[row_count].category, restaurants[row_count].categoryAdr);

			int name_length = strlen(restaurants[row_count].name);
			int additional_spaces = 30 - (name_length / 3 * 2);
			if (additional_spaces < 0) {
				additional_spaces = 0; // 음수가 되지 않도록 보정
			}
			printf("%2d: | %s%*s | %.2lf | %s \n", printNum,restaurants[row_count].name, additional_spaces, "", restaurants[row_count].rating_avg, restaurants[row_count].categoryAdr);
			printNum++;
			rc = sqlite3_step(stmt);
			row_count++;
		}
	}
	else if (rc == SQLITE_DONE || row_count == 0) {
		printf("결과가 없습니다.\n");
		free(restaurants);
	}
	else {
		fprintf(stderr, "데이터를 가져오는 데 실패했습니다: %s\n", sqlite3_errmsg(db));
	}
	printf("---------------------------------------------------\n");
	printf("\n");
	sqlite3_finalize(stmt);


	int selection;
	char input[50];

	getVisitRestNum(&selection);
	struct Restaurant* selectedRestaurant = malloc(sizeof(struct Restaurant));

	selectedRestaurant->restaurant_no = restaurants[selection - 1].restaurant_no;
	selectedRestaurant->nameAdr = restaurants[selection - 1].nameAdr;
	strcpy(selectedRestaurant->name, restaurants[selection - 1].name);
	selectedRestaurant->rating_avg = restaurants[selection - 1].rating_avg;
	selectedRestaurant->categoryAdr = restaurants[selection - 1].categoryAdr;
	strcpy(selectedRestaurant->category, restaurants[selection - 1].category);

	free(restaurants);

	return selectedRestaurant;
}

void getRecomCategoryNum(int* switchNum) {
	while (1) {
		char input[50]; // 입력을 저장할 문자열
		printf("카테고리별로 맛집을 추천해 드릴게요!\n\n");
		printf("1.한식 2.일식 3.중식 4.서양식 5.베이커리 6.동남아시아\n7.브런치 8.피자치킨 9.해물생선 10.고기 11.햄버거 12.분식\n\n");
		printf("원하는 카테고리를 숫자로 입력해 주세요: ");

		fgets(input, sizeof(input), stdin); // 문자열 입력 받기

		size_t len = strlen(input);
		if (input[len - 1] == '\n') {
			input[len - 1] = '\0';
			len--;
		}

		if (len == 1 && isdigit(input[0])) {
			int tmp = atoi(input);
			if (tmp >= 1 && tmp <=9) {
				*switchNum = tmp;
				break;
			}
		}
		else if (len == 2 && isdigit(input[0]) && isdigit(input[1])) {
			int tmp = atoi(input);
			if (tmp <= 12 && tmp >= 10) {
				*switchNum = tmp;
				break;
			}
		}
		printf("\t입력이 잘못되었습니다. 정수 범위인 1 이상 12 이하의 값을 입력해주세요.\n\n");
	}
}

void getVisitRestNum(int * selection) {
	char input[50];

	while (1) {
		printf("방문하실 식당 번호를 입력해 주세요: ");

		fgets(input, sizeof(input), stdin); // 문자열 입력 받기

		size_t len = strlen(input);
		if (input[len - 1] == '\n') {
			input[len - 1] = '\0';
			len--;
		}

		if (len == 1 && isdigit(input[0])) {
			int tmp = atoi(input);
			if (tmp >= 1) {
				*selection = tmp;
				//printf("입력된 숫자는: %d\n", tmp);
				break;
			}
		}
		else if (len == 2 && isdigit(input[0]) && isdigit(input[1])) {
			int tmp = atoi(input);
			if (tmp == 10) {
				*selection = tmp;
				//printf("입력된 숫자는: %d\n", tmp);
				break;
			}
		}
		printf("\t입력이 잘못되었습니다. 올바른 식당 번호를 입력해 주세요.");
	}
}