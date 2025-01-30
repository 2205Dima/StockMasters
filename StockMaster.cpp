#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "user_library.h"
#include "format_library.h"
#include "storage_library.h"
#define Size 100

int login_menu(User* users, int user_count, User* regist, int regist_count, char* categories);
int main_menu(User* users, int user_count, User* regist, int regist_count, int role, Product* products, int product_count, char** categories, int* category_count);

int product_all_menu(Product* products, int product_count, char** categories, int* category_count);
int product_arrival_menu(Product* products, int product_count, char** categories, int* category_count);
int product_dispatch_menu(Product* products, int product_count, char** categories, int* category_count);
int product_inventory_menu(Product* products, int product_count, char** categories, int* category_count);

int admin_menu(User* users, int user_count, User* regist, int regist_count);
int admin_users_menu(User* users, int user_count);
int admin_regist_menu(User* users, int user_count, User* regist, int regist_count);

int main() {
    SetConsoleCP(1251); SetConsoleOutputCP(1251);
    int user_count = 0, regist_count = 0, product_count = 0, category_count = 0;
    User* users = (User*)malloc(Size * sizeof(User));
    User* regist = (User*)malloc(Size * sizeof(User));
    Product* products = (Product*)malloc(Size * sizeof(Product));
    char** categories = (char**)malloc(Size * sizeof(char));
    int person;
    do {
        system("cls");
        person = -1;
        products_load(&products, &product_count);
        login_load(&users, &user_count, &regist, &regist_count);
        while (person == -1) {
            person = login_menu(users, user_count, regist, regist_count, *categories);
        };
        if (person == -2) break;
        login_load(&users, &user_count, &regist, &regist_count);
    } while (main_menu(users, user_count, regist, regist_count, person, products, product_count, categories, &category_count) == -1 );
    load_categories(categories, &category_count);
    free(users);
    free(regist);
    free(products);
    free(categories);
    return 1;
}

