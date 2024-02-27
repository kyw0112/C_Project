#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>

sqlite3* db;

// 더치페이 테이블에 대한 SQL 문
const char* create_table_sql =
"CREATE TABLE IF NOT EXISTS dutch_pay ("
"   transaction_id INTEGER PRIMARY KEY,"
"   payer_id TEXT NOT NULL,"
"   receiver_id TEXT NOT NULL,"
"   amount REAL NOT NULL,"
"   create_time TEXT DEFAULT (strftime('%Y-%m-%d %H:%M:%S', 'now', 'localtime'))"
");";

// 빚 기록하는 함수
void record_debt(const char* payer_id, const char* receiver_id, double amount);

// 빚을 상계하는 함수
void reconcile_debts();

// 빚 기록하는 함수
void record_debt(const char* payer_id, const char* receiver_id, double amount) {
    // 이미 동일한 payer_id와 receiver_id를 가진 레코드가 있는지 확인하는 쿼리
    char* check_sql = sqlite3_mprintf("SELECT COUNT(*), amount FROM dutch_pay WHERE payer_id = '%s' AND receiver_id = '%s';", payer_id, receiver_id);
    sqlite3_stmt* stmt;
    int count = 0;
    double current_amount = 0.0;

    // 쿼리 실행
    if (sqlite3_prepare_v2(db, check_sql, -1, &stmt, NULL) == SQLITE_OK) {
        // 결과 확인
        int result = sqlite3_step(stmt);
        if (result == SQLITE_ROW) {
            count = sqlite3_column_int(stmt, 0);
            printf("%d", count);
            current_amount = sqlite3_column_double(stmt, 1);
        }
        else if (result == SQLITE_DONE) {
            // 결과가 없는 경우 count를 0으로 설정
            count = 0;
        }
    }
    sqlite3_finalize(stmt);
    sqlite3_free(check_sql);

    // 이미 동일한 레코드가 없는 경우에는 새로운 레코드를 생성
    if (count == 0) {
        // INSERT 문을 생성하여 빚을 기록
        char* sql = sqlite3_mprintf("INSERT INTO dutch_pay (payer_id, receiver_id, amount) VALUES ('%s', '%s', %f);", payer_id, receiver_id, amount);
        // SQL 문 실행
        sqlite3_exec(db, sql, NULL, NULL, NULL);
        // 메모리 해제
        sqlite3_free(sql);
        printf("빚이 성공적으로 기록되었습니다.\n");
    }
    else {
        // 이미 동일한 레코드가 있는 경우에는 어마운트를 추가하여 업데이트
        double new_amount = current_amount + amount;
        char* update_sql = sqlite3_mprintf("UPDATE dutch_pay SET amount = %f WHERE payer_id = '%s' AND receiver_id = '%s';", new_amount, payer_id, receiver_id);
        sqlite3_exec(db, update_sql, NULL, NULL, NULL);
        sqlite3_free(update_sql);
        printf("기존 레코드에 어마운트가 추가되었습니다.\n");
    }
}
// 특정 사용자가 빚진 사람의 리스트를 확인하는 함수
void check_debts_to_me(const char* user_id) {
    // 특정 사용자에게 빚진 사람의 리스트를 확인하는 쿼리
    char* payer_sql = sqlite3_mprintf("SELECT DISTINCT payer_id FROM dutch_pay WHERE receiver_id = '%s';", user_id);
    sqlite3_stmt* payer_stmt;

    printf("%s님이 빚진 사람의 리스트:\n", user_id);

    // 쿼리 실행
    if (sqlite3_prepare_v2(db, payer_sql, -1, &payer_stmt, NULL) == SQLITE_OK) {
        // 결과 반복 처리
        while (sqlite3_step(payer_stmt) == SQLITE_ROW) {
            const char* payer_id = (const char*)sqlite3_column_text(payer_stmt, 0);
            printf("%s\n", payer_id);
        }
    }

    // 스테이트먼트 해제
    sqlite3_finalize(payer_stmt);
    sqlite3_free(payer_sql);
}

