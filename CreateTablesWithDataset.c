#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Tables.h"

/*
    Estructuras por archivo

//Sales Table
typedef struct{
    long int OrderNumber;
    unsigned char LineItems;
    struct{
        unsigned char DD;
        unsigned char MM;
        unsigned short int AAAA;
    } OrderDate;
    struct{
        unsigned char DD;
        unsigned char MM;
        unsigned short int AAAA;
    } DeliveryDate;
    unsigned int CustomerKey;
    unsigned short int StoreKey;
    unsigned short int ProductKey;
    unsigned short int Quantity;
    char CurrencyCode[3];
} Sales;

//Customers Table
typedef struct{
    unsigned int CustomerKey;
    char Gender[8];
    char Name[40];
    char City[40];
    char StateCode[30];
    char State[30];
    unsigned int ZipCode;
    char Country[20];
    char Continent[20];
    struct{
        unsigned char DD;
        unsigned char MM;
        unsigned short int AAAA;
    } Birthday;
} Customers;

//Products Table
typedef struct{
    unsigned short int ProductKey;
    char ProductName[100];
    char Brand[30];
    char Color[15];
    float UnitCostUSD;
    float UnitPriceUSD;
    char SubcategoryKey[5];
    char Subcategory[50];
    char CategoryKey[3];
    char Category[20];
} Products;

//Stores Table
typedef struct{
    unsigned short int StoreKey;
    char Country[35];
    char State[35];
    unsigned short int SquareMeters;
    struct{
        unsigned char DD;
        unsigned char MM;
        unsigned short int AAAA;
    } OpenDate;
} Stores;

//ExchangeRates Table
typedef struct{
    char Date[11];
    char Currency[3];
    float Exchange;
} ExchangeRates;
*/

int TellNumRecords(char fileName[], int recordSize) {
    FILE *fp = fopen(fileName, "rb"); // Abrir en modo binario
    if (fp == NULL) {
        return -1; // Retornar -1 en caso de error al abrir el archivo
    }

    fseek(fp, 0, SEEK_END); // Mover el puntero al final del archivo
    long fileSize = ftell(fp); // Obtener el tamaño del archivo en bytes
    fclose(fp);

    // Calcular el número de registros
    if (recordSize == 0) {
        return -2; // Evitar división por cero
    }
    int numRecords = fileSize / recordSize;

    return numRecords;
}

void CreateSalesTable(char originFileName[]){
    FILE *fpOrigin = fopen(originFileName, "r");
    FILE *fpSale = fopen("SalesTable", "wb");

    if(fpOrigin == NULL){
        printf("Error al abrir el archivo de origen.");
        return;
    }
    if(fpSale == NULL){
        printf("Error al abrir el archivo de destino.");
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
        sscanf(token, "%u/%u/%u", &day, &month, &year);
        recordSales.OrderDate.DD = (unsigned char) day;
        recordSales.OrderDate.MM = (unsigned char) month;
        recordSales.OrderDate.AAAA = (unsigned short int) year;
        
        token = strtok(NULL, ",");
        if (token[0] == ' '){
            recordSales.DeliveryDate.DD = 0;
            recordSales.DeliveryDate.MM = 0;
            recordSales.DeliveryDate.AAAA = 0;
        } else {
            sscanf(token, "%u/%u/%u", &day, &month, &year);
            recordSales.DeliveryDate.DD = (unsigned char) day;
            recordSales.DeliveryDate.MM = (unsigned char) month;
            recordSales.DeliveryDate.AAAA = (unsigned short int) year;
        }
        
        token = strtok(NULL, ",");
        recordSales.CustomerKey = (unsigned int) atoi(token);
        
        token = strtok(NULL, ",");
        recordSales.StoreKey = (unsigned short int) atoi(token);
        
        token = strtok(NULL, ",");
        recordSales.ProductKey = (unsigned short int) atoi(token);
        
        token = strtok(NULL, ",");
        recordSales.Quantity = (unsigned short int) atoi(token);
        
        token = strtok(NULL, ",");
        strcpy(recordSales.CurrencyCode, token);
        
        fwrite(&recordSales, sizeof(recordSales) ,1 ,fpSale);
        /*
        printf("Record: %i\n\n",numberRecord);
        printf("OrderNumber :%ld\n", recordSales.OrderNumber);
        printf("LineItem    :%u\n", recordSales.LineItems);
        printf("OrderDate   :%u/%u/%hu\n", recordSales.OrderDate.DD, recordSales.OrderDate.MM, recordSales.OrderDate.AAAA);
        printf("DeliveryDate:%u/%u/%hu\n", recordSales.DeliveryDate.DD, recordSales.DeliveryDate.MM, recordSales.DeliveryDate.AAAA);
        printf("CustomerKey :%u\n", recordSales.CustomerKey);
        printf("StoreKey    :%hu\n", recordSales.StoreKey);
        printf("ProductKey  :%hu\n", recordSales.ProductKey);
        printf("Quantity    :%hu\n", recordSales.Quantity);
        printf("CurrencyCode:%s\n\n\n", recordSales.CurrencyCode);
        */  
        numberRecord++;
    }
    fclose(fpOrigin); fclose(fpSale);
    return;
}

