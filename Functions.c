#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>
#include "Functions.h"

/* 
Changes the text color in the console. 
Parameters: 
  - color: An integer representing the color code for the text. 
Returns: 
  - Nothing. 
Variables: 
  - hConsole: A handle to the console, retrieved using GetStdHandle(STD_OUTPUT_HANDLE). 
Usage: 
  - This function is used to set the text color in the console output, 
    making the interface visually distinct. 
*/
void SetColor(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}


/* 
Determines the number of records in a binary file based on the record size. 
Parameters: 
  - fileName: A string containing the name of the binary file to analyze. 
  - recordSize: The size (in bytes) of a single record in the file. 
Returns: 
  - The number of records in the file (int). Returns 0 if the file cannot be opened 
    or `recordSize` is zero. 
Variables: 
  - fp: A pointer to the file being processed, opened in binary mode ("rb"). 
  - fileSize: A long integer storing the total size of the file in bytes. 
  - numRecords: An integer storing the calculated number of records. 
Usage: 
  - Useful for determining the number of records in a structured binary file. 
*/
int TellNumRecords(char fileName[], int recordSize) {
    FILE *fp = fopen(fileName, "rb"); // Open in binary mode.
    if (fp == NULL) {
        return 0; // Return 0 if the file cannot be opened.
    }

    fseek(fp, 0, SEEK_END); // Move the pointer to the end of the file.
    long fileSize = ftell(fp); // Get the size of the file in bytes.
    fclose(fp);

    // Calculate the number of records.
    if (recordSize == 0) {
        return 0; // Avoid division by zero.
    }

    int numRecords = fileSize / recordSize;

    return numRecords;
}



/* 
Reads data from a file and writes it into a binary sales table file. 
Parameters: 
  - originFileName: The name of the source file from which to read data. 
Returns: 
  - Nothing. 
Variables: 
  - fpOrigin: A file pointer for the source file opened in text mode ("r"). 
  - fpSale: A file pointer for the destination sales table, opened in binary mode ("wb"). 
  - recordSales: A structure where sales record data is stored before writing to the sales file. 
  - line: A string buffer used to store each line of input from the source file. 
  - numberRecord: A counter for the number of records processed. 
Usage: 
  - Used to convert CSV data into a binary format and store it in a sales table. 
*/
void CreateSalesTable(char originFileName[]) {
    FILE *fpOrigin = fopen(originFileName, "r");
    FILE *fpSale = fopen("SalesTable", "wb");

    if(fpOrigin == NULL){
        printf("Error opening the source file.");
        return;
    }
    if(fpSale == NULL){
        printf("Error opening the SalesTable file.");
        return;
    }

    Sales recordSales;
    char line[1024] = "";
    int numberRecord = 0;

    while (fgets(line, sizeof(line), fpOrigin)){
        char *token = strtok(line, ",");
        recordSales.OrderNumber = atol(token);

        token = strtok(NULL, ",");
        recordSales.LineItems = (unsigned char) atoi(token);

        token = strtok(NULL, ",");
        unsigned int day = 0, month = 0, year = 0;
        sscanf(token, "%u/%u/%u", &month, &day, &year);
        recordSales.OrderDate.MM = (unsigned char) month;
        recordSales.OrderDate.DD = (unsigned char) day;
        recordSales.OrderDate.AAAA = (unsigned short int) year;

        token = strtok(NULL, ",");
        if (token[0] == ' '){
            recordSales.DeliveryDate.MM = 0;
            recordSales.DeliveryDate.DD = 0;
            recordSales.DeliveryDate.AAAA = 0;
        } else {
            sscanf(token, "%u/%u/%u", &month, &day, &year);
            recordSales.DeliveryDate.MM = (unsigned char) month;
            recordSales.DeliveryDate.DD = (unsigned char) day;
            recordSales.DeliveryDate.AAAA = (unsigned short int) year;
        }

        token = strtok(NULL, ",");
        recordSales.CustomerKey = (unsigned long int) atoi(token);

        token = strtok(NULL, ",");
        recordSales.StoreKey = (unsigned short int) atoi(token);

        token = strtok(NULL, ",");
        recordSales.ProductKey = (unsigned short int) atoi(token);

        token = strtok(NULL, ",");
        recordSales.Quantity = (unsigned short int) atoi(token);

        token = strtok(NULL, ",");
        strcpy(recordSales.CurrencyCode, token);

        fwrite(&recordSales, sizeof(recordSales), 1, fpSale);

        /* 
        Uncomment to print each record for debugging:
        printf("Record: %i\n\n",numberRecord);
        printf("OrderNumber :%ld\n", recordSales.OrderNumber);
        printf("LineItem    :%u\n", recordSales.LineItems);
        printf("OrderDate   :%u/%u/%hu\n", recordSales.OrderDate.MM, recordSales.OrderDate.DD, recordSales.OrderDate.AAAA);
        printf("DeliveryDate:%u/%u/%hu\n", recordSales.DeliveryDate.MM, recordSales.DeliveryDate.DD, recordSales.DeliveryDate.AAAA);
        printf("CustomerKey :%u\n", recordSales.CustomerKey);
        printf("StoreKey    :%hu\n", recordSales.StoreKey);
        printf("ProductKey  :%hu\n", recordSales.ProductKey);
        printf("Quantity    :%hu\n", recordSales.Quantity);
        printf("CurrencyCode:%s\n\n\n", recordSales.CurrencyCode);
        */

        numberRecord++;
    }

    fclose(fpOrigin); fclose(fpSale);
}


