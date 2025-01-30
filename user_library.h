#ifndef USER_LIBRARY_H
#define USER_LIBRARY_H
struct User {
    int role;
    char login[50];
    char first_name[50];
    char last_name[50];
    char middle_name[50];
    unsigned int age;
    char password[30];
};
//-----------------------------------------------------------------> Зчитування користувачів з файлу Registration, User <----
void login_load(User** users, int* user_count, User** regist, int* regist_count) {
    *user_count = 0;
    *regist_count = 0;

    FILE* fin = fopen("Registration.txt", "r");
    if (fin != NULL) {
        while (!feof(fin)) {
            User temp;
            if (fscanf(fin, "%d|%49[^|]|%49[^|]|%49[^|]|%49[^|]|%u|%29[^\n]\n",
                &temp.role, temp.login, temp.first_name, temp.last_name, temp.middle_name, &temp.age, temp.password) == 7) {
                *regist = (User*)realloc(*regist, ((*regist_count) + 1) * sizeof(User));
                (*regist)[*regist_count] = temp;
                (*regist_count)++;
            }
        }
        fclose(fin);
    }
    else {
        printf("\033[31mФайл Registration.txt не знайдено або не вдалося відкрити.\033[0m\n");
    }

    fin = fopen("User.txt", "r");
    if (fin != NULL) {
        while (!feof(fin)) {
            User temp;
            if (fscanf(fin, "%d|%49[^|]|%49[^|]|%49[^|]|%49[^|]|%u|%29[^\n]\n",
                &temp.role, temp.login, temp.first_name, temp.last_name, temp.middle_name, &temp.age, temp.password) == 7) {
                *users = (User*)realloc(*users, ((*user_count) + 1) * sizeof(User));
                (*users)[*user_count] = temp;
                (*user_count)++;
            }
        }
        fclose(fin);
    }
    else {
        printf("\033[31mФайл User.txt не знайдено або не вдалося відкрити.\033[0m\n");
    }
}

//-----------------------------------------------------------------> Вивід усіх користувачів <----
void print_users(User* users, int user_count) {
    printf("-----------------------------------------------------------------------------------\n");
    printf("|%-5s |%-15s |%-15s |%-15s |%-15s |%-5s |\n",
        "Роль", "Логін", "Ім'я", "По батькові", "Прізвище", "Вік");
    printf("-----------------------------------------------------------------------------------\n");
    printf("-----------------------------------------------------------------------------------\n");
    for (int i = 0; i < user_count; i++) {
        printf("|%-5d |%-15s |%-15s |%-15s |%-15s |%-5u |\n",
            users[i].role,
            users[i].login,
            users[i].first_name,
            users[i].last_name,
            users[i].middle_name,
            users[i].age);
        printf("-----------------------------------------------------------------------------------\n");
    }
}
//-----------------------------------------------------------------> Вивід користувачів за пошуком <----
void find_print_users(char* keyword, User* users, int user_count) {
    printf("-----------------------------------------------------------------------------------\n");
    printf("\n|                                Результати пошуку                                  |\n");
    printf("-----------------------------------------------------------------------------------\n");
    printf("-----------------------------------------------------------------------------------\n");
    printf("|%-5s |%-15s |%-15s |%-15s |%-15s |%-5s |\n",
        "Роль", "Логін", "Ім'я", "По батькові", "Прізвище", "Вік");
    printf("-----------------------------------------------------------------------------------\n");

    int found = 0;
    for (int i = 0; i < user_count; i++) {
        if (strstr(users[i].login, keyword) ||
            strstr(users[i].first_name, keyword) ||
            strstr(users[i].last_name, keyword) ||
            strstr(users[i].middle_name, keyword)) {
            found = 1;
            printf("|%-5d |%-15s |%-15s |%-15s |%-15s |%-5u |\n",
                users[i].role,
                users[i].login,
                users[i].first_name,
                users[i].last_name,
                users[i].middle_name,
                users[i].age);
            printf("-----------------------------------------------------------------------------------\n");
        }
    }

    if (!found) {
        printf("-----------------------------------------------------------------------------------");
        printf("\n|\033[0;31m                         Жодного користувача не знайдено.                        \033[0m|\n");
        printf("-----------------------------------------------------------------------------------\n");
    }
}

//-----------------------------------------------------------------> Сортування !!(БЕЗ ЗАПИСУ У ФАЙЛ)!! <----
void sort_users_name(User* users, int user_count, int n) {
    char name1[50], name2[50];
    for (int i = 0; i < user_count - 1; i++) {
        for (int j = 0; j < user_count - i - 1; j++) {
            if (n == 1) { strcpy(name1, users[j].first_name); strcpy(name2, users[j + 1].first_name); }
            else if (n == 2) { strcpy(name1, users[j].middle_name); strcpy(name2, users[j + 1].middle_name); }
            else if (n == 3) { strcpy(name1, users[j].last_name); strcpy(name2, users[j + 1].last_name); }
            else { strcpy(name1, users[j].login); strcpy(name2, users[j + 1].login); }
            if (strcmp(name1, name2) > 0) {
                User temp = users[j];
                users[j] = users[j + 1];
                users[j + 1] = temp;
            }
        }
    }
}
//-----------------------------------------------------------------> Сортування за віком !!(БЕЗ ЗАПИСУ У ФАЙЛ)!! <----
void sort_users_age(User* users, int user_count) {
    for (int i = 0; i < user_count - 1; i++) {
        for (int j = 0; j < user_count - i - 1; j++) {
            if (users[j].age > users[j + 1].age) {
                User temp = users[j];
                users[j] = users[j + 1];
                users[j + 1] = temp;
            }
        }
    }
}

