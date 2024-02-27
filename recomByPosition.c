#include <stdio.h>
#include <sqlite3.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

#define dbName "test.db"

void enterStoreRating(struct Restaurant* selectedRestaurant, char* memberId);

struct Restaurant {
	int restaurant_no;
	const unsigned char* nameAdr;
	char name[100];
	double rating_avg;
	const unsigned char* categoryAdr;
	char category[50];
};

void recomByPosition(char * userId)
{
	char* id = userId;

	sqlite3* db;
	sqlite3_stmt* res;
	int err_msg = 0;
	const char* tail;
	char tmp[100];

	char positions[4][20] = { "�ӿ�", "å��", "����", "���" };
	int pos_choice;
	int rest_choice;

	struct Restaurant* restaurants = malloc(3 * sizeof(struct Restaurant));
	if (!restaurants) {
		fprintf(stderr, "��°� �ӽù迭 �Ҵ翡 �����߽��ϴ�.");
	}

	int rc = sqlite3_open(dbName, &db);
	if (rc != SQLITE_OK) {
		fprintf(stderr, "db open error: %s \n", sqlite3_errmsg(db));
		sqlite3_close(db);
		return;
	}

	while (1) 
	{
		printf("� ������ ��ȣ�ϴ� �������� ��ȸ�Ͻðڽ��ϱ�?\n");   //��ȸ�� ���� ����
		printf("�ӿ�: 1 \n");
		printf("å��: 2 \n");
		printf("����: 3 \n");
		printf("���: 4 \n");

		gets(tmp);
		pos_choice = atoi(tmp);

		if (strcspn(tmp, "\n") != 1 || pos_choice < 1 || pos_choice > 4) {
			printf("�߸��� �Է��Դϴ�. \n");
		}

		else {
			break;
		}

	}


	char* sql = "SELECT RK.POSITION, R.RESTAURANT_NO, R.NAME, RK.RATINGS, R.CATEGORY "
		"FROM RESTAURANT R, "
		"(SELECT M.POSITION, R.RESTAURANT_NO, "
		"AVG(RR.RATING) RATINGS, "
		"ROW_NUMBER() OVER(PARTITION BY M.POSITION ORDER BY RR.RATING DESC) RANK "
		"FROM RESTAURANT_REVIEW RR, "
		"MEMBER M, "
		"RESTAURANT R "
		"WHERE RR.MEMBER_ID = M.MEMBER_ID "
		"AND R.RESTAURANT_NO = RR.RESTAURANT_NO "
		"GROUP BY M.POSITION, R.RESTAURANT_NO) RK "
		"WHERE R.RESTAURANT_NO = RK.RESTAURANT_NO "
		"AND RK.POSITION = ? "
		"AND RK.RANK < 4 ORDER BY RANK;";


	rc = sqlite3_prepare_v2(db, sql, -1, &res, &tail);

	if (rc != SQLITE_OK) {
		fprintf(stderr, "sql error: %s\n", sqlite3_errmsg(db));
		sqlite3_free(err_msg);
		sqlite3_close(db);
		return;
	}

	sqlite3_bind_int(res, 1, pos_choice);

	int i = 0;

	printf("%s ������ ��ȣ�ϴ� ���� \n", positions[pos_choice - 1]);
	while (sqlite3_step(res) == SQLITE_ROW) {
		restaurants[i].restaurant_no = sqlite3_column_int(res, 1);
		restaurants[i].nameAdr = sqlite3_column_text(res, 2);
		strcpy(restaurants[i].name, restaurants[i].nameAdr);

		restaurants[i].rating_avg = sqlite3_column_double(res, 3);
		restaurants[i].categoryAdr = sqlite3_column_text(res, 4);
		strcpy(restaurants[i].category, restaurants[i].categoryAdr);

		printf("%d: %-30s, %.1lf, %s\n\n", i + 1, restaurants[i].nameAdr, restaurants[i].rating_avg, restaurants[i].categoryAdr);
		i++;
	}

	sqlite3_finalize(res);
	sqlite3_close(db);

	while (1) {
		printf("�湮�Ͻ� �Ĵ� ��ȣ(1~3)�� �Է��� �ּ���: ");
		gets(tmp);
		rest_choice = atoi(tmp);
		
		if (strcspn(tmp, "\n") != 1 || rest_choice < 1 || rest_choice > 3) {
			printf("�߸��� �Է��Դϴ�. ��ȣ�� �ٽ� �Է��� �ּ���.\n");
		}

		else {
			struct Restaurant* selectedRestaurant = malloc(sizeof(struct Restaurant));

			selectedRestaurant->restaurant_no = restaurants[rest_choice - 1].restaurant_no;
			selectedRestaurant->nameAdr = restaurants[rest_choice - 1].nameAdr;
			strcpy(selectedRestaurant->name, restaurants[rest_choice - 1].name);
			selectedRestaurant->rating_avg = restaurants[rest_choice - 1].rating_avg;
			selectedRestaurant->categoryAdr = restaurants[rest_choice - 1].categoryAdr;
			strcpy(selectedRestaurant->category, restaurants[rest_choice - 1].category);

			printf("������ �Ĵ�: %s \n", restaurants[rest_choice - 1].name);

			enterStoreRating(selectedRestaurant, userId);
			break;
		}
	}

	
	return;
}
