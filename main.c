#include <sqlite3.h>
#include <stdio.h>
#include "global_variables.h"
#include <string.h>
#include <windows.h> 

#define SHIFT_AMOUNT 2
#define ITERATIONS 10

void printInvertedArt(const char* art, int invert) {
	while (*art) {
		if (*art == ' ') {
			printf("%c", invert ? '@' : ' ');
		}
		else if (*art == '@') {
			printf("%c", invert ? ' ' : '@');
		}
		else {
			printf("%c", *art);
		}
		art++;
	}
}
void asciiart()
{
	char* ascii_art =
		"                 @@@!                 -@@@~              .**********;  !@@*  $@@!               \n"
		" . . . . . . .    @@@!                 -@@@~              .@@@@@@@@@@=  !@@*  $@@!               \n"
		":############,   @@@!                .#@@@#,             .@@@@@@@@@@=  !@@*  $@@!               \n"
		":@@@@@@@@@@@-   @@@!                @@@@@@@.            .@@@@@@@@@@=  !@@*  $@@!               \n"
		":@@@@@@@@@@@-   @@@!              ~@@@@@@@@@;                   :@@=  !@@*  $@@!               \n"
		":@@@.            @@@!        ....!@@@@#! ~$@@@@#....      .......;@@=  !@@*  $@@!               \n"
		":@@@.            @@@!       ,$@@@@@@@*.    !@@@@@@@$,    .@@@@@@@@@@=  !@@=--#@@!               \n"
		":@@@.            @@@!        -#@@@@**       !*@@@@@:     .@@@@@@@@@@=  !@@@@@@@@!               \n"
		":@@@.            @@@!        .*=!!:           :!!=*,     .@@@@@@@@@@=  !@@@@@@@@!               \n"
		":@@@.            @@@!         ,,                 ,,      .@@@=::::::~  !@@$;;#@@!               \n"
		":@@@.            @@@$****  .*************************,   .@@@:         !@@*  $@@!               \n"
		":@@@.            @@@@@@@@  .@@@@@@@@@@@@@@@@@@@@@@@@@-   .@@@:    ..,~,!@@*  $@@!               \n"
		":@@@.            @@@@@@@@  .@@@@@@@@@@@@@@@@@@@@@@@@@-   .@@@#########;!@@*  $@@!               \n"
		":@@@.            @@@=~~~~   ~~~~~~~@@#~~~~~$@@~~~~~~~.   .@@@@@@@@@@@@;!@@*  $@@!               \n"
		":@@@.            @@@!              @@#     =@@           .@@@@@@@@@@@@;!@@*  $@@!               \n"
		":@@@.            @@@!          ....@@#,....$@@....       .#####=*;-.   ;##;  $@@!               \n"
		":@@@~---------,  @@@!          !@@@@@@@@@@@@@@@@@*                   ,,-,.   !==:               \n"
		":@@@@@@@@@@@@$  @@@!          !@@@@@@@@@@@@@@@@@*                ~~:$#@#=:~~.                  \n"
		":@@@@@@@@@@@@@  @@@!          -!!!!!!!!!!!!!!@@@*              :;@@@@@@@@@@@*~                 \n"
		":@@@@@@@@@@@@@  @@@!                         @@@*             :@@@@@@@@@@@@@@#~                \n"
		",~~~~~~~~~~~~~~  @@@!          ~**************@@@*            ~#@@@*~~~~~~~=@@@$,               \n"
		"                 @@@!          !@@@@@@@@@@@@@@@@@*            !@@@-.       .-@@@;               \n"
		"                 @@@!          !@@@@@@@@@@@@@@@@@*            *@@@           $@@!               \n"
		"                 @@@!          !@@@.                          *@@@           @@@!               \n"
		"                 @@@!          !@@@.                          *@@@$         ;@@@:               \n"
		"                 @@@!          !@@@@@@@@@@@@@@@@@$-           ,=@@@@#;:~~!@@@@@*.               \n"
		"                 @@@!          !@@@@@@@@@@@@@@@@@$-            ~=@@@@@@@@@@@@@=~                \n"
		"                 @@@!          ~*****************;,              **$@@@@@@@$*!                  \n";
	
	char* ascii_art2 = 
		":@@@@@@@@@@@     @@@!                   @@,              .@@@@@@@@@@=  !@@*  $@@!               \n"
		":@@@@@@@@@@@     @@@!                 @@@@@@.            .@@@@@@@@@@=  !@@*  $@@!               \n"
		":@@@.            @@@!             @@@@@    @@@@                   @@=  !@@*  $@@!               \n"
		":@@@.            @@@!         #@@@@**       !*@@@@@:     .@@@@@@@@@@=  !@@@@@@@@!               \n"
		":@@@.            @@@!                                    .@@@          !@@    @@!               \n"
		":@@@.            @@@@@@@@  .@@@@@@@@@@@@@@@@@@@@@@@@@-   .@@@:         !@@*   @@!               \n"
		":@@@.            @@@!              @@       @@           .@@@@@@@@@@@@;!@@*   @@!               \n"
		":@@@.            @@@!              @@       @@                                                  \n"
		":@@@@@@@@@@@     @@@!          !@@@@@@@@@@@@@@@@@*                                              \n"
		":@@@@@@@@@@@     @@@!                          @@@               .@@@@@@@@@@@.                  \n"
		"                 @@@!                         @@@*            ~#@@           @@                 \n"
		"                 @@@!          !@@@@@@@@@@@@@@@@@*            !@@ -.          @@;               \n"
		"                 @@@!          !@@@.                          *@@             @@!               \n"
		"                 @@@!          !@@@.                          *@@             @@:               \n"
		"                 @@@!          !@@@@@@@@@@@@@@@@@            ,=@@           @@*.                \n"
		"                 @@@!           @@@@@@@@@@@@@@@@@                **$@@@@@@@$*!                  \n";

	int j = 0;
	for (int i = 0; i < 3; ++i) {
		
		
		

		system("cls"); // 화면 지우기
		printf("\033[1;34m");
		printInvertedArt(ascii_art2, 1);
		Sleep(400); // 아스키 아트가 사라진 상태로 0.5초 대기
		system("cls"); // 화면 지우기
		printInvertedArt(ascii_art2, 0);
		Sleep(400); // 0.5초 대기
	}






}

