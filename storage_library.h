#ifndef STORAGE_LIBRARY_H
#define STORAGE_LIBRARY_H

struct Product {
    int id;               //ID ��������
    char name[50];        //�����
    int quantity;         //ʳ������
    char unit[20];        //̳��
    double price;         //ֳ�� �� �������
    int category;         //����� �������

    int day;              //���� �������
    int month;            //̳���� �������
    int year;             //г���� �������
    int time;             //����� ��������

    int shelfLifeDays;    //����� ����������
    char description[200];//����
};
//-----------------------------------------------------------------> ���������� ��������� � ����� Product <----
void products_load(Product** products, int* product_count) {
    *product_count = 0;

    FILE* fin = fopen("Product.txt", "r");
    if (fin != NULL) {
        while (1) {
            Product temp;
            int result = fscanf(fin,
                "%d|%49[^|]|%d|%19[^|]|%lf|%d|%d|%d|%d|%d|%d|%199[^\n]\n",
                &temp.id, temp.name, &temp.quantity, temp.unit, &temp.price, &temp.category,
                &temp.day, &temp.month, &temp.year, &temp.time,
                &temp.shelfLifeDays, temp.description);
            if (result == EOF) {
                break;
            }
            if (result != 12) {
                printf("\033[31m������� � ��������� ����� �� �����. ������� %d ���� ������ 12.\033[0m\n", result);
                break;
            }
            Product* new_products = (Product*)realloc(*products, ((*product_count) + 1) * sizeof(Product));

            *products = new_products;
            (*products)[*product_count] = temp;
            (*product_count)++;
        }
        fclose(fin);
    }
    else {
        printf("\033[31m���� Product.txt �� �������� ��� �� ������� �������.\033[0m\n");
    }
}
//-----------------------------------------------------------------> ���������� �������� � ����� �ategories <----
void load_categories(char** categories, int* category_count) {
    FILE* file = fopen("�ategories.txt", "r");
    if (!file) {
        perror("\033[31m������� �������� �����!\033[0m\n");
        return;
    }

    char temp[50];
    *category_count = 0;
    *categories = NULL;

    while (fscanf(file, "%49s", temp) == 1) {
        int len = strlen(temp) + 1;
        char* new_category = (char*)malloc(len * sizeof(char));

        strcpy(new_category, temp);

        *categories = (char*)realloc(*categories, (*category_count + 1) * sizeof(char*));

        categories[*category_count] = new_category;
        (*category_count)++;

    }

    fclose(file);
}