/* 
Reads data from a file and writes it into a binary customers table file. 
Parameters: 
  - originFileName: The name of the source file from which to read data. 
Returns: 
  - Nothing. 
Variables: 
  - fpOrigin: A file pointer for the source file opened in text mode ("r"). 
  - fpCustomer: A file pointer for the destination customers table, opened in binary mode ("wb"). 
  - recordCustomer: A structure where customer data is stored before writing to the customers file. 
  - line: A string buffer used to store each line of input from the source file. 
  - numberRecord: A counter for the number of records processed. 
Usage: 
  - Used to convert semicolon-separated data into a binary format and store it in a customers table. 
*/
void CreateCustomersTable(char originFileName[]) {
    FILE *fpOrigin = fopen(originFileName, "r");
    FILE *fpCustomer = fopen("CustomersTable", "wb");

    if(fpOrigin == NULL){
        printf("Error opening the source file.");
        return;
    }
    if(fpCustomer == NULL){
        printf("Error opening the CustomerTable file.");
        return;
    }

    Customers recordCustomer;
    char line[400] = "";
    int numberRecord = 0;

    while (fgets(line, sizeof(line), fpOrigin)){
        if(numberRecord != 0){
            char *token = strtok(line, ";");
            recordCustomer.CustomerKey = (unsigned long int) atoi(token);

            token = strtok(NULL, ";");
            strcpy(recordCustomer.Gender, token);

            token = strtok(NULL, ";");
            strcpy(recordCustomer.Name, token);

            token = strtok(NULL, ";");
            strcpy(recordCustomer.City, token);

            token = strtok(NULL, ";");
            strcpy(recordCustomer.StateCode, token);

            token = strtok(NULL, ";");
            strcpy(recordCustomer.State, token);

            token = strtok(NULL, ";");
            if (token[0] >= 48 && token[0] <= 57){
                recordCustomer.ZipCode = (unsigned long int) atoi(token);
                token = strtok(NULL, ";");
            } else {
                recordCustomer.ZipCode = 0;
            }
            strcpy(recordCustomer.Country, token);

            token = strtok(NULL, ";");
            strcpy(recordCustomer.Continent, token);

            token = strtok(NULL, ";");
            unsigned int day = 0, month = 0, year = 0;
            sscanf(token, "%u/%u/%u", &month, &day, &year);
            recordCustomer.Birthday.MM = (unsigned char) month;
            recordCustomer.Birthday.DD = (unsigned char) day;
            recordCustomer.Birthday.AAAA = (unsigned short int) year;

            fwrite(&recordCustomer, sizeof(recordCustomer), 1, fpCustomer);

            /* 
            Uncomment to print each record for debugging:
            printf("Record: %i\n\n", numberRecord);
            printf("CustomerKey:%u\n", recordCustomer.CustomerKey);
            printf("Gender     :%s\n", recordCustomer.Gender);
            printf("Name       :%s\n", recordCustomer.Name);
            printf("City       :%s\n", recordCustomer.City);
            printf("StateCode  :%s\n", recordCustomer.StateCode);
            printf("State      :%s\n", recordCustomer.State);
            printf("ZipCode    :%u\n", recordCustomer.ZipCode);
            printf("Country    :%s\n", recordCustomer.Country);
            printf("Continent  :%s\n", recordCustomer.Continent);
            printf("Birthday   :%u/%u/%hu\n", recordCustomer.Birthday.MM, recordCustomer.Birthday.DD, recordCustomer.Birthday.AAAA);
            printf("\n\n\n");
            */
        }
        numberRecord++;
    }

    fclose(fpOrigin); fclose(fpCustomer);
}

