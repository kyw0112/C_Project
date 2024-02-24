//restInput

// 디비 중복 확인

#include <sqlite3.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_NAME_LENGTH 51
#define MMAX_CATEGORY_LENGTH 20
#define DBNAME "test.db"

sqlite3* openDataBase(const char* filename);

void restInput() {
	sqlite3* db = openDataBase(DBNAME);

	char restName[MAX_NAME_LENGTH];
	int rating;
	char restCategory[20];

	printf("새로운 식당 정보를 추가합니다!\n\n");
	getRestName(restName);
	getRestRating(&rating);
	getRestCategory(restCategory);


	printf("이름: %s, 별점: %d, 분류: %s", restName, rating, restCategory);
	//�Էµ� ������ db�� insert

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

getRestName(char* name) {
	char tmp[100];
	int len;

	while (1) {
		printf("식당의 이름을 입력해주세요: ");
		fgets(tmp, sizeof(tmp), stdin);
		//gets_s(tmp, sizeof(tmp));

		tmp[strcspn(tmp, "\n")] = '\0';

		//���� �Է� ����
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

		//���� �� Ȯ��
		if (len > MAX_NAME_LENGTH - 1) {
			printf("이름이 너무 깁니다. %d자 이내로 입력해 주세요.\n", (MAX_NAME_LENGTH - 1));
			continue;
		}

		// �Է°� Ȯ��
		while (1) {
			printf("이름 '%s'이 맞나요? (y/n): ", tmp);
			char answer = getchar();
			while (getchar() != '\n');
			//getchar(&answer);
			//scanf("%c", &answer);
			//getchar();

			//��� Ȯ��

			if (toupper(answer) == 'Y') {
				strcpy(name, tmp);
				return;
			}
			else if (toupper(answer) == 'N') {
				break;
			}
			else {
				printf("잘못된 입력입니다.. y/n 중에 입력해주세요.\n");
				continue;
			}
		}

	}
}

getRestRating(int* rating) {
	//char tmp[100]; 
	int input;

	while (1) {
		printf("식당의 별점을 0점에서 5점 사이로 입력해주세요!: ");

		if (scanf("%d", &input) != 1 || input < 0 || input >5) {
			printf("입력이 잘못되었습니다. 0 이상 5 이하의 값을 입력해주세요.\n"); // 한글 부분 수정
			while (getchar() != '\n');
		}
		else {
			*rating = input;
			break;
		}
	}
}

getRestCategory(char* category) {
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
