/******************
Name: Ido Itzhak
ID: 69420
Assignment: ex3
*******************/

#include <stdio.h>

#define NUM_OF_BRANDS 5
#define BRANDS_NAMES 15
#define NUM_OF_TYPES 4
#define TYPES_NAMES 10
#define DAYS_IN_YEAR 365
#define addOne  1
#define addAll  2
#define stats  3
#define print  4
#define insights  5
#define deltas  6
#define done  7

char brands[NUM_OF_BRANDS][BRANDS_NAMES] = {"Toyoga", "HyunNight", "Mazduh", "FolksVegan", "Key-Yuh"};
char types[NUM_OF_TYPES][TYPES_NAMES] = {"SUV", "Sedan", "Coupe", "GT"};

void printMenu();

void printBrandName(int brandIndex);

void printTypeName(int typeIndex);

void initializeData(int array[][NUM_OF_BRANDS][NUM_OF_TYPES], int days, int brands, int type);

int getBrandInput1();

int getBrandInput2(int today, int days[]);

void getSales(int sales[]);

int getDayInput(int days[]);

void populateDay(int data[][NUM_OF_BRANDS][NUM_OF_TYPES], int brand, int sales[], int days[]);

int min(int array[], int size);

int max(int array[], int size);

int maxIndex(int array[], int size);

int salesSum(int day, int data[][NUM_OF_BRANDS][NUM_OF_TYPES]);

int brandDailySum(int data[][NUM_OF_BRANDS][NUM_OF_TYPES], int brandIndex, int day);

int bestBrandIndex(int data[][NUM_OF_BRANDS][NUM_OF_TYPES], int day);

int typeDailySum(int data[][NUM_OF_BRANDS][NUM_OF_TYPES], int typeIndex, int day);

int bestTypeIndex(int data[][NUM_OF_BRANDS][NUM_OF_TYPES], int day);

void printData(int data[][NUM_OF_BRANDS][NUM_OF_TYPES], int brand, int days[]);

int overallBrandSales(int data[][NUM_OF_BRANDS][NUM_OF_TYPES], int brand);

int bestBrandSales(int data[][NUM_OF_BRANDS][NUM_OF_TYPES]);

int overallTypeSales(int data[][NUM_OF_BRANDS][NUM_OF_TYPES], int type);

int bestTypeSales(int data[][NUM_OF_BRANDS][NUM_OF_TYPES]);

int overallDailySales(int data[][NUM_OF_BRANDS][NUM_OF_TYPES], int day);

int bestDailySales(int data[][NUM_OF_BRANDS][NUM_OF_TYPES]);

float averageDelta(int data[][NUM_OF_BRANDS][NUM_OF_TYPES], int brand, int days[]);

int main() {
    //Part 0
    int database[DAYS_IN_YEAR][NUM_OF_BRANDS][NUM_OF_TYPES];
    int days[NUM_OF_BRANDS] = {0};
    int dailySales[NUM_OF_TYPES];
    initializeData(database, DAYS_IN_YEAR, NUM_OF_BRANDS, NUM_OF_TYPES);
    int menuChoice;
    int brandChoice;
    int dayChoice;
    //Go into Menu
    printMenu();
    scanf("%d", &menuChoice);
    while (menuChoice != done) {
        switch (menuChoice) {
            //Enter data for one brand
            case addOne:
                printf("What brand?\n");
                brandChoice = getBrandInput1();
                getSales(dailySales);
                populateDay(database, brandChoice, dailySales, days);
                break;

            //Enter for all brands
            case addAll:
                //look for the first day that has missing data
                int today = min(days, NUM_OF_BRANDS) + 1;
                int dataIsValid = 0;
                while (!dataIsValid) {
                    printf("No data for brands ");
                    //print missing brands
                    for (int i = 0; i < NUM_OF_BRANDS; i++) {
                        if (days[i] < today) {
                            printBrandName(i);
                            printf(" ");
                        }
                    }
                    printf("\nPlease complete the data\n");
                    // get user input
                    brandChoice = getBrandInput2(today, days);
                    if (brandChoice == -1)
                        continue;
                    getSales(dailySales);
                    // if input is valid, add to database.
                    populateDay(database, brandChoice, dailySales, days);
                    //if all data is up to date for all brands, finish this case.
                    if (min(days,NUM_OF_BRANDS) == today) {
                        dataIsValid = 1;
                    }
                }
                break;

            case stats:
                do {
                    printf("What day would you like to analyze?\n");
                    dayChoice = getDayInput(days);
                } while (dayChoice == -1);
                printf("In day number %d:\n", dayChoice);
                printf("The sales total was %d\n", salesSum(dayChoice, database));
                int bestBrand = bestBrandIndex(database, dayChoice);
                printf("The best sold brand with %d sales was ", brandDailySum(database, bestBrand, dayChoice));
                printBrandName(bestBrand);
                printf("\n");
                int bestType = bestTypeIndex(database, dayChoice);
                printf("The best sold type with %d sales was ", typeDailySum(database, bestType, dayChoice));
                printTypeName(bestType);
                printf("\n\n");
                break;

            case print:
                printf("*****************************************\n\n");
                for (int brand = 0; brand < NUM_OF_BRANDS; brand++) {
                    printf("Sales for ");
                    printBrandName(brand);
                    printf(":\n");
                    printData(database, brand, days);
                }
                printf("\n*****************************************\n");
                break;

            case insights:
                int overallBestDay = bestDailySales(database);
                int overallBestBrand = bestBrandSales(database);
                int overallBestType = bestTypeSales(database);
                printf("The best-selling brand overall is ");
                printBrandName(overallBestBrand);
                printf(": %d$\n", overallBrandSales(database, overallBestBrand));
                printf("The best-selling type of car is ");
                printTypeName(overallBestType);
                printf(": %d$\n", overallTypeSales(database, overallBestType));
                printf("The most profitable day was day number %d: %d$\n", overallBestDay,
                       overallDailySales(database, overallBestDay));
                break;

            case deltas:
                    for (int brand=0; brand<NUM_OF_BRANDS;brand++) {
                        printf("Brand: ");
                        printBrandName(brand);
                        printf(", Average Delta: %f\n", averageDelta(database, brand, days));
                    }
                break;
            default:
                printf("Invalid input\n");
        }
        printMenu();
        scanf("%d", &menuChoice);
    }
    printf("Goodbye!\n");
    return 0;
}