/* 
Reads data from a file and writes it into a binary products table file. 
Parameters: 
  - originFileName: The name of the source file from which to read data. 
Returns: 
  - Nothing. 
Variables: 
  - fpOrigin: A file pointer for the source file opened in text mode ("r"). 
  - fpProduct: A file pointer for the destination products table, opened in binary mode ("wb"). 
  - recordProduct: A structure where product data is stored before writing to the products file. 
  - line: A string buffer used to store each line of input from the source file. 
  - numberRecord: A counter for the number of records processed. 
Usage: 
  - Used to convert CSV data into a binary format and store it in a products table. 
*/
void CreateProductsTable(char originFileName[]) {
    FILE *fpOrigin = fopen(originFileName, "r");
    FILE *fpProduct = fopen("ProductsTable", "wb");

    if(fpOrigin == NULL){
        printf("Error opening the source file.");
        return;
    }
    if(fpProduct == NULL){
        printf("Error opening the ProductsTable file.");
        return;
    }

    Products recordProduct;
    char line[300] = "";
    int numberRecord = 0;

    while (fgets(line, sizeof(line), fpOrigin)){
        if(numberRecord != 0){
            char *token = strtok(line, ","), temp[200] = "";
            recordProduct.ProductKey = (unsigned short int) atoi(token);

            token = strtok(NULL, ",");
            if(token[0] == '"'){
                while (token[strlen(token) - 1] != '"'){
                    strcat(token, strtok(NULL, ","));
                }
                strncpy(recordProduct.ProductName, token + 1, strlen(token));
                recordProduct.ProductName[strlen(recordProduct.ProductName) - 1] = '\0';
            } else {
                strcpy(recordProduct.ProductName, token);
            }

            token = strtok(NULL, ",");
            strcpy(recordProduct.Brand, token);

            token = strtok(NULL, ",");
            strcpy(recordProduct.Color, token);

            token = strtok(NULL, ",");
            if(token[0] == '"'){
                strcat(token, strtok(NULL, ","));
                recordProduct.UnitCostUSD = atof(strncpy(temp, token + 2, strlen(token)));
            } else {
                recordProduct.UnitCostUSD = atof(strncpy(temp, token + 1, strlen(token)));
            }

            token = strtok(NULL, ",");
            if(token[0] == '"'){
                strcat(token, strtok(NULL, ","));
                recordProduct.UnitPriceUSD = atof(strncpy(temp, token + 2, strlen(token)));
            } else {
                recordProduct.UnitPriceUSD = atof(strncpy(temp, token + 1, strlen(token)));
            }

            token = strtok(NULL, ",");
            strcpy(recordProduct.SubcategoryKey, token);

            token = strtok(NULL, ",");
            if(token[0] == '"'){
                strcat(token, strtok(NULL, ","));
                strncpy(recordProduct.Subcategory, token + 1, strlen(token));
                recordProduct.Subcategory[strlen(recordProduct.Subcategory) - 1] = '\0';
            } else {
                strcpy(recordProduct.Subcategory, token);
            }

            token = strtok(NULL, ",");
            strcpy(recordProduct.CategoryKey, token);

            token = strtok(NULL, ",");
            if(token[0] == '"'){
                strcat(token, strtok(NULL, ","));
                strncpy(recordProduct.Category, token + 1, strlen(token));
                recordProduct.Category[strlen(recordProduct.Category) - 1] = '\0';
            } else {
                strcpy(recordProduct.Category, token);
            }

            fwrite(&recordProduct, sizeof(recordProduct), 1, fpProduct);

            /* 
            Uncomment to print each record for debugging:
            printf("\nRecord : %i\n\n", numberRecord);
            printf("ProductKey    :%hu\n", recordProduct.ProductKey);
            printf("ProductName   :%s\n", recordProduct.ProductName);
            printf("Brand         :%s\n", recordProduct.Brand);
            printf("Color         :%s\n", recordProduct.Color);
            printf("UnitCostUSD   :%.2f\n", recordProduct.UnitCostUSD);
            printf("UnitPriceUSD  :%.2f\n", recordProduct.UnitPriceUSD);
            printf("SubCategoryKey:%s\n", recordProduct.SubcategoryKey);
            printf("SubCategory   :%s\n", recordProduct.Subcategory);
            printf("CategoryKey   :%s\n", recordProduct.CategoryKey);
            printf("Category      :%s\n", recordProduct.Category);
            */
        }
        numberRecord++;
    }

    fclose(fpOrigin); fclose(fpProduct);
    return;
}


/* 
Reads data from a file and writes it into a binary stores table file. 
Parameters: 
  - originFileName: The name of the source file from which to read data. 
Returns: 
  - Nothing. 
Variables: 
  - fpOrigin: A file pointer for the source file opened in text mode ("r"). 
  - fpStore: A file pointer for the destination stores table, opened in binary mode ("wb"). 
  - recordStore: A structure where store data is stored before writing to the stores file. 
  - line: A string buffer used to store each line of input from the source file. 
  - numberRecord: A counter for the number of records processed. 
Usage: 
  - Used to convert CSV data into a binary format and store it in a stores table. 
*/
void CreateStoresTable(char originFileName[]) {
    FILE *fpOrigin = fopen(originFileName, "r");
    FILE *fpStore = fopen("StoresTable", "wb");

    if(fpOrigin == NULL){
        printf("Error opening the source file.");
        return;
    }
    if(fpStore == NULL){
        printf("Error opening the StoresTable file.");
        return;
    }

    Stores recordStore;
    char line[200] = "";
    int numberRecord = 0;

    while (fgets(line, sizeof(line), fpOrigin)){
        if (numberRecord != 0){
            char *token = strtok(line, ",");
            recordStore.StoreKey = (unsigned short int) atoi(token);

            token = strtok(NULL, ",");
            strcpy(recordStore.Country, token);

            token = strtok(NULL, ",");
            strcpy(recordStore.State, token);

            token = strtok(NULL, ",");
            if(token[1] == '/' || token [2] == '/'){
                recordStore.SquareMeters = 0;
            } else {
                recordStore.SquareMeters = (unsigned short int) atoi(token);
                token = strtok(NULL, ",");
            }

            unsigned int day = 0, month = 0, year = 0;
            sscanf(token, "%u/%u/%u", &month, &day, &year);
            recordStore.OpenDate.MM = (unsigned char) month;
            recordStore.OpenDate.DD = (unsigned char) day;
            recordStore.OpenDate.AAAA = (unsigned short int) year;

            fwrite(&recordStore, sizeof(recordStore), 1, fpStore);

            /* 
            Uncomment to print each record for debugging:
            printf("Record: %i\n\n", numberRecord);
            printf("StoreKey    :%hu\n", recordStore.StoreKey);
            printf("Country     :%s\n", recordStore.Country);
            printf("State       :%s\n", recordStore.State);
            printf("SquareMeters:%hu\n", recordStore.SquareMeters);
            printf("OpenDate    :%u/%u/%hu\n\n", recordStore.OpenDate.DD, recordStore.OpenDate.MM, recordStore.OpenDate.AAAA);
            */
        }
        numberRecord++;
    }

    fclose(fpOrigin); fclose(fpStore);
    return;
}