void first_menu(void)
{
	int i=1;
	printf("\n                                        Dachelin ver 1.0 developed by BYHG all rights reserved.\n");
	printf("\033[1;37m");

	// 정수만 입력받는 로직
	while (1) {
		printf("\n\n메뉴를 선택하세요 \n");
		printf(" 1. 멤버 회원가입  \n");
		printf(" 2. 기존 멤버 로그인 \n");
		if (scanf("%d", &i) != 1 || i<=0 || i>2) {
			printf("정수를 입력하세요.\n");
			while (getchar() != '\n'); // 입력 버퍼 비우기
		}
		else {
			while (getchar() != '\n'); // 입력 버퍼 비우기
			break;
		}
	}

	if (i == 1)
		signUp();
	else if (i == 2)
		signIn();
	


}
// 2 조회
void viewRestaurants()
{
	/*
	1. 전체 맛집 조회
	2. 카테고리별 맛집 조회
	*/
	int i = 0;

	while (1) {
		printf("메뉴를 선택하세요 \n");
		printf(" 1. 전체 맛집 조회  \n");
		printf(" 2. 카테고리별 맛집 조회 \n");

		if (scanf("%d", &i) != 1 || i <= 0 || i > 2) {
			printf("올바른 정수를 입력하세요.\n");
			while (getchar() != '\n'); // 입력 버퍼 비우기
		}
		else {
			while (getchar() != '\n'); // 입력 버퍼 비우기
			break;
		}
	}

	switch (i) {
	case 1:
		restOutput();
		break;


	case 2:
		//미완
		filterByCategory();
		break;


	}
}
// 3 추천
void receiveRecommendation()
{

	/*
		카테고리별 추천 받기
		거리대비 별점 추천
		직급별 맛집 추천
	*/

	int i = 0;

	while (1) {
		printf("메뉴를 선택하세요 \n");
		printf(" 1. 카테고리별 추천 받기  \n");
		printf(" 2. 거리대비 맛집 추천 받기 \n");
		printf(" 3. 직급별 선호 맛집 추천 받기 \n");

		if (scanf("%d", &i) != 1 || i <= 0 || i > 3) {
			printf("올바른 정수를 입력하세요.\n");
			while (getchar() != '\n'); // 입력 버퍼 비우기
		}
		else {
			while (getchar() != '\n'); // 입력 버퍼 비우기
			break;
		}
	}

	switch (i) {

	case 1:
		recomByCategory(loggedInId);
		break;

	case 2:
		
		recomByStarPerDist();
		break;
	
	case 3:
		recomByPosition();
		break;

	}






}




