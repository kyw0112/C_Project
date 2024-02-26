//별점 평가와 한줄평을 db에 추가합니다.
#include <sqlite3.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <time.h>

#define DBNAME "test.db"
#define MAX_CONTENT_LENGTH 51
//헤더 등으로 나중에 빼줍시다
struct Restaurant {
	int restaurant_no;
	const unsigned char* nameAdr;
	char name[100];
	double rating_avg;
	const unsigned char* categoryAdr;
	char category[50];
};

void enterStoreRating(struct Restaurant* selectedRestaurant, char *memberId) {

	sqlite3* db = openDataBase(DBNAME);
	int rating;
	char content[MAX_CONTENT_LENGTH];

	printRatingInfo(selectedRestaurant);
	// 별점 평가 입력
	getSelectedRating(&rating,selectedRestaurant->name);
	// 평가 댓글 입력
	getSelectedContent(content);
	//Restaurant_review 테이블에 행 추가
	printf("여기인가2\n");

	insertStoreRating(db, memberId, rating, content, selectedRestaurant->restaurant_no);
	//printf("%d, %s", rating, content);
	// restaurant 테이블 업데이트
	// ratingsum에 rating 추가, people + 1, rating_avg 재계산
	// 변수로 내부에 다 받아서 한 번에 업데이트 해주기 
	sqlite3_close(db);
}

printRatingInfo(struct Restaurant* selectedRestaurant) {
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

getSelectedRating(int* rating, char * restName) {
	int tmp;

	while (1) {
		printf("%s의 식사 경험을 0점에서 5점 사이로 평가해 주세요: ",restName);

		if (scanf("%d", &tmp) != 1 || tmp < 0 || tmp >5) {
			printf("\t입력이 잘못되었습니다. 0 이상 5 이하의 값을 입력해주세요.\n\n");
			while (getchar() != '\n');
		}
		else {
			*rating = tmp;
			break;
		}
	}
	while (getchar() != '\n');
}

getSelectedContent(char* restName) {
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

//review_id, create_time, rating, content, member_id, restaurant_no
int insertStoreRating(sqlite3* db, char *memberId, int rating, char* content, int restaurant_no) {

	time_t currentTime = time(NULL);
	char timeString[20];
	strftime(timeString, sizeof(timeString),"%Y-%m-%d %H:%M:%S", localtime(&currentTime));
	printf("여기인가3\n");

	char sql[150];
	char* err_msg = NULL;
	printf("여기인가4\n");

	sprintf(sql, "INSERT INTO restaurant_review (create_time, rating, content, member_id, restaurant_no) VALUES('%s', '%d', '%s', '%s', '%d');", timeString, rating, content, memberId,restaurant_no);
	printf("여기인가5\n");

	int rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
	printf("여기인가6\n");

	if (rc != SQLITE_OK)
	{
		fprintf(stderr, "SQL error: %s\n", err_msg);

		sqlite3_free(err_msg);
		return 1;
	}
	//printf("이름: %s, 별점: %d, 분류: %s 이 잘 추가되었습니다! \n\n", restName, rating, restCategory);
	printf("잘 추가되었습니다? \n");
}