//-----------------------------------------------------------------> ���� �񳺿 ��������� <----
void print_products(Product* products, int product_count, char** categories, int* category_count, int day, int month, int year) {
    printf("------------------------------------------------------------------------------------------------------------------------------------------\n");
    printf("| %-5s |%-31s |%-9s |%-10s |%-11s |%-11s |%-11s |%-12s   |%-14s   |\n",
        "ID", "�����", "ʳ������", "̳��", "ֳ��", "��������", "����", "����������", "������� ����");
    printf("------------------------------------------------------------------------------------------------------------------------------------------\n");
    int spoilage;
    for (int i = 0; i < product_count; i++) {
        spoilage = days_spoilage(products[i].time, products[i].shelfLifeDays, products[i].day, products[i].month, products[i].year, day, month, year);
        printf("| %-5d | %-30s | %-8d | %-9s | %-10.2f | %-10s | %02d.%02d.%04d | %-13d |",
            products[i].id, products[i].name, products[i].quantity, products[i].unit, products[i].price, categories[products[i].category - 1], products[i].day,
            products[i].month, products[i].year, products[i].shelfLifeDays);
        if (spoilage == 0) {
            printf("\033[0;31m %-14s \033[0m |\n", "ǳ�������");
        }
        else if (products[i].shelfLifeDays / spoilage >= 2) {
            printf("\033[0;33m %4d \033[0m %-9s |\n", spoilage, "��.");
        }
        else if (products[i].shelfLifeDays / spoilage >= 1) {
            printf("\033[0;32m %4d \033[0m %-9s |\n", spoilage, "��.");
        }
        else {
            printf("\033[0;31m %-14s \033[0m |\n", "ǳ�������");
        }
        size_t len = strlen(products[i].description);
        size_t pos = 0;
        while (pos < len) {
            char line[128 + 1] = { 0 };
            strncpy(line, products[i].description + pos, 128);
            if (!pos) printf("| ����: %-128s |\n", line);
            else printf("| %-134s |\n", line);
            pos += 128;
        }
        printf("------------------------------------------------------------------------------------------------------------------------------------------\n");
    }
}
//-----------------------------------------------------------------> ���� ��������� �� �������
void find_print_products(char* keyword, Product* products, int product_count, char** categories, int* category_count, int day, int month, int year) {
    printf("\n���������� ������ ��� '%s':\n", keyword);
    int found = 0;
    int spoilage;
    for (int i = 0; i < product_count; i++) {
        if (strstr(products[i].name, keyword)) {
            if (!found) {
                printf("------------------------------------------------------------------------------------------------------------------------------------------\n");
                printf("|%-6s |%-31s |%-9s |%-10s |%-11s |%-11s |%-11s |%-12s   |%-14s   |\n",
                    "ID", "�����", "ʳ������", "̳��", "ֳ��", "��������", "����", "����������", "������� ����");
                printf("------------------------------------------------------------------------------------------------------------------------------------------\n");
            }
            found = 1;
            spoilage = days_spoilage(products[i].time, products[i].shelfLifeDays, products[i].day, products[i].month, products[i].year, day, month, year);
            printf("| %-5d | %-30s | %-8d | %-9s | %-10.2f | %-10s | %02d.%02d.%04d | %-13d |",
                products[i].id, products[i].name, products[i].quantity, products[i].unit, products[i].price, categories[products[i].category - 1], products[i].day,
                products[i].month, products[i].year, products[i].shelfLifeDays);
            if (spoilage == 0) {
                printf("\033[0;31m %-14s \033[0m |\n", "ǳ�������");
            }
            else if (products[i].shelfLifeDays / spoilage >= 2) {
                printf("\033[0;33m %4d \033[0m %-9s |\n", spoilage, "��.");
            }
            else if (products[i].shelfLifeDays / spoilage >= 1) {
                printf("\033[0;32m %4d \033[0m %-9s |\n", spoilage, "��.");
            }
            else {
                printf("\033[0;31m %-14s \033[0m |\n", "ǳ�������");
            }
            size_t len = strlen(products[i].description);
            size_t pos = 0;
            while (pos < len) {
                char line[128 + 1] = { 0 };
                strncpy(line, products[i].description + pos, 128);
                if(!pos) printf("| ����: %-128s |\n", line);
                else printf("| %-134s |\n", line);
                pos += 128;
            }
            printf("------------------------------------------------------------------------------------------------------------------------------------------\n");
        }
    }

    if (!found) {
        printf("------------------------------------------------------------------------------------------------------------------------------------------\n");
        printf("|                                                          \033[0;31mͳ���� �� ��������.\033[0m                                                           |\n");
        printf("------------------------------------------------------------------------------------------------------------------------------------------\n");
    }
}
//-----------------------------------------------------------------> ���� ��������� �� ��������
void categories_print_products(int keyword, Product* products, int product_count, char** categories, int* category_count, int day, int month, int year) {
    int found = 0;
    int spoilage;
    for (int i = 0; i < product_count; i++) {
        if (products[i].category == keyword) {
            if (!found) {
                printf("------------------------------------------------------------------------------------------------------------------------------------------\n");
                printf("|%-6s |%-31s |%-9s |%-10s |%-11s |%-11s |%-11s |%-12s   |%-14s   |\n",
                    "ID", "�����", "ʳ������", "̳��", "ֳ��", "��������", "����", "����������", "������� ����");
                printf("------------------------------------------------------------------------------------------------------------------------------------------\n");
            }
            found = 1;
            spoilage = days_spoilage(products[i].time, products[i].shelfLifeDays, products[i].day, products[i].month, products[i].year, day, month, year);
            printf("| %-5d | %-30s | %-8d | %-9s | %-10.2f | %-10s | %02d.%02d.%04d | %-13d |",
                products[i].id, products[i].name, products[i].quantity, products[i].unit, products[i].price, categories[products[i].category - 1], products[i].day,
                products[i].month, products[i].year, products[i].shelfLifeDays);
            if (spoilage == 0) {
                printf("\033[0;31m %-14s \033[0m |\n", "ǳ�������");
            }
            else if (products[i].shelfLifeDays / spoilage >= 2) {
                printf("\033[0;33m %4d \033[0m %-9s |\n", spoilage, "��.");
            }
            else if (products[i].shelfLifeDays / spoilage >= 1) {
                printf("\033[0;32m %4d \033[0m %-9s |\n", spoilage, "��.");
            }
            else {
                printf("\033[0;31m %-14s \033[0m |\n", "ǳ�������");
            }
            size_t len = strlen(products[i].description);
            size_t pos = 0;
            while (pos < len) {
                char line[128 + 1] = { 0 };
                strncpy(line, products[i].description + pos, 128);
                if (!pos) printf("| ����: %-128s |\n", line);
                else printf("| %-134s |\n", line);
                pos += 128;
            }
            printf("------------------------------------------------------------------------------------------------------------------------------------------\n");
        }
    }

    if (!found) {
        printf("------------------------------------------------------------------------------------------------------------------------------------------\n");
        printf("|                                                          \033[0;31mͳ���� �� ��������.\033[0m                                                           |\n");
        printf("------------------------------------------------------------------------------------------------------------------------------------------\n");
    }
}