int login_menu(User* users, int user_count, User* regist, int regist_count, char* categories) {
    int login_button = 1;
    printf("=========================================\n");
    printf("|       �������� �������� ������        |\n");
    printf("=========================================\n");
    printf("| ��� ������ ������, ������ � �������.|\n");
    printf("-----------------------------------------\n");
    printf("|  1. �����        |2. �������������� |\n");
    printf("-----------------------------------------\n");
    printf("|               0. �����                |\n");
    printf("-----------------------------------------\n");
    do {
        if (login_button != 1 && login_button != 2) {
            clear_previous_line();
            clear_previous_line();
            printf("|            \033[31mĳ� �� ��������!\033[0m           |\n");
        }
        else {
            printf("|                                       |\n");
        }
        printf("\n-----------------------------------------");
        printf("\33[1A\r%s\r%s","                                        |", "|             ������ ��:");
        scanf_s("%d", &login_button);
    } while (login_button != 1 && login_button != 2 && login_button != 0);

    switch (login_button) {
    case 2: {  // Register
        system("cls");

        char password_again[50], password[50];
        User new_registr;
        printf("========================================================\n");
        printf("|                       ���������                     |\n");
        printf("========================================================\n");
        new_registr.role = 0;

        do {
            if (new_registr.role != 1 && new_registr.role != 0) {
                clear_previous_line();
                clear_previous_line();
                clear_previous_line();
                printf("|\033[31m                ���� ������� ������!                 \033[0m|\n");
            }
            else {
                printf("|                                                      |\n");
            }
            printf("\n--------------------------------------------------------");
            printf("\33[1A\r%s\r%55s\r%s", "                                                       |", "(1 - �����������, 0 - ����������)", "|������ ����:");
            scanf_s("%d", &new_registr.role);
            printf("\n");
        } while (new_registr.role != 1 && new_registr.role != 0);

        int logins = 1;
        do {
            if (logins == 0) {
                clear_previous_line();
                clear_previous_line();
                clear_previous_line();
                printf("|\033[31m                 ����� ���� ��� ����!               \033[0m|\n");
            }
            else {
                printf("|                                                      |\n");
            }
            logins = 1;
            printf("\n--------------------------------------------------------");
            printf("\33[1A\r%s\r%s", "                                                       |", "|������ ����:");
            scanf_s("%49s", new_registr.login, 50);
            printf("\n");
            for (int i = 0; i < user_count; i++) {
                if (!strcmp(users[i].login, new_registr.login)) {
                    logins = 0;
                }
            }
        } while (logins == 0);
        printf("\n--------------------------------------------------------");
        printf("\33[1A\r%s\r%s", "                                                       |", "|������ ��'�:");
        scanf_s("%49s", new_registr.first_name, 50);

        printf("\n\n--------------------------------------------------------");
        printf("\33[1A\r%s\r%s", "                                                       |", "|������ �� �������:");
        scanf_s("%49s", new_registr.last_name, 50);

        printf("\n\n--------------------------------------------------------");
        printf("\33[1A\r%s\r%s", "                                                       |", "|������ �������:");
        scanf_s("%49s", new_registr.middle_name, 50);

        printf("\n\n--------------------------------------------------------");
        printf("\33[1A\r%s\r%s", "                                                       |", "|������ ��:");
        scanf_s("%u", &new_registr.age);

        printf("\n\n--------------------------------------------------------");
        printf("\33[1A\r%s\r%s", "                                                       |", "|������ ������:");
        scanf_s("%49s", password, 50);

        snprintf(new_registr.password, sizeof(new_registr.password), "%lu", simple_hash(password));
        int passw = 0, delay = 3;
        do {
            system("cls");
            printf("========================================================");
            printf("\n|                      ϳ�����������                   |");
            printf("\n========================================================\n");
            if (passw) {
                printf("|\033[31m        ����� �� ����������. ��������� �� ���.      \033[0m|\n");
                printf("\n--------------------------------------------------------");
                printf("\33[1A\r%s\r%s", "                                                       |", "|ϳ�������� ������:");
                Sleep(1000 * delay);
                delay *= 2;
            }
            else {
                printf("|                                                      |\n");
                printf("\n--------------------------------------------------------");
                printf("\33[1A\r%s\r%s", "                                                       |", "|ϳ�������� ������:");
            }

            scanf_s("%49s", password_again, 50);
            snprintf(password, sizeof(password), "%lu", simple_hash(password_again));
            passw = strcmp(new_registr.password, password);
        } while (passw != 0);

        printf("\n|\033[32m          ����� ���������� ������������!           \033[0m|\n");
        printf("--------------------------------------------------------");

        FILE* fout = fopen("Registration.txt", "a");
        fprintf(fout, "%d|%s|%s|%s|%s|%d|%s\n", new_registr.role, new_registr.login, new_registr.first_name,
            new_registr.last_name, new_registr.middle_name, new_registr.age, new_registr.password);
        fclose(fout);

        Sleep(3000);
        system("cls");
        return -1;
    }
    case 1: {  // Sign_in
        system("cls");
        char login[50], password[50], password_n[50];
        printf("=========================================\n");
        printf("|                 ����                  |\n");
        printf("=========================================\n");
        printf("\n-----------------------------------------");
        printf("\33[1A\r%s\r%s", "                                        |", "|������ ����:");
        scanf_s("%49s", login, 50);

        printf("\n\n-----------------------------------------");
        printf("\33[1A\r%s\r%s", "                                        |", "|������ ������:");
        scanf_s("%49s", password_n, 50);
        snprintf(password, sizeof(password), "%lu", simple_hash(password_n));
        for (int i = 0; i < user_count; i++) {
            if (strcmp(users[i].login, login) == 0) {
                if (strcmp(users[i].password, password) == 0) {
                    printf("\n\n\33[1A\r%s\r%s", "                                        |", "|\033[32m�� ������\033[0m");
                    printf("\n-----------------------------------------");
                    Sleep(1000);
                    system("cls");
                    return users[i].role;
                }
                else {
                    printf("\n\n\33[1A\r%s\r%s", "                                        |", "|\033[31m������� ������\033[0m");
                    printf("\n-----------------------------------------");
                    Sleep(5000);
                    system("cls");
                    return -1;
                }
            }
        }
        printf("\n\n\33[1A\r%s\r%s", "                                        |", "|\033[31m������� ����\033[0m");
        printf("\n-----------------------------------------");
        Sleep(5000);
        system("cls");
        return -1;
    }
    case 0: {
        return -2;
    }
    }
}
int main_menu(User* users, int user_count, User* regist, int regist_count, int role, Product* products, int product_count, char** categories, int* category_count) {
    int menu_button = 1;
    do {
        system("cls");
        if (role == 1) {
            printf("====================================================\n");
            printf("|                    ������� ����                  |\n");
            printf("====================================================\n");
            printf("|  1. ��������         |  2.��������������         |\n");
            printf("----------------------------------------------------\n");
            printf("|  3. �����������      |  4. ³�����������         |\n");
            printf("----------------------------------------------------\n");
            printf("|  5. ������������� ������                       |\n");
            printf("----------------------------------------------------\n");
            printf("|  0. �����������                                  |\n");
            printf("----------------------------------------------------\n");

        }
        else {
            printf("====================================================\n");
            printf("|                    ������� ����                  |\n");
            printf("====================================================\n");
            printf("|  1. ��������         |  2.��������������         |\n");
            printf("----------------------------------------------------\n");
            printf("|  3. �����������      |  4. ³�����������         |\n");
            printf("----------------------------------------------------\n");
            printf("|  0. �����������                                  |\n");
            printf("----------------------------------------------------\n");
        }
        do {
            if (menu_button != 1 && menu_button != 2 && menu_button != 3 && menu_button != 4 && menu_button != 5 && menu_button != 0) {
                clear_previous_line();
                clear_previous_line();
                printf("|                 \033[31mĳ� �� ��������!\033[0m                 |\n");
            }
            else {
                printf("|                                                  |\n");
            }
            printf("\n----------------------------------------------------");
            printf("\33[1A\r%s\r%s", "                                                   |", "|             ������ ��:");
            scanf_s("%d", &menu_button);
            if (role != 1 && menu_button == 5) menu_button = -1;
        } while (menu_button != 1 && menu_button != 2 && menu_button != 3 && menu_button != 4 && menu_button != 5 && menu_button != 0);
        switch (menu_button)
        {
        case 1: {
            products_load(&products, &product_count);
            product_all_menu(products, product_count, categories, category_count);
            break;
        }
        case 2: {
            products_load(&products, &product_count);
            product_inventory_menu(products, product_count, categories, category_count);
            break;
        }
        case 3: {
            products_load(&products, &product_count);
            product_arrival_menu(products, product_count, categories, category_count);
            break;
        }
        case 4: {
            products_load(&products, &product_count);
            product_dispatch_menu(products, product_count, categories, category_count);
            break;
        }
        case 5: {
            admin_menu(users, user_count, regist, regist_count);
            break;
        }
        case 0: 
            return -1;
        }
    } while (menu_button == 1 || menu_button == 2 || menu_button == 3 || menu_button == 4 || menu_button == 5);
    return -1;
}

