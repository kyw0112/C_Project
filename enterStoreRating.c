// bj
// enterStoreRating
//별점 평가와 한줄평을 db에 추가합니다. 추가된 데이터를 바탕으로 테이블을 업데이트합니다.
#include <sqlite3.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <time.h>

#define DBNAME "test.db"
#define MAX_CONTENT_LENGTH 51

void printRatingInfo(struct Restaurant* selectedRestaurant);
void getSelectedRating(int* rating, char* restName);
void getSelectedContent(char* restName);
int insertStoreRating(sqlite3* db, char* memberId, int rating, char* content, int restaurant_no);
struct TOUPDATERESTAURANT* loadOgData(sqlite3* db, struct Restaurant* selectedRestaurant, int rating);
void updateRestaurant(struct TOUPDATERESTAURANT* toUpdateData);

//헤더 등으로 나중에 빼줍시다
struct Restaurant {
	int restaurant_no;
	const unsigned char* nameAdr;
	char name[100];
	double rating_avg;
	const unsigned char* categoryAdr;
	char category[50];
};

struct TOUPDATERESTAURANT {
	int restNo;
	int ratingSum;
	int people;
	double rating_avg;
};

void enterStoreRating(struct Restaurant* selectedRestaurant, char* memberId) {
	sqlite3* db = openDataBase(DBNAME);
	int rating;
	char content[MAX_CONTENT_LENGTH];

	printRatingInfo(selectedRestaurant);
	getSelectedRating(&rating, selectedRestaurant->name);
	getSelectedContent(content);
	insertStoreRating(db, memberId, rating, content, selectedRestaurant->restaurant_no);
	struct TOUPDATERESTAURANT* toUpdateData = loadOgData(db, selectedRestaurant, rating);
	//loadOgData에서 selectFromRestaurant의 메모리 사용 후 해제를 해줍니다.
	updateRestaurant(db, toUpdateData);
	//updateRestaurant에서 loadOgData에서 할당했던 메모리를 해제해 줍니다.
	free(toUpdateData);
}

void printRatingInfo(struct Restaurant* selectedRestaurant) {
	printf("\n번호: %d, 이름: %s, 점수: %lf, 카테고리: %s\n\n", selectedRestaurant->restaurant_no, selectedRestaurant->name, selectedRestaurant->rating_avg, selectedRestaurant->category);
	printf("\n%s에서 맛있는 식사 하세요!\n\n", selectedRestaurant->name);

	printf("\n식 사 중");
	Sleep(1000);
	printf(" . ");
	Sleep(1000);
	printf(" . ");
	Sleep(1000);
	printf(" . \n");
	Sleep(1000);
	printf("\n맛있게 식사 하셨습니까?\n");
}

void getSelectedRating(int* rating, char* restName) {
	int tmp;

	while (1) {
		printf("%s의 식사 경험을 0점에서 5점 사이로 평가해 주세요: ", restName);

		if (scanf("%d", &tmp) != 1 || tmp < 0 || tmp >5) {
			printf("\t입력이 잘못되었습니다. 0 이상 5 이하의 값을 입력해주세요.\n\n");
			while (getchar() != '\n');
		} else {
			*rating = tmp;
			break;
		}
	}
	while (getchar() != '\n');
}

void getSelectedContent(char* restName) {
	char tmp[500];
	while (1) {
		printf("식당에 대한 한줄평을 작성해 주세요: ");
		memset(tmp, 0, sizeof(tmp));
		fflush(stdin);

		fgets(tmp, sizeof(tmp), stdin);
		tmp[strcspn(tmp, "\n")] = '\0';

		int validInput = 0;
		for (int i = 0; tmp[i] != '\0'; i++) {
			if (!isKoreanWhiteSpace(tmp[i])) {
				validInput = 1;
				break;
			}
		}
		if (!validInput) {
			printf("\t공백을 입력하셨습니다. 잘못된 입력입니다.\n\n");
			continue;
		}

		if (strlen(tmp) > MAX_CONTENT_LENGTH - 1) {
			printf("\t한줄평이 너무 깁니다. %d자 이내로 입력해 주세요.\n\n", (MAX_CONTENT_LENGTH - 1));
			continue;
		}
		strcpy(restName, tmp);
		break;
	}
}