//-----------------------------------------------------------------> ���������� ������ products[] �� ������ !!(��� ������ � ����)!! <----
void sort_products_name(Product* products, int product_count) {
    for (int i = 0; i < product_count - 1; i++) {
        for (int j = 0; j < product_count - i - 1; j++) {
            if (strcmp(products[j].name, products[j + 1].name) > 0) {
                Product temp = products[j];
                products[j] = products[j + 1];
                products[j + 1] = temp;
            }
        }
    }
}
//-----------------------------------------------------------------> ���������� ������ products[] �� ����� ������� !!(��� ������ � ����)!! <----
void sort_products_date(Product* products, int product_count) {
    for (int i = 0; i < product_count - 1; i++) {
        for (int j = 0; j < product_count - i - 1; j++) {
            if (products[j].year < products[j + 1].year ||
                (products[j].year == products[j + 1].year && products[j].month < products[j + 1].month) ||
                (products[j].year == products[j + 1].year && products[j].month == products[j + 1].month &&
                    products[j].day < products[j + 1].day)) {
                Product temp = products[j];
                products[j] = products[j + 1];
                products[j + 1] = temp;
            }
        }
    }
}
//-----------------------------------------------------------------> ���������� �� ����� ���� ��������� �� �������� !!(��� ������ � ����)!! <----
void sort_products_expiry(Product* products, int product_count, int day, int month, int year) {
    for (int i = 0; i < product_count - 1; i++) {
        for (int j = 0; j < product_count - i - 1; j++) {
            if (days_spoilage(products[j].time, products[j].shelfLifeDays, products[j].day, products[j].month, products[j].year, day, month, year) < days_spoilage(products[j + 1].time, products[j + 1].shelfLifeDays, products[j + 1].day, products[j + 1].month, products[j + 1].year, day, month, year)) {
                Product temp = products[j];
                products[j] = products[j + 1];
                products[j + 1] = temp;
            }
        }
    }
}

