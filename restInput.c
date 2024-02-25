//restInput

#include <sqlite3.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_NAME_LENGTH 51
#define MAX_CATEGORY_LENGTH 20
#define DBNAME "test.db"

void restInput();
sqlite3* openDataBase(const char* filename);
int isKoreanWhiteSpace(char c);
void getRestName(char* name);
void getRestRating(int* rating);
void getRestCategory(char* category);
int insertData(sqlite3* db, char* restName, int rating, char* restCategory);


void restInput() {
	sqlite3* db = openDataBase(DBNAME);

	char restName[MAX_NAME_LENGTH];
	int rating;
	char restCategory[MAX_CATEGORY_LENGTH];

	printf("새로운 식당 정보를 추가합니다!\n\n");
	getRestName(restName,db);
	getRestRating(&rating);
	getRestCategory(restCategory);
	insertData(db, restName, rating, restCategory);


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
	char tmp[100];
	int len;

	while (1) {
		printf("식당의 이름을 입력해주세요: ");
		//memset(tmp, 0, sizeof(tmp));
		fgets(tmp, sizeof(tmp), stdin);

		tmp[strcspn(tmp, "\n")] = '\0';
		fflush(stdin);

		int validInput = 0;
		for (int i = 0; tmp[i] != '\0'; i++) {
			if (!isKoreanWhiteSpace(tmp[i])) {
				validInput = 1;
				break;
			}
		}
		if (!validInput) {
			printf("공백을 입력하셨습니다. 잘못된 입력입니다.\n");
			continue;
		}

		len = strlen(tmp);

		if (len > MAX_NAME_LENGTH - 1) {
			printf("이름이 너무 깁니다. %d자 이내로 입력해 주세요.\n", (MAX_NAME_LENGTH - 1));
			continue;
		}

		while (1) {
			printf("이름 '%s'이 맞나요? (y/n): ", tmp);
			char answer = getchar();
			while (getchar() != '\n');

			if (toupper(answer) == 'Y') {
				strcpy(name, tmp);

				//------
				char query[100];
				sprintf(query, "SELECT NAME FROM RESTAURANT WHERE NAME = '%s'", name);
				sqlite3_stmt* stmt;
				int rc = sqlite3_prepare_v2(db, query, -1, &stmt, NULL);
				if (rc != SQLITE_OK) {
					printf("쿼리 준비 중 오류가 발생했습니다.\n");
					continue;
				}
				// 결과 확인
				rc = sqlite3_step(stmt);
				if (rc == SQLITE_ROW) {
					printf("이미 존재하는 식당입니다.\n");
					break;
				}
				sqlite3_finalize(stmt);
				return;
			} else if (toupper(answer) == 'N') {
				break;
			} else {
				printf("잘못된 입력입니다.. y/n 중에 입력해주세요.\n");
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
			printf("입력이 잘못되었습니다. 0 이상 5 이하의 값을 입력해주세요.\n"); // 한글 부분 수정
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
		printf("원하시는 음식 종류를 입력해주세요\n 1.한식 2.양식 3.중식 4.일식\n: "); // 한글 부분 수정

		if (scanf("%d", &input) != 1 || input < 0 || input >4) {
			printf("입력이 잘못되었습니다. 음식 종류를 다시 입력해주세요.\n"); // 한글 부분 수정
			while (getchar() != '\n');
		}
		else {
			switch (input) {
			case 1:
				strcpy(category, "한식"); // 한글 부분 수정
				break;
			case 2:
				strcpy(category, "양식"); // 한글 부분 수정
				break;
			case 3:
				strcpy(category, "중식"); // 한글 부분 수정
				break;
			case 4:
				strcpy(category, "일식"); // 한글 부분 수정
				break;
			}
			return;
		}
	}
}

int insertData(sqlite3* db, char* restName, int rating, char* restCategory) {
	char sql[150];
	char* err_msg = NULL;
	sprintf(sql, "INSERT INTO RESTAURANT (NAME, RATING, CATEGORY) VALUES('%s', '%d', '%s');", restName, rating, restCategory);
	int rc = sqlite3_exec(db, sql, 0, 0, &err_msg);

	if (rc != SQLITE_OK)
	{
		fprintf(stderr, "SQL error: %s\n", err_msg);

		sqlite3_free(err_msg);
		//sqlite3_close(db);
		return 1;
	}
	printf("이름: %s, 별점: %d, 분류: %s 이 잘 추가되었습니다! \n\n", restName, rating, restCategory);
}