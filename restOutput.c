#include <stdio.h>
#include <sqlite3.h>
#include <stdlib.h>
#include <string.h>

#define PAGE_SZ 20
#define REST_CNT 1000

struct restaurant {
	int rest_no;
	char r_name[30];
	int rating;
	char ctgry[20];
} REST[REST_CNT];

void restOutput(void)
{

	sqlite3* db;
	sqlite3_stmt* res;
	char* err_msg = 0;
	const char* tail;

	int crrnt_pg = 1;
	char page_bttn;

	int rc = sqlite3_open("test.db", &db);
	if (rc != SQLITE_OK) {
		fprintf(stderr, "db open error: %s \n", sqlite3_errmsg(db));
		sqlite3_close(db);
		return;
	}

	char* sql = "SELECT * FROM RESTAURANT;";

	rc = sqlite3_prepare_v2(db, sql, 1000, &res, &tail); //쿼리 컴파일
	if (rc != SQLITE_OK) {
		fprintf(stderr, "sql error: %s\n", err_msg);
		sqlite3_free(err_msg);
		sqlite3_close(db);
		return;
	}



	int i = 0;
	while (sqlite3_step(res) == SQLITE_ROW) {       //쿼리 결과 한 줄씩 처리

		REST[i].rest_no = sqlite3_column_int(res, 0);
		strcpy(REST[i].r_name, sqlite3_column_text(res, 1));
		REST[i].rating = sqlite3_column_int(res, 2);
		strcpy(REST[i].ctgry, sqlite3_column_text(res, 3));
		i++;
	}



	while (1)
	{
		printf("=================================================== \n");
		printf("  번호          음식점        별점          분류  \n");
		printf("=================================================== \n");
		for (i = (crrnt_pg - 1) * PAGE_SZ; i < crrnt_pg * PAGE_SZ; i++) {
			printf("%5d %30s %5d %20s \n", REST[i].rest_no, REST[i].r_name, REST[i].rating, REST[i].ctgry);
		}
		printf("=================================================== \n");
		printf("%d page \n\n", crrnt_pg);

		printf("이전 페이지 이동: a \n");
		printf("다음 페이지 이동: d \n");
		printf("종료: q \n\n");
		page_bttn = getchar();
		getchar();

		if (page_bttn == 'a' || page_bttn == 'A') {
			if (crrnt_pg == 1) {
				printf("\n\n첫 번째 페이지입니다. \n");
			}
			else {
				crrnt_pg--;
			}
		}
		else if (page_bttn == 'd' || page_bttn == 'D') {
			if (crrnt_pg * PAGE_SZ >= REST_CNT) {
				printf("\n\n마지막 페이지입니다. \n");
			}
			else {
				crrnt_pg++;
			}
		}
		else if (page_bttn == 'q' || page_bttn == 'Q') {
			break;
		}


	}

	sqlite3_close(db);

	return;
}