//-----------------------------------------------------------------> ��������� ������, ��������� �������� <----
void introduction_products(Product* products, int product_count, int day, int month, int year, char** categories, int* category_count) {
    Product newProduct;

    int found = 0;
    do {
        if (found == 1) {
            clear_previous_line();
            clear_previous_line();
            printf("|                                                 \033[31mĳ� �� ��������!\033[0m                                                  |\n");
        }
        else {
            printf("|                                                                                                                   |\n");
        }
        found = 0;
        printf("\n\r---------------------------------------------------------------------------------------------------------------------");
        printf("\33[1A\r%s\r%s", "                                                                                                                    |", "|   ������ ID ��������:           |");
        scanf_s("%d", &newProduct.id);
        for (int i = 0; i < product_count; i++) {
            if (products[i].id == newProduct.id) {
                found = 1;
            }
        }
    } while (found);

    printf("\n\n\r---------------------------------------------------------------------------------------------------------------------");
    printf("\33[1A\r%s\r%s", "                                                                                                                    |", "|   ������ ����� ��������:        |");
    scanf_s(" %49s", newProduct.name, 50);
    printf("\n");
    newProduct.quantity = 1;
    do {
        if (newProduct.quantity <= 0) {
            clear_previous_line();
            clear_previous_line();
            printf("|                                                 \033[31mĳ� �� ��������!\033[0m                                                  |\n");
        }
        else {
            printf("|                                                                                                                   |\n");
        }
        printf("\n\r---------------------------------------------------------------------------------------------------------------------");
        printf("\33[1A\r%s\r%s", "                                                                                                                    |", "|   ������ ������� ��������:    |");
        scanf_s("%d", &newProduct.quantity);
    } while (newProduct.quantity <= 0);

    printf("\n\n\r---------------------------------------------------------------------------------------------------------------------");
    printf("\33[1A\r%s\r%s", "                                                                                                                    |", "|   ������ ������� �����:        |");
    scanf_s(" %19s", newProduct.unit, 20);

    printf("\n\n\r---------------------------------------------------------------------------------------------------------------------");
    printf("\33[1A\r%s\r%s", "                                                                                                                    |", "|   ������ ���� ��������:         |");
    scanf_s("%lf", &newProduct.price);

    printf("\n\n\r---------------------------------------------------------------------------------------------------------------------");
    printf("\33[1A\r%s\r%s", "                                                                                                                    |", "|   ������ �������� ��������:    |");
    scanf_s("%d", &newProduct.category);

    newProduct.day = year;
    newProduct.month = month;
    newProduct.year = day;

    printf("\n\n\r---------------------------------------------------------------------------------------------------------------------");
    printf("\33[1A\r%s\r%s", "                                              (� ���� �� ������������)                                             |", "|   ������ ��� ���������������:   |");
    scanf_s("%d", &newProduct.time);

    printf("\n\n\r---------------------------------------------------------------------------------------------------------------------");
    printf("\33[1A\r%s\r%s", "                                              (� ����)                                                              |", "|   ������ ����� ����������:    |");
    scanf_s("%d", &newProduct.shelfLifeDays);

    printf("\n\n\r---------------------------------------------------------------------------------------------------------------------");
    printf("\33[1A\r%s\r%s", "                                                                                                                    |", "|   ������ ���� ��������:         |");
    scanf_s(" %199s", newProduct.description, 200);

    FILE * fout = fopen("Product.txt", "a");
    if (fout == NULL) {
        printf("\033[31m������� �������� �����!\033[0m\n");
        return;
    }

    fprintf(fout, "%d|%s|%d|%s|%.2lf|%d|%d|%d|%d|%d|%d|%s\n",
        newProduct.id, newProduct.name, newProduct.quantity, newProduct.unit,
        newProduct.price, newProduct.category, newProduct.day,
        newProduct.month, newProduct.year, newProduct.time,
        newProduct.shelfLifeDays, newProduct.description);

    fclose(fout);

    char directory[300];
    char invoiceFileName[350];
    double sum_price = 0;

    printf("\n������ ��������� ��� ���������� ��������: ");
    scanf_s("%s", directory);
    printf("|\033[32m������� ������ ������ � ����!                                                                                     \033[0m|\n");
    sprintf(invoiceFileName, "%s/Introduction_%d_%02d_%02d.txt", directory, day, month, year);

    FILE* invoiceFile = fopen(invoiceFileName, "w");
    if (invoiceFile == NULL) {
        printf("\033[31m������� ��������� ����� ��������!\033[0m\n");
        return;
    }

    fprintf(invoiceFile, "�������� �� ��������� ������\n");
    fprintf(invoiceFile, "����: %02d.%02d.%d\n", day, month, year);
    fprintf(invoiceFile, "------------------------------------------\n");
    fprintf(invoiceFile, "ID ��������: %d\n", newProduct.id);
    fprintf(invoiceFile, "����� ��������: %s\n", newProduct.name);
    fprintf(invoiceFile, "ʳ������: %d %s\n", newProduct.quantity, newProduct.unit);
    fprintf(invoiceFile, "ֳ�� �� �������: %.2lf ���\n", newProduct.price);
    fprintf(invoiceFile, "���� ��������: %s\n", newProduct.description);
    fprintf(invoiceFile, "�������� �������: %.2lf ���\n", newProduct.price * newProduct.quantity);
    fprintf(invoiceFile, "------------------------------------------\n");
    fprintf(invoiceFile, "������ �� ��������!\n");

    fclose(invoiceFile);
    printf("|\033[32m�������� �������� ������: %-88s\033[0m|\n", invoiceFileName);
    printf("---------------------------------------------------------------------------------------------------------------------\n");
}
//-----------------------------------------------------------------> ³�����������, �������� ������, ��������� �������� <----
void dispatch_product(Product* products, int product_count, int day, int month, int year, int state) {
    Product sale_product;
    char reason[50];
    int found = 0, product_id = 1, quantity_sell = 1, product_tainted = 0;
    do {
        printf("\r---------------------------------------------------------------------------------------------------------------------\n");
        if (product_id <= 0) {
            clear_previous_line();
            clear_previous_line();
            printf("|                                                 \033[31mĳ� �� ��������!\033[0m                                                  |\n");
        }
        else {
            printf("|                                                                                                                   |");
        }
        printf("\n\n\r---------------------------------------------------------------------------------------------------------------------");
        printf("\33[1A\r%s\r%s", "                                                                                                                    |", "|   ������ ID ��������:           |");
        scanf_s("%d", &product_id);
        for (int i = 0; i < product_count; i++) {
            if (products[i].id == product_id) {
                product_tainted = products[i].quantity;
                if (products[i].quantity >= quantity_sell) {

                }
                else {
                    product_id = -1;
                    printf("|                                             \033[31m������ � ����� ID �� ����!\033[0m                                           |\n");
                }
                break;
            }
        }
    } while (product_id <= 0);
    printf("\n");
    if (!state) {
        printf("\n\r---------------------------------------------------------------------------------------------------------------------");
        printf("\33[1A\r%s\r%s", "                                                                                                                    |", "|   ������ ������� �����:         |");
        scanf_s("%49s", reason, 50);
    }
    do {
        quantity_sell = 1;
        if (state) {
            if (quantity_sell <= 0) {
                clear_previous_line();
                clear_previous_line();
                printf("|                                                 \033[31mĳ� �� ��������!\033[0m                                                  |\n");
            }
            else {
                printf("|                                                                                                                   |\n");
            }
            printf("\n\r---------------------------------------------------------------------------------------------------------------------");
            printf("\33[1A\r%s\r%s", "                                                                                                                    |", "|   ������ ������� ��������:    |");
            scanf_s("%d", &quantity_sell);
        }
        else if (product_tainted <= 0) {
            quantity_sell = 1;
        }
        else {
            quantity_sell = product_tainted;
        }
    } while (quantity_sell <= 0);
    printf("\r---------------------------------------------------------------------------------------------------------------------\n");
    for (int i = 0; i < product_count; i++) {
        if (products[i].id == product_id) {
            found = 1;
            if (products[i].quantity >= quantity_sell) {
                sale_product = products[i];
                products[i].quantity -= quantity_sell;
                if (products[i].quantity == 0) {
                    for (int j = i; j < product_count - 1; j++) {
                        products[j] = products[j + 1];
                    }
                    product_count--;
                }
            }
            else {
                quantity_sell = -1;
                printf("|                                           \033[31m����������� ������ ��� �������!\033[0m                                         |\n");
            }
            break;
        }
    }
    if (!found) {
        printf("|                                             \033[31m������ � ����� ID �� ����!\033[0m                                           |\n");
        return;
    }

    FILE* fout = fopen("Product.txt", "w");
    if (fout == NULL) {
        printf("\033[31m������� �������� ����� ��� ������!\033[0m\n");
        return;
    }

    for (int i = 0; i < product_count; i++) {
        fprintf(fout, "%d|%s|%d|%s|%.2lf|%d|%d|%d|%d|%d|%d|%s\n",
            products[i].id, products[i].name, products[i].quantity, products[i].unit,
            products[i].price, products[i].category, products[i].day,
            products[i].month, products[i].year, products[i].time,
            products[i].shelfLifeDays, products[i].description);
    }

    fclose(fout);
    char directory[300];
    char invoiceFileName[350];
    double sum_price = 0;

    printf("������ ��������� ��� ���������� ��������: ");
    scanf_s("%s", directory);

    sprintf(invoiceFileName, "%s/Dispatch_%d_%02d_%02d.txt", directory, day, month, year);

    FILE* invoiceFile = fopen(invoiceFileName, "w");
    if (invoiceFile == NULL) {
        printf("\033[31m������� ��������� ����� ��������!\033[0m\n");
        return;
    }
    if (state) {
        fprintf(invoiceFile, "�������� �� ������ ������\n");
    }
    else {
        fprintf(invoiceFile, "�������� �� �������� ������\n");
    }
    fprintf(invoiceFile, "����: %02d.%02d.%d\n", day, month, year);
    fprintf(invoiceFile, "------------------------------------------\n");
    fprintf(invoiceFile, "ID ��������: %d\n", sale_product.id);
    fprintf(invoiceFile, "����� ��������: %s\n", sale_product.name);
    fprintf(invoiceFile, "ʳ������: %d %s\n", sale_product.quantity, sale_product.unit);
    fprintf(invoiceFile, "ֳ�� �� �������: %.2lf ���\n", sale_product.price);
    fprintf(invoiceFile, "���� ��������: %s\n", sale_product.description);
    fprintf(invoiceFile, "�������� �������: %.2lf ���\n", sale_product.price * sale_product.quantity);
    if (!state) {
        fprintf(invoiceFile, "������� ��������: %s\n", reason);
    }
    fprintf(invoiceFile, "------------------------------------------\n");
    fprintf(invoiceFile, "������ �� ��������!\n");

    fclose(invoiceFile);
    printf("|\033[32m�������� �������� ������: %-88s\033[0m|\n", invoiceFileName);
    printf("---------------------------------------------------------------------------------------------------------------------\n");
}
//-----------------------------------------------------------------> ��������������, ��������� �������� <----
void inventory_products(Product* products, int product_count, char** categories, int* category_count, int day, int month, int year) {
    char directory[300];
    char invoiceFileName[350];
    double sum_price = 0;

    printf("������ ��������� ��� ���������� ��������: ");
    scanf("%s", directory);

    sprintf(invoiceFileName, "%s/Inventory_%d_%02d_%02d.txt", directory, day, month, year);

    FILE* invoiceFile = fopen(invoiceFileName, "w");
    if (invoiceFile == NULL) {
        printf("\033[31m������� ��������� ����� ��������!\033[0m\n");
        return;
    }

    fprintf(invoiceFile, "================================================================================================================================================\n");
    fprintf(invoiceFile, "|                                                  ���� ��������������: %02d.%02d.%d                                                             |\n", day, month, year);
    fprintf(invoiceFile, "================================================================================================================================================\n");
    fprintf(invoiceFile, "------------------------------------------------------------------------------------------------------------------------------------------------\n");
    fprintf(invoiceFile, "| %-5s |%-31s |%-11s |%-11s |%-12s   |%-14s   |%-9s |%-18s |%-9s |\n",
        "ID", "�����", "��������", "����", "����������", "������� ����", "ʳ������", "ֳ�� �� �������", "����");
    fprintf(invoiceFile, "------------------------------------------------------------------------------------------------------------------------------------------------\n");
    int spoilage;
    for (int i = 0; i < product_count; i++) {
        spoilage = days_spoilage(products[i].time, products[i].shelfLifeDays, products[i].day, products[i].month, products[i].year, day, month, year);
        fprintf(invoiceFile, "| %-5d | %-30s | %-10s | %02d.%02d.%04d | %-13d |",
            products[i].id, products[i].name, categories[products[i].category - 1], products[i].day,
            products[i].month, products[i].year, products[i].shelfLifeDays);
        if (spoilage == 0) {
            fprintf(invoiceFile, " %-14s  |", "ǳ�������");
        }
        else if (products[i].shelfLifeDays / spoilage >= 2) {
            fprintf(invoiceFile, " %4d  %-9s |", spoilage, "��.");
        }
        else if (products[i].shelfLifeDays / spoilage >= 1) {
            fprintf(invoiceFile, " %4d  %-9s |", spoilage, "��.");
        }
        else {
            fprintf(invoiceFile, " %-14s  |", "ǳ�������");
        }
        fprintf(invoiceFile, " %-8d | %-6.2f %-10s | %-8.2f |",
            products[i].quantity, products[i].price, products[i].unit, products[i].quantity * products[i].price);
        fprintf(invoiceFile, "\n------------------------------------------------------------------------------------------------------------------------------------------------\n");
        sum_price += products[i].quantity * products[i].price;
    }
    fprintf(invoiceFile, "------------------------------------------------------------------------------------------------------------------------------------------------\n");
    fprintf(invoiceFile, "| %-5s |%122.2f|\n", "�������� �������", sum_price);
    fprintf(invoiceFile, "------------------------------------------------------------------------------------------------------------------------------------------------\n");
    fclose(invoiceFile);
    printf("\n|\033[32m�������� �������� ������: %-88s\033[0m|\n", invoiceFileName);
    printf("---------------------------------------------------------------------------------------------------------------------\n");
}
//-----------------------------------------------------------------> ��������� ���� ������� <----
void �ategories_products(char* category) {
    FILE* fout = fopen("�ategories.txt", "a");
    if (fout == NULL) {
        printf("\033[31m������� �������� �����!\033[0m\n");
        return;
    }

    fprintf(fout, "%s\n", category);

    fclose(fout);
    printf("\033[32m������� ������ ������ � ����!\033[0m\n");
}

//-----------------------------------------------------------------> ������� ���������� ���� <----
int  date_time(int part) {
    time_t now = time(NULL);
    struct tm* local = localtime(&now);

    switch (part) {
    case 0: return local->tm_year + 1900;// г�
    case 1: return local->tm_mon + 1;// ̳����
    case 2: return local->tm_mday;// ����
    case 3: return local->tm_hour;// ������
    default: return -1;
    }
}
#endif