//-----------------------------------------------------------------> Видалення запису користувача <----
void delete_user_login(char* login_to_delete, User* users, int user_count, int indicator) {
    int found = 0;
    FILE* fin;
    if (indicator) {
        fin = fopen("User.txt", "r");
    }
    else {
        fin = fopen("Registration.txt", "r");
    }
    if (fin == NULL) {
        printf("\033[31mФайл User.txt не знайдено або не вдалося відкрити.\033[0m\n");
        return;
    }
    User* temp_users = (User*)malloc(user_count * sizeof(User));
    int temp_count = 0;
    while (!feof(fin)) {
        User temp;
        if (fscanf(fin, "%d|%49[^|]|%49[^|]|%49[^|]|%49[^|]|%u|%29[^\n]\n",
            &temp.role, temp.login, temp.first_name, temp.last_name, temp.middle_name, &temp.age, temp.password) == 7) {
            if (strcmp(temp.login, login_to_delete) != 0) {
                temp_users[temp_count++] = temp;
            }
            else {
                found = 1;
            }
        }
    }
    fclose(fin);
    if (!found) {
        printf("--------------------------------------------------------------------------------------\n");
        printf("\n|                     \033[31mКористувач не знайдений.\033[0m                                     |\n");
        printf("--------------------------------------------------------------------------------------\n");
        free(temp_users);
        return;
    }
    FILE* fout;
    if (indicator) {
        fout = fopen("User.txt", "w");
    }
    else {
        fout = fopen("Registration.txt", "w");
    }
    if (fout == NULL) {
        printf("\033[31mНе вдалося відкрити файл User.txt для запису.\033[0m\n");
        free(temp_users);
        return;
    }
    for (int i = 0; i < temp_count; i++) {
        fprintf(fout, "%d|%s|%s|%s|%s|%u|%s\n", temp_users[i].role, temp_users[i].login,
            temp_users[i].first_name, temp_users[i].last_name,
            temp_users[i].middle_name, temp_users[i].age, temp_users[i].password);
    }
    fclose(fout);
    users = temp_users;
    user_count = temp_count;
    printf("--------------------------------------------------------------------------------------\n");
    printf("\n|                     \033[31mКористувач видалено     \033[0m                                     |\n");
    printf("--------------------------------------------------------------------------------------\n");
}
//-----------------------------------------------------------------> Підтвердження запиту користувача <----
void confirmation_regist_login(char* registered, User* users, int* user_count, User* regist, int* regist_count) {
    int found = 0;
    FILE* fout = fopen("User.txt", "a");
    if (fout == NULL) {
        printf("Помилка відкриття файлу User.txt.\n");
        return;
    }
    for (int i = 0; i < *regist_count; i++) {
        if (strcmp(regist[i].login, registered) == 0) {
            fprintf(fout, "%d|%s|%s|%s|%s|%u|%s\n", regist[i].role, regist[i].login, regist[i].first_name,
                regist[i].last_name, regist[i].middle_name, regist[i].age, regist[i].password);
            found = 1;

            for (int j = i; j < *regist_count - 1; j++) {
                regist[j] = regist[j + 1];
            }
            (*regist_count)--;
            break;
        }
    }
    fclose(fout);

    if (!found) {
        printf("-----------------------------------------------------------------------------------");
        printf("\n|                     \033[31mКористувач не знайдений.\033[0m                                     |\n");
        printf("-----------------------------------------------------------------------------------\n");
        return;
    }
    fout = fopen("Registration.txt", "w");
    if (fout == NULL) {
        printf("Помилка відкриття файлу Registration.txt.\n");
        return;
    }

    for (int i = 0; i < *regist_count; i++) {
        fprintf(fout, "%d|%s|%s|%s|%s|%u|%s\n", regist[i].role, regist[i].login, regist[i].first_name,
            regist[i].last_name, regist[i].middle_name, regist[i].age, regist[i].password);
    }
    fclose(fout);
    printf("-----------------------------------------------------------------------------------\n");
    printf("|                        \033[32mКористувач підтверджено \033[0m                                 |\n");
    printf("-----------------------------------------------------------------------------------\n");
}

//-----------------------------------------------------------------> Хешування пароля <----
unsigned long simple_hash(const char* password) {
    unsigned long hash = 5381;
    int c;

    while ((c = *password++)) {
        hash = ((hash << 5) + hash) + c;
    }

    return hash;
}

#endif