/* 
Reads data from a file and writes it into a binary exchange rates table file. 
Parameters: 
  - originFileName: The name of the source file from which to read data. 
Returns: 
  - Nothing. 
Variables: 
  - fpOrigin: A file pointer for the source file opened in text mode ("r"). 
  - fpExchange: A file pointer for the destination exchange rates table, opened in binary mode ("wb"). 
  - recordExchange: A structure where exchange rate data is stored before writing to the exchange rates file. 
  - line: A string buffer used to store each line of input from the source file. 
  - numberRecord: A counter for the number of records processed. 
Usage: 
  - Used to convert CSV data into a binary format and store it in an exchange rates table. 
*/
void CreateExchangeRatesTable(char originFileName[]) {
    FILE *fpOrigin = fopen(originFileName, "r");
    FILE *fpExchange = fopen("ExchangeRatesTable", "wb");

    if(fpOrigin == NULL){
        printf("Error opening the source file.");
        return;
    }
    if(fpExchange == NULL){
        printf("Error opening the ExchangeRates file.");
        return;
    }

    ExchangeRates recordExchange;
    char line[30] = "";
    int numberRecord = 0;

    while (fgets(line, sizeof(line), fpOrigin)){
        if(numberRecord != 0){
            char *token = strtok(line, ",");
            strcpy(recordExchange.Date, token);

            token = strtok(NULL, ",");
            strncpy(recordExchange.Currency, token, 3);
            recordExchange.Currency[3] = '\0';

            token = strtok(NULL, ",");
            recordExchange.Exchange = atof(token);

            fwrite(&recordExchange, sizeof(recordExchange), 1, fpExchange);

            /* 
            Uncomment to print each record for debugging:
            printf("Record: %i\n\n", numberRecord);
            printf("Date    : %s\n", recordExchange.Date);
            printf("Currency: %s\n", recordExchange.Currency);
            printf("Exchange: %.2f\n\n", recordExchange.Exchange);
            */
        }
        numberRecord++;
    }

    fclose(fpOrigin); fclose(fpExchange);
    return;
}


/* 
Calls the functions to create all the required tables with the dataset. 
No parameters or return value. 
*/
void CreateTablesWithDataset() {
    CreateExchangeRatesTable("Exchange_Rates.csv");
    CreateSalesTable("Sales.csv");
    CreateCustomersTable("Customers.csv");
    CreateStoresTable("Stores.csv");
    CreateProductsTable("Products.csv");
}


/* 
Performs a binary search on a binary file based on the specified record type (Products, Customers, or Sales).
Parameters: 
  - fp: File pointer to the binary file where the search will occur. 
  - valueToSearch: The value to search for in the specified record type.
  - file: The type of file (1 = Products, 2 = Customers, 3 = Sales).
Returns: 
  - The index of the record where the value is found, or -1 if not found. 
Variables:
  - start: The starting index of the search range.
  - middle: The middle index used for the binary search.
  - end: The ending index of the search range.
  - sizeOfRecord: The size of a single record in the file.
  - key: The key value used for comparison in the binary search.
Usage:
  - This function is used for efficient searching within large binary files.
*/
int BinarySearch(FILE *fp, unsigned long int valueToSearch, int file) {
    unsigned int start = 0, middle = 0, end = 0, sizeOfRecord = 0;
    unsigned long int key = 0;

    if(file == 1) {
        sizeOfRecord = sizeof(Products); // Size of Products record
    } else if(file == 2) {
        sizeOfRecord = sizeof(Customers);  // Size of Customers record  
    } else if(file == 3) {
        sizeOfRecord = sizeof(Sales); // Size of Sales record
    } else if(file == 4) {
        sizeOfRecord = sizeof(Sales); // Size of Sales record (same for both sales by product and customer)
    } else {
        return -1; // Invalid file type
    }

    fseek(fp, 0, SEEK_END);
    end = ( ftell(fp) / sizeOfRecord ) - 1; // Calculate the last index based on file size
    while(start <= end) {
        middle = start + ((end - start) / 2); // Find the middle index
        fseek(fp, middle * sizeOfRecord, SEEK_SET); // Seek to the middle record

        if (file == 1) {
            Products recordProduct;
            fread(&recordProduct, sizeOfRecord, 1, fp);
            key = (unsigned long int) recordProduct.ProductKey; // Get the key from the product record
        } else if (file == 2) {
            Customers recordCustomer;
            fread(&recordCustomer, sizeOfRecord, 1, fp);
            key = (unsigned long int) recordCustomer.CustomerKey; // Get the key from the customer record
        } else if (file == 3) {
            Sales recordSale;
            fread(&recordSale, sizeOfRecord, 1, fp);
            key = (unsigned long int) recordSale.ProductKey; // Get the key from the sale record
        } else if(file == 4) {
            Sales recordSale;
            fread(&recordSale, sizeOfRecord, 1, fp);
            key = (unsigned long int) recordSale.CustomerKey; // Get the key from the sale record (customer key)
        } else {
            return -1; // Invalid file type
        }

        if(key == valueToSearch) { // If the key matches the value being searched
            return middle; // Return the index of the matching record
        } else if(key < valueToSearch) {
            start = middle + 1; // Move the start to the right half
        } else {
            end = middle - 1; // Move the end to the left half
        }
    }
    return -1; // Return -1 if the value is not found
}