// 6. 맛잘알 팔로우하기


void follow(void)
{
	/*
	1. 멤버 팔로우 하기
	2. 멤버의 팔로워 표시하기
	3. 멤버가 팔로우 하는 사람 표시하기
	4. 팔로우 한 사람들이 추천하는 맛집 보기
	
	*/

	int i = 0;

	while (1) {
		printf("메뉴를 선택하세요 \n");
		printf(" 1. 멤버 팔로우 하기  \n");
		printf(" 2. 멤버의 팔로워 표시하기 \n");
		printf(" 3. 멤버가 팔로우 하는 사람 표시하기  \n");
		printf(" 4. 팔로우 한 사람들이 추천하는 맛집 보기 \n");

		if (scanf("%d", &i) != 1 || i <= 0 || i > 4) {
			printf("올바른 정수를 입력하세요.\n");
			while (getchar() != '\n'); // 입력 버퍼 비우기wsda
		}
		else {
			while (getchar() != '\n'); // 입력 버퍼 비우기
			break;
		}
	}

	switch (i) {
	case 1:
	{
		char* following_id = searchmember();
		//printf("메인 팔로잉 : ____ %p", following_id);
		if (following_id != NULL) {
			follow_member(loggedInId, following_id);
			free(following_id); // 동적 할당된 메모리 해제
		}
		else {
			printf("팔로우할 멤버를 찾을 수 없습니다.\n");
		}
		break;

	}
	case 2:
		display_followers(loggedInId);
		break;
	
	case 3:
		display_following(loggedInId);
		break;
	
	case 4:
		//printf("%s", loggedInId);
		recommend_restaurants(loggedInId);
		break;
	
	default:
		printf("잘못된 입력입니다.\n");
		break;


	}


}



void dept(void)
{




}




void after_login_menu(void)
{
	/*	1. 새로운 맛집 입력
	2. 여러 기준들을 이용한 맛집 조회
	3. 여러 기준들을 이용한 맛집 추천
	4. 오늘 뭐먹지?
	5. 맛집 랭킹 기능
	6. 맛잘알 팔로우하기
	7. 장부 확인하기
	*/

	int i = 0;
	
	while (1) {
		printf("메뉴를 선택하세요 \n");
		printf(" 1. 새로운 맛집 입력하기  \n");
		printf(" 2. 맛집 조회하기 \n");
		printf(" 3. 맛집 추천 받기  \n");
		printf(" 4. 오늘 뭐먹지? \n");
		printf(" 5. 맛집 랭킹 기능  \n");
		printf(" 6. 맛잘알 설정 \n");
		printf(" 7. 더치페이 기록 \n");
		printf(" 8. 로그아웃 \n");


		if (scanf("%d", &i) != 1 || i<=0 || i>8) {
			printf("올바른 정수를 입력하세요.\n");
			while (getchar() != '\n'); // 입력 버퍼 비우기
		}
		else {
			while (getchar() != '\n'); // 입력 버퍼 비우기
			break;
		}
	}


	switch (i) {
	case 1:
		restInput();
		break;
		
	case 2:
		//미완
		viewRestaurants();
		break;
	case 3:
		//함수 미완
		receiveRecommendation();
		break;
	case 4:
		recommendMenu();
		break;
	case 5:
		//viewRanking();
		break;
	case 6:
		follow();
		break;
	case 7:
		//dept();
		break;
	case 8:
		signOut();
		break;
	default:
		printf("잘못된 입력입니다.\n");
		break;
	}




}




int main(void)
{

	asciiart();
	/*int first_menu = 0;*/
	 //2. 로그인 OR 회원가입 선택
	first_menu();
		
	// 3. (자동 반복 시작 점) 로그인 성공시 선택 가능한 메뉴 출력 
	
	if (isLoggedIn)
	{
		while(1){
		after_login_menu();
		}
	}
	/*char tmp[30] = "qwer1";
	recomByCategory(tmp);*/


	/*	1. 새로운 맛집 입력
	2. 여러 기준들을 이용한 맛집 조회
	3. 여러 기준들을 이용한 맛집 추천
	4. 오늘 뭐먹지?
	5. 맛집 랭킹 기능*/


	// 4. 추천받은 레스토랑 중 방문할 레스토랑 선택



	// 5. 일정 시간 슬립(식사 중 문구 출력)



	//  6. 맛있게 식사 하셨습니까? 문구 출력 후 메뉴 선택


	// 7. 











}