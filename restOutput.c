//restOutput.c
/*
	����ڰ� ��ü ��ȸ�� �����ϸ� ���� ����Ʈ�� ù �������� �����ش�.
	����ڴ� ��ư �Է��� ���� �� �� �������� �̵��� �� �ִ�.
*/


#include <stdio.h>
#include <sqlite3.h>
#include <stdlib.h>
#include <string.h>

#define PAGE_SZ 20
#define REST_CNT 2000
#define dbName "test.db"


void restOutput()
{
	struct restaurant {
		int rest_no;
		char r_name[30];
		double rating_avg;
		char ctgry[20];
	} REST[REST_CNT];

	int actual_cnt = 0;
	char tmp[100];

	sqlite3* db;
	sqlite3_stmt* res;
	char* err_msg = 0;
	const char* tail;

	int crrnt_pg = 1;
	int page_bttn;

	int rc = sqlite3_open(dbName, &db);
	if (rc != SQLITE_OK) {
		fprintf(stderr, "db open error: %s \n", sqlite3_errmsg(db));
		sqlite3_close(db);
		return;
	}

	char* sql = "SELECT * FROM RESTAURANT;";

	rc = sqlite3_prepare_v2(db, sql, -1, &res, &tail); //���� ������
	if (rc != SQLITE_OK) {
		fprintf(stderr, "sql error: %s\n", err_msg);
		sqlite3_free(err_msg);
		sqlite3_close(db);
		return;
	}

	int i = 0;
	while (sqlite3_step(res) == SQLITE_ROW) {       //���� ��� �� �پ� ó��

		REST[i].rest_no = sqlite3_column_int(res, 0);
		strcpy(REST[i].r_name, sqlite3_column_text(res, 1));
		REST[i].rating_avg = sqlite3_column_double(res, 5);
		strcpy(REST[i].ctgry, sqlite3_column_text(res, 3));
		i++;
	}

	actual_cnt = i;

	while (1)
	{
		printf("=================================================== \n");
		printf("  ��ȣ          ������        ����          �з�  \n");
		printf("=================================================== \n");
		for (i = (crrnt_pg - 1) * PAGE_SZ; (i < crrnt_pg * PAGE_SZ) && i <= actual_cnt; i++) {
			printf("%5d %30s %5.1lf %20s \n", REST[i].rest_no, REST[i].r_name, REST[i].rating_avg, REST[i].ctgry);
		}
		printf("=================================================== \n");
		printf("%d page \n\n", crrnt_pg);

		do {
			printf("���� ������ �̵�: 1 \n");
			printf("���� ������ �̵�: 2 \n");
			printf("����: 3 \n\n");
			gets(tmp);
			page_bttn = atoi(tmp);
		} while (strcspn(tmp, "\n") != 1 || page_bttn < 1 || page_bttn > 3);

		if (page_bttn == 1) {
			if (crrnt_pg == 1) {
				printf("\n\nù ��° �������Դϴ�. \n");
			}
			else {
				crrnt_pg--;
			}
		}
		
		else if (page_bttn == 2) {
			if (crrnt_pg * PAGE_SZ >= actual_cnt) {
				printf("\n\n������ �������Դϴ�. \n");
			}
			else {
				crrnt_pg++;
			}
		}
			
		else if (page_bttn == 3) {
			break;
		}
		
		else 
		{
			printf("�߸� �Է��߽��ϴ�. \n");
		}
	

	}

	sqlite3_close(db);

	return;
}