/* 
Performs a binary search to match the exchange rate date for a given sales record.
Parameters: 
  - fp: File pointer to the binary file where exchange rates are stored.
  - recordSale: The sales record used to match the exchange date.
Returns: 
  - The index of the exchange rate where the date matches, or -1 if not found.
Variables:
  - start: The starting index of the search range.
  - middle: The middle index used for the binary search.
  - end: The ending index of the search range.
  - sizeOfRecord: The size of a single record in the exchange rates file.
  - key: The date of the exchange rate being searched.
  - currentMonth, currentDay, currentYear: The month, day, and year of the exchange rate date.
  - month, day, year: The month, day, and year from the sales record used for comparison.
Usage: 
  - This function is used for finding the correct exchange rate based on the date of a sale.
*/
int BinarySearchExchangeDate(FILE *fp, Sales recordSale) {
    unsigned int start = 0, middle = 0, end = 0, sizeOfRecord = sizeof(ExchangeRates);
    ExchangeRates record;
    fseek(fp, 0, SEEK_END);
    end = ( ftell(fp) / sizeOfRecord ) - 1; // Calculate the last index based on file size

    while (start <= end) {
        middle = start + ((end - start) / 2); // Find the middle index
        fseek(fp, middle * sizeOfRecord, SEEK_SET); // Seek to the middle record
        fread(&record, sizeOfRecord, 1, fp); // Read the exchange rate record

        char key[11] = "";
        strcpy(key, record.Date); // Store the exchange rate date as a string

        int currentMonth = 0, currentDay = 0, currentYear = 0;
        int month = 0, day = 0, year = 0;
        month = recordSale.OrderDate.MM; // Get the month from the sales record
        day = recordSale.OrderDate.DD; // Get the day from the sales record
        year = recordSale.OrderDate.AAAA; // Get the year from the sales record

        sscanf(key, "%d/%d/%d", &currentMonth, &currentDay, &currentYear); // Parse the exchange rate date
        if(currentYear == year) {
            if(currentMonth == month) {
                if(currentDay == day) {
                    return middle; // Return the index if the dates match
                } else if(currentDay > day) {
                    end = middle - 1; // Move the end to the left half
                } else {
                    start = middle + 1; // Move the start to the right half
                }
            } else if(currentMonth > month) {
                end = middle - 1; // Move the end to the left half
            } else {
                start = middle + 1; // Move the start to the right half
            }
        } else if(currentYear > year) {
            end = middle - 1; // Move the end to the left half
        } else {
            start = middle + 1; // Move the start to the right half
        }
    }
    return -1; // Return -1 if the date is not found
}


/* 
Compares two Products structures by the product name for sorting purposes.
Parameters: 
  - a: Pointer to the first Products structure.
  - b: Pointer to the second Products structure.
Returns: 
  - A negative integer if the first product name is lexicographically less than the second.
  - A positive integer if the first product name is lexicographically greater.
  - Zero if both product names are the same.
Usage: 
  - This function is used in sorting functions (such as `qsort`) to sort products by name.
Variables:
  - prodA: Pointer to the first Products structure.
  - prodB: Pointer to the second Products structure.
Usage: 
  - This function is used for comparing product names in sorting functions.
*/
int CompareProductsByProductName(void *a, void *b) {
    Products *prodA = (Products *)a; // Pointer to the first product
    Products *prodB = (Products *)b; // Pointer to the second product
    return strcmp(prodA->ProductName, prodB->ProductName); // Compare product names lexicographically
}

/* 
Compares two Products structures by the ProductKey for sorting purposes.
Parameters: 
  - a: Pointer to the first Products structure.
  - b: Pointer to the second Products structure.
Returns: 
  - A negative integer if the first product's ProductKey is less than the second's.
  - A positive integer if the first product's ProductKey is greater than the second's.
  - Zero if both ProductKeys are the same.
Usage:
  - This function is used in sorting functions (such as `qsort`) to sort products by ProductKey.
Variables:
  - prodA: Pointer to the first Products structure.
  - prodB: Pointer to the second Products structure.
*/
int CompareProductsByProductKey(void *a, void *b) {
    Products *prodA = (Products *)a; // Pointer to the first product
    Products *prodB = (Products *)b; // Pointer to the second product
    return prodA->ProductKey - prodB->ProductKey; // Compare ProductKeys numerically
}


/* 
Compares two Customers structures by the CustomerKey for sorting purposes.
Parameters: 
  - a: Pointer to the first Customers structure.
  - b: Pointer to the second Customers structure.
Returns: 
  - A negative integer if the first customer's CustomerKey is less than the second's.
  - A positive integer if the first customer's CustomerKey is greater than the second's.
  - Zero if both CustomerKeys are the same.
Usage:
  - This function is used in sorting functions (such as `qsort`) to sort customers by CustomerKey.
Variables:
  - custA: Pointer to the first Customers structure.
  - custB: Pointer to the second Customers structure.
*/
int CompareCustomersByCustomerKey(void *a, void *b) {
    Customers *custA = (Customers *)a; // Pointer to the first customer
    Customers *custB = (Customers *)b; // Pointer to the second customer
    return custA->CustomerKey - custB->CustomerKey; // Compare CustomerKeys numerically
}

/* 
Compares two Customers structures by their location (Continent, Country, State, and City) for sorting purposes.
Parameters: 
  - a: Pointer to the first Customers structure.
  - b: Pointer to the second Customers structure.
Returns: 
  - A negative integer if the first customer’s location is lexicographically less than the second's.
  - A positive integer if the first customer’s location is lexicographically greater than the second's.
  - Zero if both customers have the same location.
Usage:
  - This function is used in sorting functions (such as `qsort`) to sort customers by their location.
Variables:
  - result: Holds the result of comparison between the location fields.
  - custA: Pointer to the first Customers structure.
  - custB: Pointer to the second Customers structure.
*/
int CompareCustomersByCustomerLocation(void *a, void *b) {
    int result = 0; 
    Customers *custA = (Customers *)a; // Pointer to the first customer
    Customers *custB = (Customers *)b; // Pointer to the second customer

    result = strcmp(custA->Continent, custB->Continent); // Compare by Continent
    if(result != 0) {
        return result; // If continents differ, return the comparison result
    }

    result = strcmp(custA->Country, custB->Country); // Compare by Country
    if(result != 0) {
        return result; // If countries differ, return the comparison result
    }

    result = strcmp(custA->State, custB->State); // Compare by State
    if(result != 0) {
        return result; // If states differ, return the comparison result
    }

    result = strcmp(custA->City, custB->City); // Compare by City
    return result; // Return the comparison result based on City
}

