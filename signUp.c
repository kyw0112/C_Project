#include <sqlite3.h>
#include <stdio.h>
#include <string.h>

void signUp(void)
{
    sqlite3* db;
    char* err_msg = 0;

    int rc = sqlite3_open("test.db", &db);

    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);

        return 1;
    }
    int valid_input = 0;
    char response = ' ';
    while (!valid_input) {
        printf("ȸ�� ���� �Ͻðڽ��ϱ�?  ( Y / N ): ");
        scanf(" %c", &response);

        // �Է°��� Y, N, y, n �� �ϳ����� Ȯ��
        if (response == 'Y' || response == 'y') {
            valid_input = 1; // �ùٸ� �Է��� ��� ������ ��������
        }
        else if (response == 'N' || response == 'n')
        {
            printf("ȸ�� ������ �����մϴ�. ");

        }
        else {
            printf("�߸��� �Է��Դϴ�. Y �Ǵ� N�� �Է��ϼ���.\n");
        }

        // �Է� ���� ����
        while (getchar() != '\n');
    }

    char ID[31] = "";
    char PW[31] = "";
    char name[31] = "";
    int age = 0;
    char position[31] = "";



    // �ϴ� ����ó�� ���ϰ� ��������
    // �ߺ� Ȯ�� ���� �߰� ���
    printf(" ID �Է�  \n");
    gets(ID);
    printf(" PW �Է�  \n");
    gets(PW);
    printf(" name �Է�  \n");
    gets(name);
    printf(" age �Է�  \n");
    scanf("%d%*c", &age);
    printf(" position �Է�  \n");
    gets(position);



    char sql[300];
    sprintf(sql, "INSERT INTO member (ID, PW, NAME, AGE, POSITION) VALUES ('%s', '%s', '%s', %d, '%s');", ID, PW, name, &age, position);

    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);

    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "SQL error: %s\n", err_msg);

        sqlite3_free(err_msg);
        sqlite3_close(db);
        return 1;
    }
    sqlite3_close(db);
    return 0;
}