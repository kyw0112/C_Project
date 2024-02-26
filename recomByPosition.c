#include <stdio.h>
#include <sqlite3.h>
#include <stdlib.h>
#include <string.h>
// 리뷰를 같이 보여주는 기능 추가 필요 보여준다면 몇 개 보여줄건지..

void recomByPosition(void)
{
	sqlite3* db;
	sqlite3_stmt* res;
	int err_msg = 0;
	const char* tail;
	char again;

	char positions[4][20] = { "staff", "manager", "책임", "임원" };
	char target_pos[20];
	int choice;

	int rc = sqlite3_open("test.db", &db);
	if (rc != SQLITE_OK) {
		fprintf(stderr, "db open error: %s \n", sqlite3_errmsg(db));
		sqlite3_close(db);
		return;
	}

	while (1) {
		do {
			printf("어떤 직급이 선호하는 맛집을 조회하시겠습니까?\n");
			printf("사원: 1 \n");
			printf("선임: 2 \n");
			printf("책임: 3 \n");
			printf("임원: 4 \n");

			scanf("%d%", &choice);
			while (getchar() != '\n');

		} while (choice < 1 || 4 < choice);

		strcpy(target_pos, positions[choice - 1]);

		char* sql = "SELECT RK.POSITION, RK.RANK, R.NAME, RK.RATINGS "
			"FROM RESTAURANT R, "
			"(SELECT M.POSITION, R.RESTAURANT RESTAURANT_NO, "
			"AVG(RR.RATING) RATINGS, "
			"RANK() OVER(PARTITION BY M.POSITION ORDER BY RR.RATING DESC) RANK "
			"FROM RESTAURANT_REVIEW RR, "
			"MEMBER M, "
			"RESTAURANT R "
			"WHERE RR.MEMBER_NO = M.MEMBER_NO "
			"AND R.RESTAURANT = RR.RESTAURANT_NO "
			"GROUP BY M.POSITION, R.RESTAURANT) RK "
			"WHERE R.RESTAURANT = RK.RESTAURANT_NO "
			"AND RK.POSITION = ? "
			"AND RK.RANK < 4;";


		rc = sqlite3_prepare_v2(db, sql, -1, &res, &tail);

		if (rc != SQLITE_OK) {
			fprintf(stderr, "sql error: %s\n", sqlite3_errmsg(db));
			sqlite3_free(err_msg);
			sqlite3_close(db);
			return;
		}

		sqlite3_bind_text(res, 1, target_pos, -1, SQLITE_STATIC);

		printf("%s 직급이 선호하는 맛집 \n", target_pos);
		printf("=================================================== \n");
		printf("   포지션    순위            맛집         평균평점 \n");
		printf("=================================================== \n");
		while (sqlite3_step(res) == SQLITE_ROW) {
			printf("%10s ", sqlite3_column_text(res, 0));
			printf("%5s ", sqlite3_column_text(res, 1));
			printf("%20s ", sqlite3_column_text(res, 2));
			printf("%8s \n", sqlite3_column_text(res, 3));
		}
		printf("=================================================== \n");

		printf("다시 조회를 원하면 y, 종료를 원하면 다른 버튼 입력 \n");
		again = getchar();
		while (getchar() != '\n');

		if ((again != 'y') && (again != 'Y')) {
			break;
		}

	}

	sqlite3_close(db);
	return;
}