// bj
// restInput
// 새로운 식당 정보를 추가하는 기능을 구현합니다. (이름, 별점, 분류)

#include <sqlite3.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_NAME_LENGTH 51
#define MAX_CATEGORY_LENGTH 20
#define MAX_ADDRESS_LENGTH 300
#define DBNAME "test.db" //추후 사용할 db의 이름으로 변경하면 됩니다. (csv 로드 할 db)

void restInput(); //아래 함수들을 호출하는 부모 함수.
sqlite3* openDataBase(const char* filename);//db 접속 (에러 체크)
int isKoreanWhiteSpace(char c); // 공백 입력 방지, 공백 체크 함수
void getRestName(char* name);// 식당의 이름을 입력받습니다. (공백, 길이, 입력 재확인, db중복확인)
void getRestRating(int* rating);//평점을 입력받습니다.(0~5점 입력 체크)
void getRestCategory(char* category);//분류를 입력받습니다.(정수형 입력)
void getDistance(int* distance);
void getAddress(char* address);
int insertData(sqlite3* db, char* restName, int rating, char* restCategory, int distance, char* address);//입력값들을 통해 테이블에 행을 추가합니다.


void restInput() {
	sqlite3* db = openDataBase(DBNAME);

	char restName[MAX_NAME_LENGTH];
	int rating;
	char restCategory[MAX_CATEGORY_LENGTH];
	int distance;
	char address[MAX_ADDRESS_LENGTH];

	printf("새로운 식당 정보를 추가합니다!\n\n");
	getRestName(restName, db);
	getRestRating(&rating);
	getRestCategory(restCategory);
	getDistance(&distance);
	getAddress(address);
	insertData(db, restName, rating, restCategory, distance, address);

	sqlite3_close(db);
	return 0;
}

sqlite3* openDataBase(const char* filename) {
	sqlite3* db;
	char* err_msg = NULL;
	int returnCode = sqlite3_open(filename, &db);

	if (returnCode != SQLITE_OK) {
		printf(stderr, "DB 접속에 실패했습니다. : %s\n", sqlite3_errmsg(db));

		sqlite3_close(db);
		return 1;
	}
	return db;
}

int isKoreanWhiteSpace(char c) {
	// 한글 공백 범위: 0x0020 - 0x007E, 0x00A1 - 0x00FF
	return (c == 0x0020 || c == 0x00A0 || (c >= 0x2000 && c <= 0x200A) || c == 0x3000);
}

void getRestName(char* name, sqlite3* db) {
	char tmp[500];

	while (1) {
		printf("식당의 이름을 입력해주세요: ");
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

		if (strlen(tmp) > MAX_NAME_LENGTH - 1) {
			printf("\t이름이 너무 깁니다. %d자 이내로 입력해 주세요.\n\n", (MAX_NAME_LENGTH - 1));
			continue;
		}

		while (1) {
			printf("이름 '%s'이 맞나요? (y/n): ", tmp);
			char answer = getchar();
			while (getchar() != '\n');

			if (toupper(answer) == 'Y') {
				char query[100];
				sprintf(query, "SELECT NAME FROM RESTAURANT WHERE NAME = '%s'", tmp);
				sqlite3_stmt* stmt;
				int rc = sqlite3_prepare_v2(db, query, -1, &stmt, NULL);
				if (rc != SQLITE_OK) {
					printf("\t쿼리 준비 중 오류가 발생했습니다.\n\n");
					continue;
				}
				rc = sqlite3_step(stmt);
				if (rc == SQLITE_ROW) {
					printf("\t이미 존재하는 식당입니다.\n\n");
					break;
				}
				sqlite3_finalize(stmt);
				strcpy(name, tmp);
				return;
			}
			else if (toupper(answer) == 'N') {
				break;
			}
			else {
				printf("\t잘못된 입력입니다.. y/n 중에 입력해주세요.\n\n");
				continue;
			}
		}
	}
}

void getRestRating(int* rating) {
	int tmp;

	while (1) {
		printf("식당의 별점을 0점에서 5점 사이로 입력해주세요!: ");

		if (scanf("%d", &tmp) != 1 || tmp < 0 || tmp >5) {
			printf("\t입력이 잘못되었습니다. 0 이상 5 이하의 값을 입력해주세요.\n\n");
			while (getchar() != '\n');
		}
		else {
			*rating = tmp;
			break;
		}
	}
}

void getRestCategory(char* category) {
	int input;

	while (1) {
		printf("식당 분류에 맞는 숫자를 입력해주세요\n 1.한식 2.양식 3.중식 4.일식\n: ");

		if (scanf("%d", &input) != 1 || input < 0 || input >4) {
			printf("\t입력이 잘못되었습니다. 음식 종류를 다시 입력해주세요.\n\n");
			while (getchar() != '\n');
		}
		else {
			switch (input) {
			case 1:
				strcpy(category, "한식");
				break;
			case 2:
				strcpy(category, "양식");
				break;
			case 3:
				strcpy(category, "중식");
				break;
			case 4:
				strcpy(category, "일식");
				break;
			}
			return;
		}
	}
}

void getDistance(int* distance) {
	int tmp;

	while (1) {
		printf("식당의 거리를 600미터 이하로 입력해 주세요: ");

		if (scanf("%d", &tmp) != 1 || tmp < 0 || tmp >600) {
			printf("\t입력이 잘못되었습니다. 0 이상 600 이하의 값을 입력해주세요.\n\n");
			while (getchar() != '\n');
		}
		else {
			*distance = tmp;
			break;
		}
	}
	while (getchar() != '\n');
}

void getAddress(char* address) {
	char tmp[500];
	while (1) {
		printf("식당의 주소를 입력해 주세요: ");
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

		if (strlen(tmp) > MAX_ADDRESS_LENGTH - 1) {
			printf("\t주소가 너무 깁니다. %d자 이내로 입력해 주세요.\n\n", (MAX_ADDRESS_LENGTH - 1));
			continue;
		}
		strcpy(address, tmp);
		break;
	}
}

int insertData(sqlite3* db, char* restName, int rating, char* restCategory, int distance, char* address) {
	char sql[500];
	char* err_msg = NULL;
	sprintf(sql, "INSERT INTO RESTAURANT (NAME, RATING_SUM, CATEGORY, PEOPLE, RATING_AVG, DISTANCE, ADDRESS) VALUES('%s', '%d', '%s','%d','%lf', '%d', '%s');", restName, rating, restCategory, 1, (double)rating, distance, address);
	int rc = sqlite3_exec(db, sql, 0, 0, &err_msg);

	if (rc != SQLITE_OK)
	{
		fprintf(stderr, "SQL error: %s\n", err_msg);

		sqlite3_free(err_msg);
		return 1;
	}
	printf("이름: %s, 별점: %d, 분류: %s 이 잘 추가되었습니다! \n\n", restName, rating, restCategory);
}