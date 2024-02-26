#include <sqlite3.h>
#include <stdio.h>

int selectData(sqlite3* db) {
    sqlite3_stmt* stmt;
    const char* select_query = "SELECT * FROM restaurant;";

    int rc = sqlite3_prepare_v2(db, select_query, -1, &stmt, 0);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return rc;
    }

    printf("restaurant_no\tNAME\tRATING\tCATEGORY\n");

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        int restaurant_no = sqlite3_column_int(stmt, 0);
        const unsigned char* name = sqlite3_column_text(stmt, 1);
        int rating = sqlite3_column_int(stmt, 2);
        const unsigned char* category = sqlite3_column_text(stmt, 3);

        printf("%d\t%s\t%d\t%s\n", restaurant_no, name, rating, category);
    }

    sqlite3_finalize(stmt);

    if (rc != SQLITE_DONE) {
        fprintf(stderr, "Failed to fetch data: %s\n", sqlite3_errmsg(db));
        return rc;
    }

    return SQLITE_OK;
}

int main() {
    sqlite3* db;
    int result = sqlite3_open("restaurants.db", &db);

    if (result != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return result;
    }

    // SELECT ���� ����
    result = selectData(db);

    if (result != SQLITE_OK) {
        fprintf(stderr, "Failed to select data\n");
    }

    // �����ͺ��̽� �ݱ�
    sqlite3_close(db);

    return 0;
}
