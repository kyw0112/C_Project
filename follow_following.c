#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>

sqlite3* db; // SQLite 데이터베이스 핸들러

// 데이터베이스 열기 함수
void db_open(const char* db_name) {
    // 데이터베이스 열기 시도
    int rc = sqlite3_open(db_name, &db);
    if (rc != SQLITE_OK) {
        // 실패 시 에러 메시지 출력 후 프로그램 종료
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        exit(1);
    }
    else {
        // 성공 시 메시지 출력
        printf("Database opened successfully. 팔로우 디비오픈\n");
    }
}

// 멤버를 팔로우하는 함수
void follow_member(const char* follower_id, const char* following_id) {
    // 데이터베이스 열기

    //printf("팔로우 멤버 두개 : %s %s ", follower_id, following_id);

    sqlite3_close(db);


    db_open("test.db");
    // SQL 쿼리 생성
    char* sql = sqlite3_mprintf("INSERT INTO follow_following (follower_id, following_id) VALUES ('%s', '%s');", follower_id, following_id);
    
    // SQL 실행
    sqlite3_exec(db, sql, NULL, NULL, NULL);
    // 동적 할당된 메모리 해제
    sqlite3_free(sql);
    // 성공 메시지 출력
    printf("멤버 팔로우가 완료되었습니다. %s\n", following_id);
}

// 멤버의 팔로워를 표시하는 함수
void display_followers(const char* member_id) {
    // 데이터베이스 열기
    db_open("test.db");
    // SQL 쿼리 생성
    char* sql = sqlite3_mprintf("SELECT follower_id FROM follow_following WHERE following_id = '%q';", member_id);
    // SQL 문 실행을 위한 statement 생성
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        printf("내 팔로워 %s:\n", member_id);
        // 결과셋을 순회하며 출력
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            const char* follower_id = (const char*)sqlite3_column_text(stmt, 0);
            printf("%s\n", follower_id);
        }
    }
    // statement 종료
    sqlite3_finalize(stmt);
    // 동적 할당된 메모리 해제
    sqlite3_free(sql);
}

// 멤버가 팔로우하는 사람을 표시하는 함수
void display_following(const char* member_id) {
    // 데이터베이스 열기
    db_open("test.db");
    // SQL 쿼리 생성
    char* sql = sqlite3_mprintf("SELECT following_id FROM follow_following WHERE follower_id = '%q';", member_id);
    // SQL 문 실행을 위한 statement 생성
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        printf("맛잘알 다우인 %s:\n", member_id);
        // 결과셋을 순회하며 출력
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            const char* following_id = (const char*)sqlite3_column_text(stmt, 0);
            printf("%s\n", following_id);
        }
    }
    // statement 종료
    sqlite3_finalize(stmt);
    // 동적 할당된 메모리 해제
    sqlite3_free(sql);
}

// 멤버에게 추천하는 레스토랑을 표시하는 함수
void recommend_restaurants(const char* member_id) {
    // 데이터베이스 열기
    db_open("test.db");
    // SQL 쿼리 생성. restaurant 테이블과 JOIN하여 레스토랑의 이름을 가져온다.
    char* sql = sqlite3_mprintf(
        "SELECT r.restaurant_no, r.name, AVG(rr.rating) AS avg_rating, rr.content "
        "FROM restaurant_review rr "
        "JOIN follow_following f ON rr.member_id = f.following_id "
        "JOIN restaurant r ON rr.restaurant_no = r.restaurant_no "
        "WHERE f.follower_id = '%s' "
        "GROUP BY rr.restaurant_no "
        "ORDER BY avg_rating DESC "
        "LIMIT 10;",
        member_id);
    // SQL 문 실행을 위한 statement 생성
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        printf("다슐랭 맛잘알들의 추천 맛집 %s:\n", member_id);
        // 결과셋을 순회하며 출력
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int restaurant_no = sqlite3_column_int(stmt, 0);
            const char* name = (const char*)sqlite3_column_text(stmt, 1); // 레스토랑 이름
            double avg_rating = sqlite3_column_double(stmt, 2);
            const char* content = (const char*)sqlite3_column_text(stmt, 3);
            printf("가게 이름 : %s, 평점 : %.2f, 한줄평: %s\n",  name, avg_rating, content);
        }
    }
    else {
        printf("SQL Error: %s\n", sqlite3_errmsg(db));
    }
    // 사용한 자원 해제
    sqlite3_finalize(stmt);
    sqlite3_free(sql);
    // 데이터베이스 닫기
    sqlite3_close(db);
}

//int main() {
//    // 멤버를 팔로우하고, 팔로워 및 팔로잉 목록을 표시하며, 추천 레스토랑을 표시하는 메인 함수
//    follow_member("qwer1", "hero");
//    follow_member("qwer1", "spiderman");
//    follow_member("qwer1", "da");
//    follow_member("da", "qwer1");
//    follow_member("hero", "da");
//    display_followers("qwer1");
//    display_following("qwer1");
//    recommend_restaurants("qwer1");
//
//    // 데이터베이스 닫기
//    sqlite3_close(db);
//    return 0;
//}