int product_all_menu(Product* products, int product_count, char** categories, int* category_count) {
    int product_button = 1, keyword_n = 1;
    char keyword[50];
    system("cls");
    printf("==========================================================================================================================================\n");
    printf("|                                                                ��������                                                                |\n");
    printf("==========================================================================================================================================\n");
    int day = date_time(2);
    int month = date_time(1);
    int year = date_time(0);
    do {
        printf("------------------------------------------------------------------------------------------------------------------------------------------\n");
        printf("|  1. ³��������� ��               |  2. ������ ��������             |  3. �����                     |  4. ��������� �� ����� ������   |\n");
        printf("------------------------------------------------------------------------------------------------------------------------------------------\n");
        printf("|  5. ��������� �� �������� ������ |  6. ��������� �� ������          |  7. ������� �����             | 0. �����������                  |\n");
        printf("------------------------------------------------------------------------------------------------------------------------------------------\n");
        do {
            if (product_button != 1 && product_button != 2 && product_button != 3 && product_button != 4 && product_button != 5 && product_button != 6 && product_button != 7 && product_button != 0) {
                clear_previous_line();
                clear_previous_line();
                printf("|                                      |                     \033[31mĳ� �� ��������!\033[0m                     |                                      |\n");
            }
            else {
                printf("|                                      |                                                          |                                      |\n");
            }
            printf("\n------------------------------------------------------------------------------------------------------------------------------------------");
            printf("\33[1A\r%s\r%s", "                                                                                                  |                                      |", "|                                      |                      ������ ��: ");
            scanf_s("%d", &product_button);
        } while (product_button != 1 && product_button != 2 && product_button != 3 && product_button != 4 && product_button != 5 && product_button != 6 && product_button != 7 && product_button != 0);
        switch (product_button)
        {
        case 1: {
            load_categories(categories, category_count);
            products_load(&products, &product_count);
            system("cls");
            printf("==========================================================================================================================================\n");
            printf("|                                                                ��������                                                                |\n");
            printf("==========================================================================================================================================\n");
            load_categories(categories, category_count);
            print_products(products, product_count, categories, category_count, day, month, year);
            break;
        }
        case 2: {
            load_categories(categories, category_count);
            products_load(&products, &product_count);
            system("cls");
            printf("==========================================================================================================================================\n");
            printf("|                                                                ��������                                                                |\n");
            printf("==========================================================================================================================================\n");
            load_categories(categories, category_count);
            printf("------------------------------------------------------------------------------------------------------------------------------------------");
            printf("\n|                                                          ������� �������                                                            |");
            printf("\n------------------------------------------------------------------------------------------------------------------------------------------");
            int i = 0;
            printf("\n|  ");
            for (i = 0; i < (*category_count); i++) {
                if (categories[i] != NULL) printf(" %3d. %-20s|", i + 1, categories[i]);
                if ((i + 1) % 5 == 0) {
                    printf("\n------------------------------------------------------------------------------------------------------------------------------------------");
                    printf("\n|  ");
                }
            }
            printf("\n\r------------------------------------------------------------------------------------------------------------------------------------------");
            printf("\n");
            

            printf("\n------------------------------------------------------------------------------------------------------------------------------------------");
            printf("\33[1A\r%s\r%s", "                                                                                                  |                                      |", "|  0. �����������                      |         ������� ����� �������:");
            scanf_s("%d", &keyword_n);
            if (keyword_n == 0) break;
            system("cls");
            printf("==========================================================================================================================================\n");
            printf("|                                                                ��������                                                                |\n");
            printf("==========================================================================================================================================\n");
            categories_print_products(keyword_n, products, product_count, categories, category_count, day, month, year);
            break;
        }
        case 3: {
            products_load(&products, &product_count);
            system("cls");
            printf("==========================================================================================================================================\n");
            printf("|                                                                ��������                                                                |\n");
            printf("==========================================================================================================================================\n");
            printf("������ �����: ");
            scanf_s("%49s", keyword, 50);
            load_categories(categories, category_count);
            find_print_products(keyword, products, product_count, categories, category_count, day, month, year);
            break;
        }
        case 4: {
            products_load(&products, &product_count);
            system("cls");
            printf("==========================================================================================================================================\n");
            printf("|                                                                ��������                                                                |\n");
            printf("==========================================================================================================================================\n");
            sort_products_date(products, product_count);
            load_categories(categories, category_count);
            print_products(products, product_count, categories, category_count, day, month, year);
            break;
        }
        case 5: {
            products_load(&products, &product_count);
            system("cls");
            printf("==========================================================================================================================================\n");
            printf("|                                                                ��������                                                                |\n");
            printf("==========================================================================================================================================\n");
            sort_products_expiry(products, product_count, day, month, year);
            load_categories(categories, category_count);
            print_products(products, product_count, categories, category_count, day, month, year);
            break;
        }
        case 6: {
            products_load(&products, &product_count);
            system("cls");
            printf("==========================================================================================================================================\n");
            printf("|                                                                ��������                                                                |\n");
            printf("==========================================================================================================================================\n");
            sort_products_name(products, product_count);
            load_categories(categories, category_count);
            print_products(products, product_count, categories, category_count, day, month, year);
            break;
        }
        case 7: {
            products_load(&products, &product_count);
            system("cls");
            printf("==========================================================================================================================================\n");
            printf("|                                                                ��������                                                                |\n");
            printf("==========================================================================================================================================\n");
            int day = date_time(2);
            int month = date_time(1);
            int year = date_time(0);
            dispatch_product(products, product_count, day, month, year, 0);
            break;
        }
        case 0:
            return -1; break;
        }
    } while (product_button == 1 || product_button == 2 || product_button == 3 || product_button == 4 || product_button == 5 || product_button == 6 || product_button == 7 || product_button == 0);
}
int product_arrival_menu(Product* products, int product_count, char** categories, int* category_count) {
    int arrival_button;
    load_categories(categories, category_count);
    products_load(&products, &product_count);
    system("cls");
    printf("=====================================================================================================================\n");
    printf("|                                               ����������� ������                                                  |\n");
    printf("=====================================================================================================================\n");
    do {
        arrival_button = 1;
        printf("---------------------------------------------------------------------------------------------------------------------\n");
        printf("|  1. ������������ �����������       |  2. ���� ��������                   |  0. �����������                      |\n");
        printf("---------------------------------------------------------------------------------------------------------------------\n");
        do {
            if (arrival_button != 1 && arrival_button != 2 && arrival_button != 0) {
                clear_previous_line();
                clear_previous_line();
                printf("                             |                    \033[31mĳ� �� ��������!\033[0m                     |                             \n");
            }
            else {
                printf("                             |                                                         |                             \n");
            }
            printf("\n                             -----------------------------------------------------------                             ");
            printf("\33[1A\r%s\r%s", "                             |                                                         |                             ", "                             |  ������ ��:");
            scanf_s("%d", &arrival_button);
        } while (arrival_button != 1 && arrival_button != 2 && arrival_button != 0);
        switch (arrival_button)
        {
        case 1: {
            system("cls");
            printf("=====================================================================================================================\n");
            printf("|                                               ����������� ������                                                  |\n");
            printf("=====================================================================================================================\n");
            int day = date_time(0);
            int month = date_time(1);
            int year = date_time(2);
            load_categories(categories, category_count);
            printf("---------------------------------------------------------------------------------------------------------------------");
            printf("\n|                                                ������� �������                                                 |");
            printf("\n---------------------------------------------------------------------------------------------------------------------\n");
            int i = 0;
            printf("| ");
            for (i = 0; i < (*category_count); i++) {
                if (categories[i] != NULL) printf(" %-2d. %-17s|", i + 1, categories[i]);
                if ((i + 1) % 5 == 0) {
                    printf("\n---------------------------------------------------------------------------------------------------------------------");
                    printf("\n| ");
                }
            }
            printf("\n\r---------------------------------------------------------------------------------------------------------------------");
            printf("\n");
            introduction_products(products, product_count, day, month, year, categories, category_count);
            break;
        }
        case 2: {
            load_categories(categories, category_count);
            printf("---------------------------------------------------------------------------------------------------------------------");
            printf("\n|                                                ������� �������                                                 |");
            printf("\n---------------------------------------------------------------------------------------------------------------------\n");
            int i = 0;
            printf("| ");
            for (i = 0; i < (*category_count); i++) {
                if (categories[i] != NULL) printf(" %-2d. %-17s|", i + 1, categories[i]);
                if ((i + 1) % 5 == 0) {
                    printf("\n---------------------------------------------------------------------------------------------------------------------");
                    printf("\n| ");
                }
            }
            printf("\n\r---------------------------------------------------------------------------------------------------------------------");
            printf("\n");
            char category[50];
            printf("\n\r---------------------------------------------------------------------------------------------------------------------");
            printf("\33[1A\r%s\r%s", "                             |                                                                                      |", "|  0. �����������            |  ������ ����� �������:");
            scanf_s(" %49s", category, 50);
            if (category[0] == '0' && category[1] == '\0') {
                system("cls");
                printf("=====================================================================================================================\n");
                printf("|                                               ����������� ������                                                  |\n");
                printf("=====================================================================================================================\n");
                break;
            }
            else {
                �ategories_products(category);
            }
            system("cls");
            printf("=====================================================================================================================\n");
            printf("|                                               ����������� ������                                                  |\n");
            printf("=====================================================================================================================\n");
            break;
        }
        case 0:
            return -1;
        }
    } while (arrival_button == 1 || arrival_button == 2 || arrival_button == 0);
}
int product_dispatch_menu(Product* products, int product_count, char** categories, int* category_count) {
    int arrival_button = 1;
    load_categories(categories, category_count);
    products_load(&products, &product_count);
    system("cls");
    printf("=====================================================================================================================\n");
    printf("|                                             ³����������� ������                                                  |\n");
    printf("=====================================================================================================================\n");
    do {
        printf("---------------------------------------------------------------------------------------------------------------------\n");
        printf("|  1. ³�����������                                      |  0. �����������                                          |\n");
        printf("---------------------------------------------------------------------------------------------------------------------\n");
        do {
            if (arrival_button != 1 && arrival_button != 0) {
                clear_previous_line();
                clear_previous_line();
                printf("                             |                    \033[31mĳ� �� ��������!\033[0m                     |                             \n");
            }
            else {
                printf("                             |                                                         |                             \n");
            }
            printf("\n                             -----------------------------------------------------------                             ");
            printf("\33[1A\r%s\r%s", "                             |                                                         |                             ", "                             |  ������ ��:");
            scanf_s("%d", &arrival_button);
        } while (arrival_button != 1 && arrival_button != 0);
        switch (arrival_button)
        {
        case 1: {
            system("cls");
            printf("=====================================================================================================================\n");
            printf("|                                             ³����������� ������                                                  |\n");
            printf("=====================================================================================================================\n");
            int day = date_time(2);
            int month = date_time(1);
            int year = date_time(0);
            dispatch_product(products, product_count, day, month, year, 1);
            break;
        }
        case 0:
            return -1;
        }
    } while (arrival_button == 1 || arrival_button == 0);
}
int product_inventory_menu(Product* products, int product_count, char** categories, int* category_count) {
    int arrival_button = 1;
    load_categories(categories, category_count);
    products_load(&products, &product_count);
    system("cls");
    printf("=====================================================================================================================\n");
    printf("|                                             �������������� ������                                                 |\n");
    printf("=====================================================================================================================\n");
    do {
        printf("---------------------------------------------------------------------------------------------------------------------\n");
        printf("|  1. ��������������                                     |                      0. �����������                      |\n");
        printf("---------------------------------------------------------------------------------------------------------------------\n");
        do {
            if (arrival_button != 1 && arrival_button != 0) {
                clear_previous_line();
                clear_previous_line();
                printf("                             |                    \033[31mĳ� �� ��������!\033[0m                     |                             \n");
            }
            else {
                printf("                             |                                                         |                             \n");
            }
            printf("\n                             -----------------------------------------------------------                             ");
            printf("\33[1A\r%s\r%s", "                             |                                                         |                             ", "                             |  ������ ��:");
            scanf_s("%d", &arrival_button);
        } while (arrival_button != 1 && arrival_button != 0);
        switch (arrival_button)
        {
        case 1: {
            system("cls");
            printf("=====================================================================================================================\n");
            printf("|                                             �������������� ������                                                 |\n");
            printf("=====================================================================================================================\n");
            int day = date_time(2);
            int month = date_time(1);
            int year = date_time(0);
            inventory_products(products, product_count, categories, category_count, day, month, year);
            break;
        }
        case 0:
            return -1;
        }
    } while (arrival_button == 1 || arrival_button == 0);
}

