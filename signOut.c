void signOut() {
    if (isLoggedIn) {
        printf("로그아웃 되었습니다.\n");
        isLoggedIn = 0; // 로그인 상태를 로그아웃으로 변경
        strcpy(loggedInId, ""); // 현재 로그인한 사용자의 아이디 초기화
    }
    else {
        printf("로그인 상태가 아닙니다.\n");
    }
}