/* 
Compares two Customers structures by the Customer's Name for sorting purposes.
Parameters: 
  - a: Pointer to the first Customers structure.
  - b: Pointer to the second Customers structure.
Returns: 
  - A negative integer if the first customer’s Name is lexicographically less than the second's.
  - A positive integer if the first customer’s Name is lexicographically greater than the second's.
  - Zero if both customers have the same Name.
Usage:
  - This function is used in sorting functions (such as `qsort`) to sort customers by Name.
Variables:
  - custA: Pointer to the first Customers structure.
  - custB: Pointer to the second Customers structure.
*/
int CompareCustomersByName(void *a, void *b) {
    Customers *custA = (Customers *)a; // Pointer to the first customer
    Customers *custB = (Customers *)b; // Pointer to the second customer
    return strcmp(custA->Name, custB->Name); // Compare customer names lexicographically
}

/* 
Compares two Sales structures by the ProductKey for sorting purposes.
Parameters: 
  - a: Pointer to the first Sales structure.
  - b: Pointer to the second Sales structure.
Returns: 
  - A negative integer if the first sale's ProductKey is less than the second's.
  - A positive integer if the first sale's ProductKey is greater than the second's.
  - Zero if both ProductKeys are the same.
Usage:
  - This function is used in sorting functions (such as `qsort`) to sort sales by ProductKey.
Variables:
  - saleA: Pointer to the first Sale structure.
  - saleB: Pointer to the second Sale structure.
*/
int CompareSalesByProductKey(void *a, void *b) {
    Sales *saleA = (Sales *)a; // Pointer to the first sale
    Sales *saleB = (Sales *)b; // Pointer to the second sale
    return (int)(saleA->ProductKey - saleB->ProductKey); // Compare ProductKeys numerically
}

/* 
Compares two Sales structures by the CustomerKey for sorting purposes.
Parameters: 
  - a: Pointer to the first Sales structure.
  - b: Pointer to the second Sales structure.
Returns: 
  - A negative integer if the first sale's CustomerKey is less than the second's.
  - A positive integer if the first sale's CustomerKey is greater than the second's.
  - Zero if both CustomerKeys are the same.
Usage:
  - This function is used in sorting functions (such as `qsort`) to sort sales by CustomerKey.
Variables:
  - saleA: Pointer to the first Sale structure.
  - saleB: Pointer to the second Sale structure.
*/
int CompareSalesByCustomerKey(void *a, void *b) {
    Sales *saleA = (Sales *)a; // Pointer to the first sale
    Sales *saleB = (Sales *)b; // Pointer to the second sale
    return saleA->CustomerKey - saleB->CustomerKey; // Compare CustomerKeys numerically
}

/* 
Compares two Sales structures by OrderNumber, and if they are equal, compares by ProductKey.
Parameters: 
  - a: Pointer to the first Sales structure.
  - b: Pointer to the second Sales structure.
Returns: 
  - A negative integer if the first sale’s OrderNumber is less than the second's.
  - A positive integer if the first sale’s OrderNumber is greater than the second's.
  - Zero if both OrderNumbers are the same, then compares ProductKey.
Usage:
  - This function is used in sorting functions (such as `qsort`) to sort sales by OrderNumber, then ProductKey.
Variables:
  - saleA: Pointer to the first Sale structure.
  - saleB: Pointer to the second Sale structure.
  - comparation: Holds the comparison result between OrderNumbers.
*/
int CompareSalesByOrderNumber(void *a, void *b) {
    Sales *saleA = (Sales *)a; // Pointer to the first sale
    Sales *saleB = (Sales *)b; // Pointer to the second sale
    long int comparation = saleA->OrderNumber - saleB->OrderNumber; // Compare OrderNumbers

    if(comparation != 0) {
        return (int)comparation; // If OrderNumbers differ, return the comparison result
    } else {
        return (int)(saleA->ProductKey - saleB->ProductKey); // If OrderNumbers are equal, compare ProductKeys
    }
}


/* 
Compares two ExchangeRates structures by the CurrencyCode for sorting purposes.
Parameters: 
  - a: Pointer to the first ExchangeRates structure.
  - b: Pointer to the second ExchangeRates structure.
Returns: 
  - A negative integer if the first currency code is lexicographically less than the second's.
  - A positive integer if the first currency code is lexicographically greater than the second's.
  - Zero if both currency codes are the same.
Usage:
  - This function is used for sorting ExchangeRates by Currency.
Variables:
  - exchanA: Pointer to the first ExchangeRates structure.
  - exchanB: Pointer to the second ExchangeRates structure.
*/
int CompareExchangeByCurrencyCode(void *a, void *b) {
    ExchangeRates *exchanA = (ExchangeRates *)a; // Pointer to the first exchange rate record
    ExchangeRates *exchanB = (ExchangeRates *)b; // Pointer to the second exchange rate record
    return strcmp(exchanA->Currency, exchanB->Currency); // Compare Currency codes lexicographically
}


