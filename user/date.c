#include "kernel/types.h"
#include "user/user.h"

#define NS_PER_SEC 1000000000L
#define SEC_PER_MIN 60
#define MIN_PER_HOUR 60
#define HOUR_PER_DAY 24
#define SEC_PER_HOUR (SEC_PER_MIN * MIN_PER_HOUR)
#define SEC_PER_DAY (SEC_PER_HOUR * HOUR_PER_DAY)


int is_leap(int year) {
    return (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
}

int days_in_month(int year, int month) {
    int days[] = {
        31, 28, 31, 30, 31, 30,
        31, 31, 30, 31, 30, 31 
    };

    if (month == 1 && is_leap(year))
        return 29;
    return days[month];
}

void print2dec(int n) {
    if (n < 10) printf("0");
    printf("%d", n);
}

int main(int argc, char *argv[]) {
    uint64 ns = rtc();
    uint64 sec = ns / NS_PER_SEC;
    uint64 nsec = ns % NS_PER_SEC;

    int year = 1970;
    while (1) {
        int days_in_year = is_leap(year) ? 366 : 365;
        if (sec >= days_in_year * SEC_PER_DAY) {
            sec -= days_in_year * SEC_PER_DAY;
            year++;
        } else break;
    }

    int month = 0;
    while (1) {
        int dim = days_in_month(year, month);
        if (sec >= dim * SEC_PER_DAY) {
            sec -= dim * SEC_PER_DAY;
            month++;
        } else break;
    }

    int day = sec / SEC_PER_DAY + 1;
    sec %= SEC_PER_DAY;
    int hour = sec / SEC_PER_HOUR;
    sec %= SEC_PER_HOUR;
    int min = sec / SEC_PER_MIN;
    int second = sec % SEC_PER_MIN;


    printf("%d-", year);
    print2dec(month + 1); printf("-");
    print2dec(day); printf(" ");
    print2dec(hour); printf(":");
    print2dec(min); printf(":");
    print2dec(second); printf(".");

    nsec /= 1000000;
    if (nsec < 10)
        printf("00%d\n", (int)nsec);
    else if (nsec < 100)
        printf("0%d\n", (int)nsec);
    else 
        printf("%d\n", (int)nsec);

    exit(0);
}
