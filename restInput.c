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
#define DBNAME "test.db"

void restInput();//SUPER
sqlite3* openDataBase(const char* filename);
int isKoreanWhiteSpace(char c);
void getRestName(char* name, sqlite3* db);
void getRestRating(int* rating);
void getCategoryNumber(int* rating);
void getRestCategory(char* category);
void getDistance(int* rating);
void getAddress(char* address);
int insertData(sqlite3* db, char* restName, int rating, char* restCategory, int distance, char* address);
void closeDataBase(sqlite3* db);

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

	closeDataBase(db);
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
	char input[50]; // 입력을 저장할 문자열

	while (1) {
		printf("\n식당의 별점을 0점에서 5점 사이로 입력해주세요!: ");

		fgets(input, sizeof(input), stdin); // 문자열 입력 받기

		char* token1 = strtok(input, " "); // 첫 번째 문자열 토큰 분리
		char* token2 = strtok(NULL, " "); // 두 번째 문자열 토큰 분리
		//중간에 뭘 띄워썼을 경우 token2가 null이 아닐 것
		if (token1 == NULL || token2 != NULL || !isdigit(token1[0])) {
			printf("\t입력이 잘못되었습니다. 정수 범위인 0 이상 5 이하의 값을 입력해주세요.\n\n");
		}
		else {
			int tmp = atoi(token1); // 문자열을 정수로 변환
			if (tmp < 0 || tmp > 5) {
				printf("\t입력이 잘못되었습니다. 정수 범위인 0 이상 5 이하의 값을 입력해주세요.\n\n");
			}
			else {
				*rating = tmp;
				break;
			}
		}
	}
}

void getCategoryNumber(int* rating) {
	char input[50]; // 입력을 저장할 문자열

	while (1) {
		printf("\n식당 분류에 맞는 숫자를 입력해주세요\n\n1.한식 2.일식 3.중식 4.서양식 5.베이커리 6.동남아시아\n7.브런치 8.피자치킨 9.해물생선 10.고기 11.햄버거 12.분식\n: ");

		fgets(input, sizeof(input), stdin); // 문자열 입력 받기

		size_t len = strlen(input);
		if (input[len - 1] == '\n') {
			input[len - 1] = '\0';
			len--;
		}

		if (len == 1 && isdigit(input[0])) {
			int tmp = atoi(input);
			if (tmp >= 0) {
				*rating = tmp;
				break;
			}
		}
		else if (len == 2 && isdigit(input[0]) && isdigit(input[1])) {
			int tmp = atoi(input);
			if (tmp <= 12 && tmp >= 10) {
				*rating = tmp;
				break;
			}
		}
		printf("\t입력이 잘못되었습니다. 정수 범위인 1 이상 12 이하의 값을 입력해주세요.\n\n");
	}
}

void getRestCategory(char* category) {
	int input;

	getCategoryNumber(&input);
			switch (input) {
			case 1:
				strcpy(category, "한식");
				break;
			case 2:
				strcpy(category, "일식");
				break;
			case 3:
				strcpy(category, "중식");
				break;
			case 4:
				strcpy(category, "서양식");
				break;
			case 5:
				strcpy(category, "베이커리");
				break;
			case 6:
				strcpy(category, "동남아시아");
				break;
			case 7:
				strcpy(category, "브런치");
				break;
			case 8:
				strcpy(category, "피자치킨");
				break;
			case 9:
				strcpy(category, "해물생선");
				break;
			case 10:
				strcpy(category, "고기");
				break;
			case 11:
				strcpy(category, "햄버거");
				break;
			case 12:
				strcpy(category, "분식");
				break;
			}
			return;
}

void getDistance(int* rating) {
	char input[100]; // 입력을 저장할 문자열

	while (1) {
		printf("\n식당의 거리를 600미터 이하로 입력해 주세요: ");

		fgets(input, sizeof(input), stdin); // 문자열 입력 받기

		size_t len = strlen(input);
		if (input[len - 1] == '\n') {
			input[len - 1] = '\0';
			len--;
		}

		if (len == 1 && isdigit(input[0])) {
			int tmp = atoi(input);
			if (tmp >= 0 && tmp <=9) {
				*rating = tmp;
				//printf("입력된 숫자는: %d\n", tmp);
				break;
			}
		}
		else if (len == 2 && isdigit(input[0]) && isdigit(input[1])) {
			int tmp = atoi(input);
			if (tmp <= 99 && tmp >= 10) {
				*rating = tmp;
				//printf("입력된 숫자는: %d\n", tmp);
				break;
			}
		} 
		else if (len == 3 && isdigit(input[0]) && isdigit(input[1]) && isdigit(input[2])) {
			int tmp = atoi(input);
			if (tmp <= 600 && tmp >= 100) {
				*rating = tmp;
				//printf("입력된 숫자는: %d\n", tmp);
				break;
			}
		}
		printf("\t입력이 잘못되었습니다. 정수 0 이상 600 이하의 값을 입력해주세요.\n\n");
	}
}

void getAddress(char* address) {
	char tmp[500];
	while (1) {
		printf("\n식당의 주소를 입력해 주세요: ");
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
	printf("\n이름: %s, 별점: %d, 분류: %s 이 잘 추가되었습니다! \n\n", restName, rating, restCategory);
}

void closeDataBase(sqlite3* db) {
	sqlite3_close(db);
}