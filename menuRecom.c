#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 함수 선언
void showBranch(const char* title, const char** options, int numOptions, int level);
void recommendMenu();

// 메인 함수
int main() {
    recommendMenu();
    return 0;
}

// 브랜치와 선택지를 출력하는 함수
void showBranch(const char* title, const char** options, int numOptions, int level) {
    printf("%*s%s\n", level * 4, "", title); // 들여쓰기 후 브랜치 제목 출력
    for (int i = 0; i < numOptions; ++i) {
        printf("%*s%d. %s\n", (level * 4) + 2, "", i + 1, options[i]); // 선택지 출력
    }
}

// 메뉴를 추천하고 선택하는 함수
void recommendMenu() {
    const char* menuTree[] = { "메뉴 추천", "├─ 안고기", "└─ 고기" };
    const char* meatBranch[] = { "안고기", "├─ 얼큰", "└─ 안얼큰" };
    const char* spicyBranch[] = { "얼큰", "├─ 매움", "└─ 안매움" };
    const char* notSpicyBranch[] = { "안얼큰", "├─ 매움", "└─ 안매움" };
    const char* cookedBranch[] = { "고기", "├─ 익은", "└─ 안익은" };
    const char* spicyCookedBranch[] = { "익은", "├─ 매움", "└─ 안매움" };
    const char* notSpicyCookedBranch[] = { "안익은", "├─ 풍성", "└─ 안풍성" };

    int choice;
    char answer[100];

    do {
        printf("\n          ╔═══════════════════════════════ 메뉴 추천\n");
        showBranch(menuTree[0], &menuTree[1], 2, 0);
        printf("          ╚─ 선택하세요 (숫자 입력): ");
        if (fgets(answer, sizeof(answer), stdin)) {
            if (sscanf(answer, "%d", &choice) != 1) {
                printf("잘못된 입력입니다. 숫자를 입력해주세요.\n");
                continue;
            }
        }
        else {
            printf("입력이 없습니다. 다시 시도해주세요.\n");
            continue;
        }
        switch (choice) {
        case 1:
            while (1) {
                printf("\n              ╔══════════════ 안고기\n");
                showBranch(meatBranch[0], &meatBranch[1], 2, 1);
                printf("              ╚─ 선택하세요 (숫자 입력): ");
                if (fgets(answer, sizeof(answer), stdin)) {
                    if (sscanf(answer, "%d", &choice) != 1) {
                        printf("잘못된 입력입니다. 숫자를 입력해주세요.\n");
                        continue;
                    }
                }
                else {
                    printf("입력이 없습니다. 다시 시도해주세요.\n");
                    continue;
                }
                if (choice == 1) {
                    while (1) {
                        printf("\n                  ╔══ 얼큰\n");
                        showBranch(spicyBranch[0], &spicyBranch[1], 2, 2);
                        printf("                  ╚─ 선택하세요 (숫자 입력): ");
                        if (fgets(answer, sizeof(answer), stdin)) {
                            if (sscanf(answer, "%d", &choice) != 1) {
                                printf("잘못된 입력입니다. 숫자를 입력해주세요.\n");
                                continue;
                            }
                        }
                        else {
                            printf("입력이 없습니다. 다시 시도해주세요.\n");
                            continue;
                        }
                        if (choice == 1) {
                            printf("\n                      └─ 라면\n");
                            printf("                      └─ 짬뽕\n");
                            printf("                      └─ 부대찌개\n");
                            printf("                      └─ 매운탕\n");
                            break;
                        }
                        else if (choice == 2) {
                            printf("\n                      └─ 우동\n");
                            printf("                      └─ 해장국\n");
                            printf("                      └─ 순대국\n");
                            printf("                      └─ 설렁탕\n");
                            printf("                      └─ 라멘\n");
                            printf("                      └─ 만두국\n");
                            printf("                      └─ 쌀국수\n");
                            break;
                        }
                        else {
                            printf("잘못된 입력입니다.\n");
                        }
                    }
                    break;
                }
                else if (choice == 2) {
                    while (1) {
                        printf("\n                  ╔══ 안얼큰\n");
                        showBranch(notSpicyBranch[0], &notSpicyBranch[1], 2, 2);
                        printf("                  ╚─ 선택하세요 (숫자 입력): ");
                        if (fgets(answer, sizeof(answer), stdin)) {
                            if (sscanf(answer, "%d", &choice) != 1) {
                                printf("잘못된 입력입니다. 숫자를 입력해주세요.\n");
                                continue;
                            }
                        }
                        else {
                            printf("입력이 없습니다. 다시 시도해주세요.\n");
                            continue;
                        }
                        if (choice == 1) {
                            printf("\n                      └─ 불닭볶음면\n");
                            printf("                      └─ 떡볶이\n");
                            printf("                      └─ 비빔면\n");
                            printf("                      └─ 순대볶음\n");
                            printf("                      └─ 커리\n");
                            printf("                      └─ 양장피\n");
                            break;
                        }
                        else if (choice == 2) {
                            printf("\n                      └─ 김밥\n");
                            printf("                      └─ 짜장면\n");
                            printf("                      └─ 햄버거\n");
                            printf("                      └─ 주먹밥\n");
                            printf("                      └─ 만두\n");
                            printf("                      └─ 냉면\n");
                            printf("                      └─ 전\n");
                            printf("                      └─ 팟타이\n");
                            printf("                      └─ 피자\n");
                            printf("                      └─ 파스타\n");
                            break;
                        }
                        else {
                            printf("잘못된 입력입니다.\n");
                        }
                    }
                    break;
                }
                else {
                    printf("잘못된 입력입니다.\n");
                }
            }
            break;
        case 2:
            while (1) {
                printf("\n              ╔══════════════ 고기\n");
                showBranch(cookedBranch[0], &cookedBranch[1], 2, 1);
                printf("              ╚─ 선택하세요 (숫자 입력): ");
                if (fgets(answer, sizeof(answer), stdin)) {
                    if (sscanf(answer, "%d", &choice) != 1) {
                        printf("잘못된 입력입니다. 숫자를 입력해주세요.\n");
                        continue;
                    }
                }
                else {
                    printf("입력이 없습니다. 다시 시도해주세요.\n");
                    continue;
                }
                if (choice == 1) {
                    while (1) {
                        printf("\n                  ╔══════════════ 익은\n");
                        showBranch(spicyCookedBranch[0], &spicyCookedBranch[1], 2, 2);
                        printf("                  ╚─ 선택하세요 (숫자 입력): ");
                        if (fgets(answer, sizeof(answer), stdin)) {
                            if (sscanf(answer, "%d", &choice) != 1) {
                                printf("잘못된 입력입니다. 숫자를 입력해주세요.\n");
                                continue;
                            }
                        }
                        else {
                            printf("입력이 없습니다. 다시 시도해주세요.\n");
                            continue;
                        }
                        if (choice == 1) {
                            printf("\n                      └─ 닭발\n");
                            printf("                      └─ 매운치킨\n");
                            printf("                      └─ 찜닭\n");
                            printf("                      └─ 쭈꾸미\n");
                            printf("                      └─ 등갈비\n");
                            printf("                      └─ 해물탕\n");
                            printf("                      └─ 불족발\n");
                            printf("                      └─ 아구찜\n");
                            printf("                      └─ 조개탕\n");
                            break;
                        }
                        else if (choice == 2) {
                            printf("\n                      └─ 대패삼겹살\n");
                            printf("                      └─ 치킨\n");
                            printf("                      └─ 막창\n");
                            printf("                      └─ 양꼬치\n");
                            printf("                      └─ 탕수육\n");
                            printf("                      └─ 족발\n");
                            printf("                      └─ 양념갈비\n");
                            printf("                      └─ 오리고기\n");
                            printf("                      └─ 대하\n");
                            printf("                      └─ 보쌈\n");
                            printf("                      └─ 백숙\n");
                            printf("                      └─ 스테이크\n");
                            printf("                      └─ 대게\n");
                            printf("                      └─ 랍스타\n");
                            break;
                        }
                        else {
                            printf("잘못된 입력입니다.\n");
                        }
                    }
                    break;
                }
                else if (choice == 2) {
                    while (1) {
                        printf("\n                  ╔══════════════ 안익은\n");
                        showBranch(notSpicyCookedBranch[0], &notSpicyCookedBranch[1], 2, 2);
                        printf("                  ╚─ 선택하세요 (숫자 입력): ");
                        if (fgets(answer, sizeof(answer), stdin)) {
                            if (sscanf(answer, "%d", &choice) != 1) {
                                printf("잘못된 입력입니다. 숫자를 입력해주세요.\n");
                                continue;
                            }
                        }
                        else {
                            printf("입력이 없습니다. 다시 시도해주세요.\n");
                            continue;
                        }
                        if (choice == 1) {
                            printf("\n                      └─ 초밥\n");
                            printf("                      └─ 간장게장\n");
                            printf("                      └─ 삼합\n");
                            printf("                      └─ 회\n");
                            printf("                      └─ 참치회\n");
                            break;
                        }
                        else if (choice == 2) {
                            printf("\n                      └─ 멍게해삼\n");
                            printf("                      └─ 오징어회\n");
                            printf("                      └─ 간장새우\n");
                            printf("                      └─ 문어숙회\n");
                            printf("                      └─ 꽃새우\n");
                            printf("                      └─ 연어\n");
                            printf("                      └─ 육회\n");
                            break;
                        }
                        else {
                            printf("잘못된 입력입니다.\n");
                        }
                    }
                    break;
                }
                else {
                    printf("잘못된 입력입니다.\n");
                }
            }
            break;
        default:
            printf("잘못된 입력입니다.\n");
            break;
        }
        printf("\n");
        do {
            printf("\n다시 추천 받으시겠습니까? (Y/N): ");
            fgets(answer, sizeof(answer), stdin);
            if (strcmp(answer, "Y\n") == 0 || strcmp(answer, "y\n") == 0) {
                break;
            }
            else if (strcmp(answer, "N\n") == 0 || strcmp(answer, "n\n") == 0) {
                return;
            }
            else {
                printf("잘못된 입력입니다. 'Y' 또는 'N'을 입력해주세요.\n");
            }
        } while (1);

    } while (1);
}