// 특정 사용자에게 빚진 사람의 리스트를 확인하는 함수
void check_debts_from_me(const char* user_id) {
    // 특정 사용자에게 빚진 사람의 리스트를 확인하는 쿼리
    char* receiver_sql = sqlite3_mprintf("SELECT DISTINCT receiver_id FROM dutch_pay WHERE payer_id = '%s';", user_id);
    sqlite3_stmt* receiver_stmt;

    printf("%s님이 받은 빚의 리스트:\n", user_id);

    // 쿼리 실행
    if (sqlite3_prepare_v2(db, receiver_sql, -1, &receiver_stmt, NULL) == SQLITE_OK) {
        // 결과 반복 처리
        while (sqlite3_step(receiver_stmt) == SQLITE_ROW) {
            const char* receiver_id = (const char*)sqlite3_column_text(receiver_stmt, 0);
            printf("%s\n", receiver_id);
        }
    }

    // 스테이트먼트 해제
    sqlite3_finalize(receiver_stmt);
    sqlite3_free(receiver_sql);
}

// 빚을 상계하는 함수
void reconcile_debts() {
    // 각 지불자의 총 빚을 계산하는 쿼리
    char* sql = sqlite3_mprintf("SELECT payer_id, SUM(amount) FROM dutch_pay GROUP BY payer_id;");
    sqlite3_stmt* stmt;

    // 쿼리 실행
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        // 결과 반복 처리
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            const char* payer_id = (const char*)sqlite3_column_text(stmt, 0);
            double total_debt = sqlite3_column_double(stmt, 1);

            // 총 빚이 양수인 경우에만 상계 수행
            if (total_debt > 0) {
                // 상대가 해당 지불자에게 줄 빚을 계산하는 쿼리
                char* reverse_sql = sqlite3_mprintf("SELECT SUM(amount) FROM dutch_pay WHERE payer_id = '%s';", payer_id);
                double reverse_debt = 0.0;

                sqlite3_stmt* reverse_stmt;
                if (sqlite3_prepare_v2(db, reverse_sql, -1, &reverse_stmt, NULL) == SQLITE_OK) {
                    if (sqlite3_step(reverse_stmt) == SQLITE_ROW) {
                        reverse_debt = sqlite3_column_double(reverse_stmt, 0);
                    }
                    sqlite3_finalize(reverse_stmt);
                }
                sqlite3_free(reverse_sql);

                // 지불자가 상대에게 더 많이 빚진 경우
                if (total_debt > reverse_debt) {
                    double difference = total_debt - reverse_debt;
                    char* offset_sql = sqlite3_mprintf("UPDATE dutch_pay SET amount = amount - %f WHERE payer_id = '%s';", difference, payer_id);
                    sqlite3_exec(db, offset_sql, NULL, NULL, NULL);
                    sqlite3_free(offset_sql);
                    printf("%s님의 빚에서 %f만큼 상계되었습니다.\n", payer_id, difference);
                }
                // 상대가 지불자에게 더 많이 빚진 경우
                else if (reverse_debt > total_debt) {
                    double difference = reverse_debt - total_debt;
                    char* offset_sql = sqlite3_mprintf("DELETE FROM dutch_pay WHERE payer_id = '%s' AND amount = %f;", payer_id, difference);
                    sqlite3_exec(db, offset_sql, NULL, NULL, NULL);
                    sqlite3_free(offset_sql);
                    printf("%s님의 빚이 완전히 상계되었습니다.\n", payer_id);
                }
                // 빚이 서로 같은 경우
                else {
                    char* delete_sql = sqlite3_mprintf("DELETE FROM dutch_pay WHERE payer_id = '%s';", payer_id);
                    sqlite3_exec(db, delete_sql, NULL, NULL, NULL);
                    sqlite3_free(delete_sql);
                    printf("%s님의 빚이 완전히 상계되었습니다.\n", payer_id);
                }
            }
        }
    }
    // 결과 처리 완료 후 스테이트먼트 해제
    sqlite3_finalize(stmt);
}


int main() {
    int rc = sqlite3_open("test.db", &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "데이터베이스를 열 수 없습니다: %s\n", sqlite3_errmsg(db));
        return 1;
    }

    // 더치페이 테이블 생성
    //sqlite3_exec(db, create_table_sql, NULL, NULL, NULL);

    // 빚 기록
    record_debt("qwer1", "hero", 15000.0);
    record_debt("da", "qwer1", 20000.0);
    record_debt("frank", "qwer1", 10000.0);
    record_debt("qwer1", "da", 10000.0);

    // 빚 정리 실행
    reconcile_debts();

    // 데이터베이스 연결 종료
    sqlite3_close(db);
    return 0;
}
