#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "Functions.h"


int TellNumRecords(char fileName[], int recordSize) {
    FILE *fp = fopen(fileName, "rb"); // Abrir en modo binario
    if (fp == NULL) {
        return 0; // Retornar -1 en caso de error al abrir el archivo
    }

    fseek(fp, 0, SEEK_END); // Mover el puntero al final del archivo
    long fileSize = ftell(fp); // Obtener el tamaño del archivo en bytes
    fclose(fp);

    // Calcular el número de registros
    if (recordSize == 0) {
        return 0; // Evitar división por cero
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
            sscanf(token, "%u/%u/%u", &month, &day, &year);
            recordCustomer.Birthday.MM = (unsigned char) month;
            recordCustomer.Birthday.DD = (unsigned char) day;
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
            printf("Birthday   :%u/%u/%hu\n\n", recordCustomer.Birthday.MM, recordCustomer.Birthday.DD, recordCustomer.Birthday.AAAA);
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
            sscanf(token, "%u/%u/%u", &month, &day, &year);
            recordStore.OpenDate.MM = (unsigned char) month;
            recordStore.OpenDate.DD = (unsigned char) day;
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

void CreateTablesWithDataset(){
    CreateExchangeRatesTable("Exchange_Rates.csv");
    CreateSalesTable("Sales.csv");
    CreateCustomersTable("Customers.csv");
    CreateStoresTable("Stores.csv");
    CreateProductsTable("Products.csv");
}


int BinarySearch(FILE *fp, unsigned int valueToSearch, int file){
    unsigned int start = 0, middle = 0, end = 0, sizeOfRecord = 0, key = 0;

    if(file == 1){
		sizeOfRecord = sizeof(Products);
	} else if(file == 2){
		sizeOfRecord = sizeof(Customers);	
	} else if(file == 3){
		sizeOfRecord = sizeof(Sales);
	} else {
		return -1;
	}

    fseek(fp, 0 ,SEEK_END);
    end = ( ftell(fp) / sizeOfRecord ) - 1;
    while(start <= end){
        middle = start + ((end - start) / 2);
        fseek(fp, middle * sizeOfRecord, SEEK_SET);

		if (file == 1){
			Products recordProduct;
			fread(&recordProduct, sizeOfRecord, 1, fp);
			key = (unsigned int) recordProduct.ProductKey;
		} else if (file == 2){
			Customers recordCustomer;
			fread(&recordCustomer, sizeOfRecord, 1, fp);
			key = (unsigned int) recordCustomer.CustomerKey;
		} else if (file == 3){
			Sales recordSale;
			fread(&recordSale, sizeOfRecord, 1, fp);
			key = (unsigned int) recordSale.ProductKey;
		}
        
        if(key == valueToSearch){
            return middle;
        }else if(key < valueToSearch){
			start = middle + 1;
        }else{
            end = middle - 1;
        }
    }
	return -1;
}



int CompareCustomersByCustomerKey(void *a, void *b) {
    Customers *custA = (Customers *)a;
    Customers *custB = (Customers *)b;
    return custA->CustomerKey - custB->CustomerKey;
}

int CompareProductsByProductName(void *a, void *b) {
    Products *prodA = (Products *)a;
    Products *prodB = (Products *)b;
    return strcmp(prodA->ProductName, prodB->ProductName);
}

int CompareSalesByProductKey(void *a, void *b){
    Sales *saleA = (Sales *)a;
    Sales *saleB = (Sales *)b;
    return saleA->ProductKey - saleB->ProductKey;
}

int CompareCustomersByCustomerLocation(void *a, void*b){
    int result = 0;
    Customers *custA = (Customers *)a;
    Customers *custB = (Customers *)b;

    result = strcmp(custA->Continent, custB->Continent);
    if(result != 0){
        return result;
    }

    result = strcmp(custA->Country, custB->Country);
    if(result != 0){
        return result;
    }

    result = strcmp(custA->State, custB->State);
    if(result != 0){
        return result;
    }

    result = strcmp(custA->City, custB->City);
    return result;

}


void Merge(void *array, int left, int right, int medium, int recordSize, int (*compare)(void*, void*)) {
    int firstArray = medium - left + 1;
    int secondArray = right - medium;
    void *temporalLeft = calloc(firstArray,  recordSize);
    void *temporalRight = calloc(secondArray,  recordSize);

    //Copy the values of the first half
    for (int i = 0; i < firstArray; i++) {
        memcpy((char*)temporalLeft + i * recordSize, (char*)array + (left + i) * recordSize, recordSize);
    }

    //Copy the values of the second half
    for (int j = 0; j < secondArray; j++) {
        memcpy((char*)temporalRight + j * recordSize, (char*)array + (medium + 1 + j) * recordSize, recordSize);
    }

    int i = 0, j = 0;
    int posicion = left;

    //Order and combine the two temporal arrays
    while (i < firstArray && j < secondArray) {
        if (compare((char*)temporalLeft + i * recordSize, (char*)temporalRight + j * recordSize) <= 0) {
            memcpy((char*)array + posicion * recordSize, (char*)temporalLeft + i * recordSize, recordSize);
            i++;
        } else {
            memcpy((char*)array + posicion * recordSize, (char*)temporalRight + j * recordSize, recordSize);
            j++;
        }
        posicion++;
    }

    // Copiar los elementos restantes de temporalLeft
    while (i < firstArray) {
        memcpy((char*)array + posicion * recordSize, (char*)temporalLeft + i * recordSize, recordSize);
        i++;
        posicion++;
    }

    // Copiar los elementos restantes de temporalRight
    while (j < secondArray) {
        memcpy((char*)array + posicion * recordSize, (char*)temporalRight + j * recordSize, recordSize);
        j++;
        posicion++;
    }

    free(temporalLeft);
    free(temporalRight);
}

void MergeSort(void *array, int left, int right, int recordSize, int (*compare)(void*, void*)) {
    if (left < right) {
        int medium = left + ((right - left) / 2);

        //Recursive call for the first half
        MergeSort(array, left, medium, recordSize, compare);

        //Recursive call for the second half
        MergeSort(array, medium + 1, right, recordSize, compare);

        //Order and combine the two halfs
        Merge(array, left, right, medium, recordSize, compare);

    }
}