/* 
Merges two sorted sub-arrays into a single sorted array using the Merge Sort algorithm.
Parameters: 
  - array: Pointer to the array to be merged.
  - left: Starting index of the left sub-array.
  - right: Ending index of the right sub-array.
  - middle: Index of the middle element separating the two sub-arrays.
  - recordSize: Size of each record (element) in bytes.
  - compare: Comparison function to compare two records.
Usage:
  - This function is used by the Merge Sort algorithm to merge two sorted sub-arrays.
Variables:
  - firstArray: Size of the first sub-array.
  - secondArray: Size of the second sub-array.
  - temporalLeft: Temporary array to hold the first sub-array.
  - temporalRight: Temporary array to hold the second sub-array.
  - i: Iterator for the first sub-array.
  - j: Iterator for the second sub-array.
  - posicion: The position in the original array to place the merged element.
*/
void MergeArray(void *array, int left, int right, int middle, int recordSize, int (*compare)(void*, void*)) {
    int firstArray = middle - left + 1; // Number of elements in the first sub-array
    int secondArray = right - middle;   // Number of elements in the second sub-array
    void *temporalLeft = calloc(firstArray, recordSize);  // Allocate memory for the first sub-array
    void *temporalRight = calloc(secondArray, recordSize); // Allocate memory for the second sub-array

    // Copy the values of the first half into the temporalLeft array
    for (int i = 0; i < firstArray; i++) {
        memcpy((char*)temporalLeft + i * recordSize, (char*)array + (left + i) * recordSize, recordSize);
    }

    // Copy the values of the second half into the temporalRight array
    for (int j = 0; j < secondArray; j++) {
        memcpy((char*)temporalRight + j * recordSize, (char*)array + (middle + 1 + j) * recordSize, recordSize);
    }

    int i = 0, j = 0; 
    int posicion = left; // Position to place the merged elements in the original array

    // Merge the two sorted sub-arrays into the original array
    while (i < firstArray && j < secondArray) {
        if (compare((char*)temporalLeft + i * recordSize, (char*)temporalRight + j * recordSize) <= 0) {
            memcpy((char*)array + posicion * recordSize, (char*)temporalLeft + i * recordSize, recordSize);
            i++;
        } else {
            memcpy((char*)array + posicion * recordSize, (char*)temporalRight + j * recordSize, recordSize);
            j++;
        }
        posicion++; // Increment the position in the original array
    }

    // Copy the remaining elements from temporalLeft
    while (i < firstArray) {
        memcpy((char*)array + posicion * recordSize, (char*)temporalLeft + i * recordSize, recordSize);
        i++;
        posicion++;
    }

    // Copy the remaining elements from temporalRight
    while (j < secondArray) {
        memcpy((char*)array + posicion * recordSize, (char*)temporalRight + j * recordSize, recordSize);
        j++;
        posicion++;
    }

    // Free allocated memory
    free(temporalLeft); temporalLeft = NULL;
    free(temporalRight); temporalRight = NULL;
}


/* 
Performs Merge Sort on the given array.
Parameters: 
  - array: Pointer to the array to be sorted.
  - left: Starting index of the array.
  - right: Ending index of the array.
  - recordSize: Size of each record (element) in bytes.
  - compare: Comparison function to compare two records.
Usage:
  - This function uses the MergeSort algorithm to sort the array.
Variables:
  - middle: Index of the middle element to divide the array into two halves.
*/
void MergeSortArray(void *array, int left, int right, int recordSize, int (*compare)(void*, void*)) {
    if (left < right) {
        int middle = left + ((right - left) / 2); // Calculate the middle index

        // Recursively sort the first half
        MergeSortArray(array, left, middle, recordSize, compare);

        // Recursively sort the second half
        MergeSortArray(array, middle + 1, right, recordSize, compare);

        // Merge the two sorted halves
        MergeArray(array, left, right, middle, recordSize, compare);
    }
}


/*
void MergeFile(FILE *fp, int left, int right, int middle, int recordSize, int (*compare)(void*, void*)){
    // Calcular el tamaño de los sub-arrays
    int firstArray = middle - left + 1;
    int secondArray = right - middle;

    // Crear arrays temporales para almacenar los registros
    void *temporalLeft = calloc(firstArray, recordSize);
    void *temporalRight = calloc(secondArray, recordSize);

    // Mover el puntero al inicio del archivo
    fseek(fp, left * recordSize, SEEK_SET);

    // Leer registros para el primer sub-array (izquierda)
    fread(temporalLeft, recordSize, firstArray, fp);
   

    // Leer registros para el segundo sub-array (derecha)
    fseek(fp, (middle + 1) * recordSize, SEEK_SET);
    fread(temporalRight, recordSize, secondArray, fp);

    // Índices de los sub-arrays temporales
    int i = 0, j = 0;
    // Posición de escritura en el archivo
    int posicion = left;

    // Mover el puntero al inicio del archivo donde se escribirá
    fseek(fp, posicion * recordSize, SEEK_SET);

    // Fusionar los registros ordenados
    while (i < firstArray && j < secondArray) {
        fseek(fp, posicion * recordSize, SEEK_SET);
        if (compare((char*)temporalLeft + i * recordSize, (char*)temporalRight + j * recordSize) <= 0) {
            fseek(fp, posicion * recordSize, SEEK_SET);
            fwrite((char*)temporalLeft + i * recordSize, recordSize, 1, fp);
            i++;
        } else {
            fseek(fp, posicion * recordSize, SEEK_SET);
            fwrite((char*)temporalRight + j * recordSize, recordSize, 1, fp);
            j++;
        }
        posicion++;
    }

    // Escribir los registros restantes del primer sub-array
    while (i < firstArray) {
        fseek(fp, posicion * recordSize, SEEK_SET);
        fwrite((char*)temporalLeft + i * recordSize, recordSize, 1, fp);
        i++;
        posicion++;
    }

    // Escribir los registros restantes del segundo sub-array
    while (j < secondArray) {
        fseek(fp, posicion * recordSize, SEEK_SET);
        fwrite((char*)temporalRight + j * recordSize, recordSize, 1, fp);
        j++;
        posicion++;
    }
}

void MergeSortFile(FILE *fp, int left, int right, int recordSize, int (*compare)(void*, void*)){
    if(left<right){
        //variable middle que guarda el valor de la posición de la mitad del archivo
        int middle = left + (right - left) / 2;

        //Volvemos a llamar a la función pero desde la izquierda hasta la mitad
        MergeSortFile(fp, left, middle, recordSize, compare);
        //Volvemos a llamar a la función pero desde la mitad + 1  hasta la derecha
        MergeSortFile(fp, middle + 1, right, recordSize, compare);
        //Unimos las dos mitades
        MergeFile(fp, left, right, middle, recordSize, compare);
    }
}
*/