void CreateCustomersTable(char originFileName[]){
    FILE *fpOrigin = fopen(originFileName, "r");
    FILE *fpCustomer = fopen("CustomersTable", "wb");

    if(fpOrigin == NULL){
        printf("Error al abrir el archivo de origen.");
        return;
    }
    if(fpCustomer == NULL){
        printf("Error al abrir el archivo de destino.");
        return;
    }
    Customers recordCustomer;
    char line[400] = "";
    int numberRecord = 0;

    while (fgets(line, sizeof(line), fpOrigin)){
        if(numberRecord != 0){
            char *token = strtok(line, ";");
            recordCustomer.CustomerKey = (unsigned int) atoi(token);

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
                recordCustomer.ZipCode = (unsigned int) atoi(token);
                token = strtok(NULL, ";");
            } else {
                recordCustomer.ZipCode = 0;
            }
            strcpy(recordCustomer.Country, token);

            token = strtok(NULL, ";");
            strcpy(recordCustomer.Continent, token);

            token = strtok(NULL, ";");
            unsigned int day = 0, month = 0, year = 0;
            sscanf(token, "%u/%u/%u", &day, &month, &year);
            recordCustomer.Birthday.DD = (unsigned char) day;
            recordCustomer.Birthday.MM = (unsigned char) month;
            recordCustomer.Birthday.AAAA = (unsigned short int) year;

            fwrite(&recordCustomer, sizeof(recordCustomer), 1, fpCustomer);
            /*
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
            printf("Birthday   :%u/%u/%hu\n\n", recordCustomer.Birthday.DD, recordCustomer.Birthday.MM, recordCustomer.Birthday.AAAA);
            */
        }
        numberRecord++;
    }
    fclose(fpOrigin); fclose(fpCustomer);
    return;
}

void CreateProductsTable(char originFileName[]){
    FILE *fpOrigin = fopen(originFileName, "r");
    FILE *fpProduct = fopen("ProductsTable", "wb");

    if(fpOrigin == NULL){
        printf("Error al abrir el archivo de origen.");
        return;
    }
    if(fpProduct == NULL){
        printf("Error al abrir el archivo de destino.");
        return;
    }
    Products recordProduct;
    char line[300] = "";
    int numberRecord = 0;

    while (fgets(line, sizeof(line), fpOrigin)){
        if(numberRecord != 0){
            char *token = strtok(line, ","), temp[] = "";
            recordProduct.ProductKey = (unsigned short int) atoi(token);

            token =strtok(NULL, ",");
            if(token[0] == '"'){

            } else {
                strcpy(recordProduct.ProductName, token);
            }

            token =strtok(NULL, ",");
            strcpy(recordProduct.Brand, token);

            token =strtok(NULL, ",");
            strcpy(recordProduct.Color, token);

            token = strtok(NULL, ",");
            printf("%i\t%s\n", numberRecord, token);
            recordProduct.UnitCostUSD = atof(strncpy(temp, token + 1, strlen(token)));

            token = strtok(NULL, ",");
            recordProduct.UnitPriceUSD = atof(strncpy(temp, token + 1, strlen(token)));

            token =strtok(NULL, ",");
            strcpy(recordProduct.SubcategoryKey, token);

            token =strtok(NULL, ",");
            strcpy(recordProduct.Subcategory, token);

            token =strtok(NULL, ",");
            strcpy(recordProduct.CategoryKey, token);

            token =strtok(NULL, ",");
            strcpy(recordProduct.Category, token);

            fwrite(&recordProduct, sizeof(recordProduct), 1, fpProduct);
            /*
            printf("Record : %i\n\n", numberRecord);
            printf("ProductKey    :%hu\n", recordProduct.ProductKey);
            printf("ProductName   :%s\n", recordProduct.ProductName);
            printf("Brand         :%s\n", recordProduct.Brand);
            printf("Color         :%s\n", recordProduct.Color);
            printf("UnitCostUSD   :%.2f\n", recordProduct.UnitCostUSD);
            printf("UnitPriceUSD  :%.2f\n", recordProduct.UnitPriceUSD);
            printf("SubCategoryKey:%s\n", recordProduct.SubcategoryKey);
            printf("SubCategory   :%s\n", recordProduct.Subcategory);
            printf("CategoryKey   :%s\n", recordProduct.CategoryKey);
            printf("Category      :%s\n\n", recordProduct.Category);
            */
        }
        numberRecord++;
    }
    fclose(fpOrigin); fclose(fpProduct);
    return;
}

void CreateStoresTable(char originFileName[]){
    FILE *fpOrigin = fopen(originFileName, "r");
    FILE *fpStore = fopen("StoresTable", "wb");

    if(fpOrigin == NULL){
        printf("Error al abrir el archivo de origen.");
        return;
    }
    if(fpStore == NULL){
        printf("Error al abrir el archivo de destino.");
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
            sscanf(token, "%u/%u/%u", &day, &month, &year);
            recordStore.OpenDate.DD = (unsigned char) day;
            recordStore.OpenDate.MM = (unsigned char) month;
            recordStore.OpenDate.AAAA = (unsigned short int) year;

            fwrite(&recordStore, sizeof(recordStore), 1, fpStore);
            /*
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

void CreateExchangeRatesTable(char originFileName[]){
    FILE *fpOrigin = fopen(originFileName, "r");
    FILE *fpExchange = fopen("ExchangeRatesTable", "wb");

    if(fpOrigin == NULL){
        printf("Error al abrir el archivo de origen.");
        return;
    }
    if(fpExchange == NULL){
        printf("Error al abrir el archivo de destino.");
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

void CreateDatasetTables(){
    CreateExchangeRatesTable("Exchange_Rates.csv");
    CreateSalesTable("Sales.csv");
    CreateCustomersTable("Customers.csv");
    CreateStoresTable("Stores.csv");
    CreateProductsTable("Products.csv");
}