int insertStoreRating(sqlite3* db, char* memberId, int rating, char* content, int restaurant_no) {

	time_t currentTime = time(NULL);
	char timeString[20];
	strftime(timeString, sizeof(timeString), "%Y-%m-%d %H:%M:%S", localtime(&currentTime));

	char sql[300];
	char* err_msg = NULL;
	sprintf(sql, "INSERT INTO restaurant_review (create_time, rating, content, member_id, restaurant_no) VALUES('%s', '%d', '%s', '%s', '%d');", timeString, rating, content, memberId, restaurant_no);
	int rc = sqlite3_exec(db, sql, 0, 0, &err_msg);

	if (rc != SQLITE_OK)
	{
		fprintf(stderr, "SQL error: %s\n", err_msg);

		sqlite3_free(err_msg);
		return 1;
	}
}

struct TOUPDATERESTAURANT* loadOgData(sqlite3* db, struct Restaurant* selectedRestaurant, int rating) {

	int ogRestNo;
	int ogRatingSum;
	int ogPeople;
	double ogRatingAvg;

	sqlite3_stmt* stmt;

	const char* load_query =
		"SELECT restaurant_no, rating_sum, people, rating_avg "
		"FROM restaurant "
		"WHERE restaurant_no = ?;";

	int rc = sqlite3_prepare_v2(db, load_query, -1, &stmt, 0);
	if (rc != SQLITE_OK) {
		printf("구문 준비에 실패했습니다.");
		fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
		return;
	}

	int bindNum = selectedRestaurant->restaurant_no;
	sqlite3_bind_int(stmt, 1, selectedRestaurant->restaurant_no, -1, SQLITE_STATIC);

	rc = sqlite3_step(stmt);

	if (rc == SQLITE_ROW) {
		ogRestNo = sqlite3_column_int(stmt, 0);
		ogRatingSum = sqlite3_column_int(stmt, 1);
		ogPeople = sqlite3_column_int(stmt, 2);
		ogRatingAvg = sqlite3_column_double(stmt, 3);
		rc = sqlite3_step(stmt);
	}
	else if (rc == SQLITE_DONE) {
		printf("결과가 없습니다.\n");
	}
	else {
		fprintf(stderr, "데이터를 가져오는 데 실패했습니다: %s\n", sqlite3_errmsg(db));
	}

	sqlite3_finalize(stmt);

	int updateRatingSum = ogRatingSum + rating;
	int updatePeople = ogPeople + 1;
	double updateRatingAvg = (double)updateRatingSum / updatePeople;

	//selectFromRestaurant에서 할당해준 메모리를 해제해줍니다 !!!
	free(selectedRestaurant);

	struct TOUPDATERESTAURANT* toUpdateRestaurant = malloc(sizeof(struct TOUPDATERESTAURANT));
	if (toUpdateRestaurant == NULL) {
		fprintf(stderr, "메모리 할당 실패\n");
		exit(1);
	}
	toUpdateRestaurant->restNo = ogRestNo;
	toUpdateRestaurant->ratingSum = updateRatingSum;
	toUpdateRestaurant->people = updatePeople;
	toUpdateRestaurant->rating_avg = updateRatingAvg;

	return toUpdateRestaurant;
}

void updateRestaurant(sqlite3* db, struct TOUPDATERESTAURANT* toUpdateData) {

	const char* update_query =
		"UPDATE restaurant "
		"SET rating_sum = ?, "
		"    people = ?, "
		"    rating_avg = ? "
		"WHERE restaurant_no = ?;";

	sqlite3_stmt* stmt;
	int rc = sqlite3_prepare_v2(db, update_query, -1, &stmt, 0);
	if (rc != SQLITE_OK) {
		fprintf(stderr, "쿼리 준비에 실패했습니다: %s\n", sqlite3_errmsg(db));
		return;
	}

	sqlite3_bind_int(stmt, 1, toUpdateData->ratingSum);
	sqlite3_bind_int(stmt, 2, toUpdateData->people);
	sqlite3_bind_double(stmt, 3, toUpdateData->rating_avg);
	sqlite3_bind_int(stmt, 4, toUpdateData->restNo);

	rc = sqlite3_step(stmt);
	if (rc != SQLITE_DONE) {
		fprintf(stderr, "쿼리 실행에 실패했습니다: %s\n", sqlite3_errmsg(db));
	}
	sqlite3_finalize(stmt);
	printf("소중한 의견이 DB에 잘 반영되었습니다!\n");
}