int admin_menu(User* users, int user_count, User* regist, int regist_count) {
    int admin_button = 1;
    do {
        system("cls");
        printf("====================================================\n");
        printf("|                   ���� ������                   |\n");
        printf("====================================================\n");
        login_load(&users, &user_count, &regist, &regist_count);
        printf("|  1. �����������         |  2. ������             |\n");
        printf("----------------------------------------------------\n");
        printf("|                  0. �����������                  |\n");
        printf("----------------------------------------------------\n");
        do {
            if (admin_button != 1 && admin_button != 2 && admin_button != 0) {
                clear_previous_line();
                clear_previous_line();
                printf("|                 \033[31mĳ� �� ��������!\033[0m                 |\n");
            }
            else {
                printf("|                                                  |\n");
            }
            printf("\n----------------------------------------------------");
            printf("\33[1A\r%s\r%s", "                                                   |", "|             ������ ��:");
            scanf_s("%d", &admin_button);
        } while (admin_button != 1 && admin_button != 2 && admin_button != 0);
        switch (admin_button)
        {
        case 1: {
            admin_users_menu(users, user_count);
            break;
        }
        case 2: {
            admin_regist_menu(users, user_count, regist, regist_count);
            break;
        }
        case 0:
            return -1;
        }
    } while (admin_button == 1 || admin_button == 2 || admin_button == 0);
    return -1;
}
int admin_users_menu(User* users, int user_count) {
    char keyword[50], login_delete[50];
    int users_button = 1;
    system("cls");
    printf("===================================================================================\n");
    printf("|                                   �����������                                   |\n");
    printf("===================================================================================\n");

    do {
        printf("===================================================================================\n");
        printf("| 1. ³��������� ���       | 2. ��������� �� ������  | 3. ��������� �� �������� |\n");
        printf("-----------------------------------------------------------------------------------\n");
        printf("| 4. ��������� �� ���������| 5. ��������� �� ����   | 6. ������ �����������     |\n");
        printf("-----------------------------------------------------------------------------------\n");
        printf("| 7. �������� �����������               | 0. �����������                          |\n");
        printf("-----------------------------------------------------------------------------------\n");
        do {

            if (users_button != 1 && users_button != 2 && users_button != 3 && users_button != 4 && users_button != 5 && users_button != 6 && users_button != 7) {
                clear_previous_line();
                clear_previous_line();
                printf("|                                  \033[31mĳ� �� ��������!\033[0m                               |\n");
            }
            else {
                printf("|                                                                                 |\n");
            }
            printf("\n-----------------------------------------------------------------------------------");
            printf("\33[1A\r%s\r%s", "                                                                                  |", "|                       ������ ��:");
            scanf_s("%d", &users_button);
        } while (users_button != 1 && users_button != 2 && users_button != 3 && users_button != 4 && users_button != 5 && users_button != 6 && users_button != 7 && users_button != 0);
        
        switch (users_button)
        {
        case 1: {
            system("cls");
            printf("===================================================================================\n");
            printf("|                                   �����������                                   |\n");
            printf("===================================================================================\n");
            print_users(users, user_count);
            break;
        }
        case 2: {
            system("cls");
            printf("===================================================================================\n");
            printf("|                                   �����������                                   |\n");
            printf("===================================================================================\n");
            sort_users_name(users, user_count, 1);
            print_users(users, user_count);
            break;
        }
        case 3: {
            system("cls");
            printf("===================================================================================\n");
            printf("|                                   �����������                                   |\n");
            printf("===================================================================================\n");
            sort_users_name(users, user_count, 3);
            print_users(users, user_count);
            break;
        }
        case 4: {
            system("cls");
            printf("===================================================================================\n");
            printf("|                                   �����������                                   |\n");
            printf("===================================================================================\n");
            sort_users_name(users, user_count, 2);
            print_users(users, user_count);
            break;
        }
        case 5: {
            system("cls");
            printf("===================================================================================\n");
            printf("|                                   �����������                                   |\n");
            printf("===================================================================================\n");
            sort_users_age(users, user_count);
            print_users(users, user_count);
            break;
        }
        case 6: {
            system("cls");
            printf("===================================================================================\n");
            printf("|                                   �����������                                   |\n");
            printf("===================================================================================\n");
            printf("-----------------------------------------------------------------------------------\n");
            printf("\n\r-----------------------------------------------------------------------------------");
            printf("\33[1A\r%s\r%s", "                                                                                  |", "|  ������ ϲ�: |");
            scanf_s("%49s", keyword, 50);
            find_print_users(keyword, users, user_count);
            break;
        }
        case 7: {
            system("cls");
            printf("===================================================================================\n");
            printf("|                                   �����������                                   |\n");
            printf("===================================================================================\n");
            printf("-----------------------------------------------------------------------------------\n");
            printf("\n\r-------------------------------------------------------------------------------");
            printf("\33[1A\r%s\r%s", "                                                                                   |", "|  ������ ���� �����������: |");
            scanf_s("%49s", login_delete, 50);
            delete_user_login(login_delete, users, user_count, 1);
            break;
        }
        case 0: {
            return -1;
            break;
        }
        }
    } while (users_button == 1 || users_button == 2 || users_button == 3 || users_button == 4 || users_button == 5 || users_button == 6 || users_button == 7 || users_button == 0);
}
int admin_regist_menu(User* users, int user_count, User* regist, int regist_count) {
    char keyword[50], login_delete[50], registered[50];
    int regist_button = 1;
    system("cls");
    printf("===================================================================================\n");
    printf("|                                     ������                                      |\n");
    printf("===================================================================================\n");
    do {
        printf("===================================================================================\n");
        printf("| 1. ³��������� ���       | 2. ��������� �� ������  | 3. ��������� �� �������� |\n");
        printf("-----------------------------------------------------------------------------------\n");
        printf("| 4. ��������� �� ���������| 5. ��������� �� ����   | 6. ������ �����           |\n");
        printf("-----------------------------------------------------------------------------------\n");
        printf("| 7. ϳ��������� �����������| 8. ³������� �����������| 0. �����������            |\n");
        printf("-----------------------------------------------------------------------------------\n");
        do {

            if (regist_button != 1 && regist_button != 2 && regist_button != 3 && regist_button != 4 && regist_button != 5 && regist_button != 6 && regist_button != 7 && regist_button != 8 && regist_button != 0) {
                clear_previous_line();
                clear_previous_line();
                printf("|                                  \033[31mĳ� �� ��������!\033[0m                               |\n");
            }
            else {
                printf("|                                                                                 |\n");
            }
            printf("\n-----------------------------------------------------------------------------------");
            printf("\33[1A\r%s\r%s", "                                                                                  |", "|                       ������ ��:");
            scanf_s("%d", &regist_button);
        } while (regist_button != 1 && regist_button != 2 && regist_button != 3 && regist_button != 4 && regist_button != 5 && regist_button != 6 && regist_button != 7 && regist_button != 8 && regist_button != 0);

        switch (regist_button)
        {
        case 1: {
            system("cls");
            printf("===================================================================================\n");
            printf("|                                     ������                                      |\n");
            printf("===================================================================================\n");
            print_users(regist, regist_count);
            break;
        }
        case 2: {
            system("cls");
            printf("===================================================================================\n");
            printf("|                                     ������                                      |\n");
            printf("===================================================================================\n");
            sort_users_name(regist, regist_count, 1);
            print_users(regist, regist_count);
            break;
        }
        case 3: {
            system("cls");
            printf("===================================================================================\n");
            printf("|                                     ������                                      |\n");
            printf("===================================================================================\n");
            sort_users_name(regist, regist_count, 3);
            print_users(regist, regist_count);
            break;
        }
        case 4: {
            system("cls");
            printf("===================================================================================\n");
            printf("|                                     ������                                      |\n");
            printf("===================================================================================\n");
            sort_users_name(regist, regist_count, 2);
            print_users(regist, regist_count);
            break;
        }
        case 5: {
            system("cls");
            printf("===================================================================================\n");
            printf("|                                     ������                                      |\n");
            printf("===================================================================================\n");
            sort_users_age(regist, regist_count);
            print_users(regist, regist_count);
            break;
        }
        case 6: {
            system("cls");
            printf("===================================================================================\n");
            printf("|                                     ������                                      |\n");
            printf("===================================================================================\n");
            printf("-----------------------------------------------------------------------------------\n");
            printf("\n\r-----------------------------------------------------------------------------------");
            printf("\33[1A\r%s\r%s", "                                                                                  |", "|  ������ ϲ�: |");
            scanf_s("%49s", keyword, 50);
            find_print_users(keyword, regist, regist_count);
            break;
        }
        case 7: {
            system("cls");
            printf("===================================================================================\n");
            printf("|                                     ������                                      |\n");
            printf("===================================================================================\n");
            printf("-----------------------------------------------------------------------------------\n");
            printf("\n\r-----------------------------------------------------------------------------------");
            printf("\33[1A\r%s\r%s", "                                                                                  |", "|  ������ ���� �����������, ����� ������� ����������: |");
            scanf_s("%49s", registered, 50);
            confirmation_regist_login(registered, users, &user_count, regist, &regist_count);
            break;
        }
        case 8: {
            system("cls");
            printf("===================================================================================\n");
            printf("|                                     ������                                      |\n");
            printf("===================================================================================\n");
            printf("-----------------------------------------------------------------------------------\n");
            printf("\n\r-----------------------------------------------------------------------------------");
            printf("\33[1A\r%s\r%s", "                                                                                  |", "|  ������ ���� �����������, ����� ������� ��������: |");
            scanf_s("%49s", login_delete, 50);
            delete_user_login(login_delete, regist, regist_count, 0);
            break;
        }
        case 0: {
            return -1;
            break;
        }
        }
    } while (regist_button == 1 || regist_button == 2 || regist_button == 3 || regist_button == 4 || regist_button == 5 || regist_button == 6 || regist_button == 7 || regist_button == 8 || regist_button == 0);
    return -1;
}
