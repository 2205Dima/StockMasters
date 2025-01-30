#ifndef FORMAT_LIBRARY_H
#define FORMAT_LIBRARY_H

void clear_line() {
    printf("\33[2K\r");
    fflush(stdout);
}
void clear_previous_line() {
    printf("\33[1A");
    printf("\33[2K");
    printf("\r");
    fflush(stdout);
}
int print_category(char* categories, int number) {
    char* token = strtok(categories, ", ");
    int index = 1;

    while (token != NULL) {
        if (index == number) {
            printf("Категорія номер %d: %s\n", number, token);
            return 1;
        }
        token = strtok(NULL, ", ");
        index++;
    }
    return 0;
}
int days_month(int month, int year) {
    if (month == 2) {
        return (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0)) ? 29 : 28;
    }
    if (month == 4 || month == 6 || month == 9 || month == 11) {
        return 30;
    }
    return 31;
}
int calculate_days_between_dates(int day, int month, int year, int new_day, int new_month, int new_year) {
    int days = 0;

    if (year == new_year) {
        if (month == new_month) {
            return new_day - day;
        }
        else {
            days += days_month(month, year) - day;
            for (int m = month + 1; m < new_month; m++) {
                days += days_month(m, year);
            }
            days += new_day;
            return days;
        }
    }

    days += days_month(month, year) - day;
    for (int m = month + 1; m <= 12; m++) {
        days += days_month(m, year);
    }
    for (int y = year + 1; y < new_year; y++) {
        days += (y % 4 == 0 && (y % 100 != 0 || y % 400 == 0)) ? 366 : 365;
    }
    for (int m = 1; m < new_month; m++) {
        days += days_month(m, new_year);
    }
    days += new_day;

    return days;
}
int days_spoilage(int days_delivery, int storage_lifetime, int delivery_day, int delivery_month, int delivery_year, int current_day, int current_month, int current_year) {
    int days_since_delivery = calculate_days_between_dates(delivery_day, delivery_month, delivery_year, current_day, current_month, current_year);

    int total_days = days_delivery + days_since_delivery;

    return storage_lifetime - total_days;
}

#endif