void printMenu() {
    printf("Welcome to the Cars Data Cube! What would you like to do?\n"
        "1.Enter Daily Data For A Brand\n"
        "2.Populate A Day Of Sales For All Brands\n"
        "3.Provide Daily Stats\n"
        "4.Print All Data\n"
        "5.Provide Overall (simple) Insights\n"
        "6.Provide Average Delta Metrics\n"
        "7.exit\n");
}

void printBrandName(int brandIndex) {
    for (int i = 0; i < BRANDS_NAMES; i++) {
        if (brands[brandIndex][i] == '\0') {
            break;
        }
        printf("%c", brands[brandIndex][i]);
    }
}

void printTypeName(int typeIndex) {
    for (int i = 0; i < TYPES_NAMES; i++) {
        if (types[typeIndex][i] == '\0') {
            break;
        }
        printf("%c", types[typeIndex][i]);
    }
}

void initializeData(int array[][NUM_OF_BRANDS][NUM_OF_TYPES], int days, int brands, int type) {
    for (int i = 0; i < days; i++) {
        for (int j = 0; j < brands; j++) {
            for (int k = 0; k < type; k++) {
                array[i][j][k] = -1;
            }
        }
    }
}

//scans and validates input and returns index of brand (case 1)
int getBrandInput1() {
    int userChoice;
    int validInput = 0;
    while (!validInput) {
        scanf("%d", &userChoice);
        if (userChoice >= 0 && userChoice <= NUM_OF_BRANDS - 1) {
            validInput = 1;
            return userChoice;
        }
        printf("No such brand.\n");
    }
    return -1;
}

//scans and validates input and returns index of brand (case 2)
int getBrandInput2(int today, int days[]) {
    int userChoice;
    scanf("%d", &userChoice);
    // checks if data already exists for this brand
    if (days[userChoice] > today) {
        printf("This brand is not valid\n");
        return -1;
    }
    //checks if brands exists
    if (userChoice >= 0 && userChoice <= NUM_OF_BRANDS - 1) {
        return userChoice;
    }
    printf("This brand is not valid.\n");
    return -1;
}

//enters user data to given array
void getSales(int sales[]) {
    int types = NUM_OF_TYPES;
    for (int i = 0; i < types; i++) {
        scanf("%d", &sales[i]);
    }
}

//scans and validates input and returns day
int getDayInput(int days[]) {
    int input;
    scanf("%d", &input);
    if (input > max(days, NUM_OF_BRANDS) || input < 0) {
        printf("Please enter a valid day.\n");
        return -1;
    }
    return input;
}

//enter a day to database
void populateDay(int data[][NUM_OF_BRANDS][NUM_OF_TYPES], int brand, int sales[], int days[]) {
    days[brand]++;
    for (int i = 0; i < NUM_OF_TYPES; i++) {
        data[days[brand]][brand][i] = sales[i];
    }
}

//returns minimum value of given array
int min(int array[], int size) {
    int temp = array[0];
    for (int i = 1; i < size; i++) {
        if (temp < array[i]) {
            continue;
        }
        temp = array[i];
    }
    return temp;
}

//returns maximum value of given array
int max(int array[], int size) {
    int temp = array[0];
    for (int i = 1; i < size; i++) {
        if (temp > array[i]) {
            continue;
        }
        temp = array[i];
    }
    return temp;
}

//returns index of maximum value of given array
int maxIndex(int array[], int size) {
    int temp = array[0];
    int tempIndex = 0;
    for (int i = 1; i < size; i++) {
        if (temp > array[i]) {
            continue;
        }
        temp = array[i];
        tempIndex = i;
    }
    return tempIndex;
}