/* 
Merges two sorted sub-arrays from a file into a single sorted array using the Merge Sort algorithm.
Parameters: 
  - file: Pointer to the file to be merged.
  - left: Starting index of the left sub-array.
  - right: Ending index of the right sub-array.
  - medium: Index of the middle element separating the two sub-arrays.
  - recordSize: Size of each record (element) in bytes.
  - compare: Comparison function to compare two records.
Usage:
  - This function is used by the Merge Sort algorithm to merge two sorted sub-arrays from a file.
Variables:
  - firstArray: Size of the first sub-array.
  - secondArray: Size of the second sub-array.
  - leftFile: Temporary file to store the left sub-array.
  - rightFile: Temporary file to store the right sub-array.
  - buffer: Temporary buffer for reading and writing records.
  - i, j: Iterators for the left and right sub-arrays.
  - posicion: The position in the file to place the merged element.
*/
void MergeFile(FILE *file, int left, int right, int medium, int recordSize, int (*compare)(void*, void*)) {
    int firstArray = medium - left + 1; // Number of elements in the first sub-array
    int secondArray = right - medium;   // Number of elements in the second sub-array

    // Create temporary files for the left and right sub-arrays
    FILE *leftFile = fopen("tempLeft.bin", "wb+");
    FILE *rightFile = fopen("tempRight.bin", "wb+");

    void *buffer = malloc(recordSize); // Temporary buffer for reading and writing records

    // Copy the values of the first half into the left temporary file
    for (int i = 0; i < firstArray; i++) {
        fseek(file, (left + i) * recordSize, SEEK_SET);
        fread(buffer, recordSize, 1, file);
        fwrite(buffer, recordSize, 1, leftFile);
    }

    // Copy the values of the second half into the right temporary file
    for (int j = 0; j < secondArray; j++) {
        fseek(file, (medium + 1 + j) * recordSize, SEEK_SET);
        fread(buffer, recordSize, 1, file);
        fwrite(buffer, recordSize, 1, rightFile);
    }

    fseek(leftFile, 0, SEEK_SET);
    fseek(rightFile, 0, SEEK_SET);

    int i = 0, j = 0;
    int posicion = left;

    // Merge and sort the records from the temporary files back into the original file
    while (i < firstArray && j < secondArray) {
        fseek(leftFile, i * recordSize, SEEK_SET);
        fseek(rightFile, j * recordSize, SEEK_SET);

        void *leftRecord = malloc(recordSize);
        void *rightRecord = malloc(recordSize);

        fread(leftRecord, recordSize, 1, leftFile);
        fread(rightRecord, recordSize, 1, rightFile);

        if (compare(leftRecord, rightRecord) <= 0) {
            fseek(file, posicion * recordSize, SEEK_SET);
            fwrite(leftRecord, recordSize, 1, file);
            i++;
        } else {
            fseek(file, posicion * recordSize, SEEK_SET);
            fwrite(rightRecord, recordSize, 1, file);
            j++;
        }

        posicion++; // Move to the next position in the file
        free(leftRecord); free(rightRecord); // Free temporary memory
    }

    // Copy remaining elements from the left temporary file
    while (i < firstArray) {
        fseek(leftFile, i * recordSize, SEEK_SET);
        fread(buffer, recordSize, 1, leftFile);
        fseek(file, posicion * recordSize, SEEK_SET);
        fwrite(buffer, recordSize, 1, file);
        i++;
        posicion++;
    }

    // Copy remaining elements from the right temporary file
    while (j < secondArray) {
        fseek(rightFile, j * recordSize, SEEK_SET);
        fread(buffer, recordSize, 1, rightFile);
        fseek(file, posicion * recordSize, SEEK_SET);
        fwrite(buffer, recordSize, 1, file);
        j++;
        posicion++;
    }

    // Free allocated memory and close temporary files
    free(buffer);
    fclose(leftFile);
    fclose(rightFile);
    remove("tempLeft.bin");
    remove("tempRight.bin");
}

/* 
Performs Merge Sort on the given file.
Parameters: 
  - file: Pointer to the file to be sorted.
  - left: Starting index of the file.
  - right: Ending index of the file.
  - recordSize: Size of each record (element) in bytes.
  - compare: Comparison function to compare two records.
Usage:
  - This function uses the Merge Sort algorithm to sort the file.
Variables:
  - medium: The middle index of the file to divide it into two halves.
*/
void MergeSortFile(FILE *file, int left, int right, int recordSize, int (*compare)(void*, void*)) {
    if (left < right) {
        int medium = left + ((right - left) / 2); // Calculate the middle index

        // Recursively sort the first half of the file
        MergeSortFile(file, left, medium, recordSize, compare);

        // Recursively sort the second half of the file
        MergeSortFile(file, medium + 1, right, recordSize, compare);

        // Merge the two sorted halves
        MergeFile(file, left, right, medium, recordSize, compare);
    }
}
