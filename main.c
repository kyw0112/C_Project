#include <sqlite3.h>
#include <stdio.h>
#include "global_variables.h"


void first_menu(void)
{
	int i=1;

	
	
	// 정수만 입력받는 로직
	while (1) {
		printf("메뉴를 선택하세요 \n");
		printf(" 1. 멤버 회원가입  \n");
		printf(" 2. 기존 멤버 로그인 \n");
		if (scanf("%d", &i) != 1) {
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








}



void after_login_menu(void)
{
	/*	1. 새로운 맛집 입력
	2. 여러 기준들을 이용한 맛집 조회
	3. 여러 기준들을 이용한 맛집 추천
	4. 오늘 뭐먹지?
	5. 맛집 랭킹 기능*/

	int i = 0;
	
	while (1) {
		printf("메뉴를 선택하세요 \n");
		printf(" 1. 새로운 맛집 입력하기  \n");
		printf(" 2. 맛집 조회하기 \n");
		printf(" 3. 맛집 추천 받기  \n");
		printf(" 4. 오늘 뭐먹지? \n");
		printf(" 5. 맛집 랭킹 기능  \n");
		printf(" 6. 로그아웃 \n");
		if (scanf("%d", &i) != 1 || i<=0 || i>6) {
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
		//restInput();
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
		signOut();
		break;
	default:
		printf("잘못된 입력입니다.\n");
		break;
	}




}




int main(void)
{
	/*int first_menu = 0;*/
	// 2. 로그인 OR 회원가입 선택
	first_menu();
		
	// 3. (자동 반복 시작 점) 로그인 성공시 선택 가능한 메뉴 출력 
	
	if (isLoggedIn)
	{
		while(1){
		after_login_menu();
		}
	}
	
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