//returns the sum of sales in given day
int salesSum(int day, int data[][NUM_OF_BRANDS][NUM_OF_TYPES]) {
    int sum = 0;
    for (int i = 0; i < NUM_OF_BRANDS; i++) {
        for (int j = 0; j < NUM_OF_TYPES; j++) {
            sum += data[day][i][j];
        }
    }
    return sum;
}

//returns the sum of sales of given brand and day
int brandDailySum(int data[][NUM_OF_BRANDS][NUM_OF_TYPES], int brandIndex, int day) {
    int sum = 0;
    for (int i = 0; i < NUM_OF_TYPES; i++) {
        sum += data[day][brandIndex][i];
    }
    return sum;
}

//returns the index of most selling brand in given day
int bestBrandIndex(int data[][NUM_OF_BRANDS][NUM_OF_TYPES], int day) {
    int brandSums[NUM_OF_BRANDS];
    for (int i = 0; i < NUM_OF_BRANDS; i++) {
        brandSums[i] = brandDailySum(data, i, day);
    }
    return maxIndex(brandSums, NUM_OF_BRANDS);
}

//returns the sum of sales of given car type and day
int typeDailySum(int data[][NUM_OF_BRANDS][NUM_OF_TYPES], int typeIndex, int day) {
    int sum = 0;
    for (int i = 0; i < NUM_OF_BRANDS; i++) {
        sum += data[day][i][typeIndex];
    }
    return sum;
}

//returns the index of most selling car type in given day
int bestTypeIndex(int data[][NUM_OF_BRANDS][NUM_OF_TYPES], int day) {
    int typeSums[NUM_OF_BRANDS];
    for (int i = 0; i < NUM_OF_BRANDS; i++) {
        typeSums[i] = typeDailySum(data, i, day);
    }
    return maxIndex(typeSums, NUM_OF_BRANDS);
}

//prints sales data of given brand and day
void printData(int data[][NUM_OF_BRANDS][NUM_OF_TYPES], int brand, int days[]) {
    for (int day = 1; day <= days[brand]; day++) {
        printf("Day %d- ", day);
        for (int type = 0; type < NUM_OF_TYPES; type++) {
            printTypeName(type);
            printf(": %d ", data[day][brand][type]);
        }
        printf("\n");
    }
}

//returns the sum of all sales of given brand
int overallBrandSales(int data[][NUM_OF_BRANDS][NUM_OF_TYPES], int brand) {
    int sum = 0;
    for (int i = 1; i < DAYS_IN_YEAR; i++) {
        for (int j = 0; j < NUM_OF_TYPES; j++) {
            if (data[i][brand][j] >= 0) {
                sum += data[i][brand][j];
            }
        }
    }
    return sum;
}

//returns the index of the most selling brand overall
int bestBrandSales(int data[][NUM_OF_BRANDS][NUM_OF_TYPES]) {
    int brandSums[NUM_OF_BRANDS];
    for (int i = 0; i < NUM_OF_BRANDS; i++) {
        brandSums[i] = overallBrandSales(data, i);
    }
    return maxIndex(brandSums, NUM_OF_BRANDS);
}

//return the sum of all sales of given type
int overallTypeSales(int data[][NUM_OF_BRANDS][NUM_OF_TYPES], int type) {
    int sum = 0;
    for (int i = 1; i < DAYS_IN_YEAR; i++) {
        for (int j = 0; j < NUM_OF_BRANDS; j++) {
            if (data[i][j][type] >= 0) {
                sum += data[i][j][type];
            }
        }
    }
    return sum;
}

//returns the index of the most selling type overall
int bestTypeSales(int data[][NUM_OF_BRANDS][NUM_OF_TYPES]) {
    int typeSums[NUM_OF_TYPES];
    for (int i = 0; i < NUM_OF_TYPES; i++) {
        typeSums[i] = overallTypeSales(data, i);
    }
    return maxIndex(typeSums, NUM_OF_TYPES);
}

//return the sum of all sales in given day
int overallDailySales(int data[][NUM_OF_BRANDS][NUM_OF_TYPES], int day) {
    int sum = 0;
    for (int i = 0; i < NUM_OF_BRANDS; i++) {
        for (int j = 0; j < NUM_OF_TYPES; j++) {
            sum += data[day][i][j];
        }
    }
    return sum;
}

//returns the index of the most selling type day
int bestDailySales(int data[][NUM_OF_BRANDS][NUM_OF_TYPES]) {
    int dailySums[DAYS_IN_YEAR];
    for (int i = 0; i < DAYS_IN_YEAR; i++) {
        dailySums[i] = overallDailySales(data, i);
    }
    return maxIndex(dailySums, DAYS_IN_YEAR);
}

//calculates average delta of given brand. (implicit casting of int to float)
float averageDelta(int data[][NUM_OF_BRANDS][NUM_OF_TYPES], int brand, int days[]) {
    float sum = 0;
    for(int day=2; day<=days[brand];day++) {
        float todayRevenue = brandDailySum(data, brand, day);
        float yesterdayRevenue = brandDailySum(data, brand, day-1);
        sum+=(todayRevenue-yesterdayRevenue);
    }
    float average = sum/(days[brand]-1);
    return average;
}