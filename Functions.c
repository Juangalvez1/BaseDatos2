#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>
#include "Functions.h"

void SetColor(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}


int TellNumRecords(char fileName[], int recordSize){
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
        recordSales.CustomerKey = (unsigned long int) atoi(token);
        
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
    system("cls");
    SetColor(9);
    printf("\nTables Created Succesfully\n");
}


int BinarySearch(FILE *fp, unsigned long int valueToSearch, int file){
    unsigned int start = 0, middle = 0, end = 0, sizeOfRecord = 0;
    unsigned long int key = 0;

    if(file == 1){
		sizeOfRecord = sizeof(Products);
	} else if(file == 2){
		sizeOfRecord = sizeof(Customers);	
	} else if(file == 3){
		sizeOfRecord = sizeof(Sales);
	} else if(file == 4){
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
			key = (unsigned long int) recordProduct.ProductKey;
            //printf("productsKey que agarro: %u\t", recordProduct.ProductKey);
		} else if (file == 2){
			Customers recordCustomer;
			fread(&recordCustomer, sizeOfRecord, 1, fp);
			key = (unsigned long int) recordCustomer.CustomerKey;
		} else if (file == 3){
			Sales recordSale;
			fread(&recordSale, sizeOfRecord, 1, fp);
			key = (unsigned long int) recordSale.ProductKey;
		}  else if(file == 4){
            Sales recordSale;
			fread(&recordSale, sizeOfRecord, 1, fp);
			key = (unsigned long int) recordSale.CustomerKey;
        } else {
		    return -1;
	    }
        //printf("Start: %u\tEnd: %u\tMiddle: %u\tValueToSearch: %lu\tKey: %lu\n", start, end, middle, valueToSearch, key);
        
        if(key == valueToSearch){
            //printf("\nPOR FIN\n");
            return middle;
        }else if(key < valueToSearch){
			start = middle + 1;
        }else{
            end = middle - 1;
        }
    }
	return -1;
}

int BinarySearchExchangeDate(FILE *fp, Sales recordSale){
    unsigned int start = 0, middle = 0, end = 0, sizeOfRecord = sizeof(ExchangeRates);
    ExchangeRates record;
    fseek(fp, 0 ,SEEK_END);
    end = ( ftell(fp) / sizeOfRecord ) - 1;
    while (start <= end) {
        middle = start + ((end - start) / 2);
        fseek(fp, middle * sizeOfRecord, SEEK_SET);
        fread(&record, sizeOfRecord, 1, fp);

        char key[11] = "", keyAux[11] = "";
        strcpy(key, record.Date);

        printf("Start: %u, Middle: %u, End: %u, Clave: '%s', Buscando: %d/%d/%d ", start, middle, end, key, recordSale.OrderDate.MM, recordSale.OrderDate.DD, recordSale.OrderDate.AAAA);
        printf(",KeyAux: %s\n", keyAux);
        
        int currentMonth = 0, currentDay = 0, currentYear = 0;
        int month = 0, day = 0, year = 0;
        month = recordSale.OrderDate.MM;
        day = recordSale.OrderDate.DD;
        year = recordSale.OrderDate.AAAA;

        //Leer el formato flexible (con o sin ceros iniciales)
        sscanf(key, "%d/%d/%d", &currentMonth, &currentDay, &currentYear);
        if(currentYear == year){
            if(currentMonth == month){
                if(currentDay == day){
                    return middle;
                } else if(currentDay > day){
                    end = middle - 1;
                } else {
                    start = middle + 1;
                }
            } else if(currentMonth > month){
                end = middle - 1;
            } else {
                start = middle + 1;
            }
        } else if(currentYear > year){
            end = middle - 1;
        } else {
            start = middle + 1;
        }


        // Crear la nueva fecha en formato AAAA/MM/DD
        //sprintf(keyAux, "%04d/%02d/%02d", year, month, day);


        /*
        int comparation1 = strcmp(key, DateToSearch);
        if (comparation1 == 0) {
            return middle;
        } else if (comparation1 < 0) {
            start = middle + 1;
        } else {
            end = middle - 1;
        }
        */
    }
	return -1;
}


int CompareProductsByProductName(void *a, void *b){
    Products *prodA = (Products *)a;
    Products *prodB = (Products *)b;
    return strcmp(prodA->ProductName, prodB->ProductName);
}

int CompareProductsByProductKey(void *a, void *b){
    Products *prodA = (Products *)a;
    Products *prodB = (Products *)b;
    return prodA->ProductKey - prodB->ProductKey;
}

int CompareCustomersByCustomerKey(void *a, void *b){
    Customers *custA = (Customers *)a;
    Customers *custB = (Customers *)b;
    return custA->CustomerKey - custB->CustomerKey;
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

int CompareCustomersByName(void *a, void *b){
    Customers *custA = (Customers *)a;
    Customers *custB = (Customers *)b;
    return strcmp(custA->Name, custB->Name);
}

int CompareSalesByProductKey(void *a, void *b){
    Sales *saleA = (Sales *)a;
    Sales *saleB = (Sales *)b;
    return (int)(saleA->ProductKey - saleB->ProductKey);
}

int CompareSalesByCustomerKey(void *a, void *b){
    Sales *saleA = (Sales *)a;
    Sales *saleB = (Sales *)b;
    return saleA->CustomerKey - saleB->CustomerKey;
}

int CompareSalesByOrderNumber(void *a, void *b){
    Sales *saleA = (Sales *)a;
    Sales *saleB = (Sales *)b;
    long int comparation = saleA->OrderNumber - saleB->OrderNumber;
    
    if(comparation != 0){
        return (int)comparation;
    } else{
        return (int)(saleA->ProductKey - saleB->ProductKey);
    }
    
}


void MergeArray(void *array, int left, int right, int middle, int recordSize, int (*compare)(void*, void*)){
    int firstArray = middle - left + 1;
    int secondArray = right - middle;
    void *temporalLeft = calloc(firstArray,  recordSize);
    void *temporalRight = calloc(secondArray,  recordSize);

    //Copy the values of the first half
    for (int i = 0; i < firstArray; i++) {
        memcpy((char*)temporalLeft + i * recordSize, (char*)array + (left + i) * recordSize, recordSize);
    }

    //Copy the values of the second half
    for (int j = 0; j < secondArray; j++) {
        memcpy((char*)temporalRight + j * recordSize, (char*)array + (middle + 1 + j) * recordSize, recordSize);
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

    free(temporalLeft); temporalLeft = NULL;
    free(temporalRight); temporalRight = NULL;
}

void MergeSortArray(void *array, int left, int right, int recordSize, int (*compare)(void*, void*)){
    if (left < right) {
        int middle = left + ((right - left) / 2);

        //Recursive call for the first half
        MergeSortArray(array, left, middle, recordSize, compare);

        //Recursive call for the second half
        MergeSortArray(array, middle + 1, right, recordSize, compare);

        //Order and combine the two halfs
        MergeArray(array, left, right, middle, recordSize, compare);
    }
}


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

/*
void MergeFile(FILE *file, int left, int right, int medium, int recordSize, int (*compare)(void*, void*)) {
    int firstArray = medium - left + 1;
    int secondArray = right - medium;

    // Crear archivos temporales
    FILE *leftFile = fopen("tempLeft.bin", "wb+");
    FILE *rightFile = fopen("tempRight.bin", "wb+");

    void *buffer = malloc(recordSize);

    // Copiar los valores de la primera mitad al archivo temporal izquierdo
    for (int i = 0; i < firstArray; i++) {
        fseek(file, (left + i) * recordSize, SEEK_SET);
        fread(buffer, recordSize, 1, file);
        fwrite(buffer, recordSize, 1, leftFile);
    }

    // Copiar los valores de la segunda mitad al archivo temporal derecho
    for (int j = 0; j < secondArray; j++) {
        fseek(file, (medium + 1 + j) * recordSize, SEEK_SET);
        fread(buffer, recordSize, 1, file);
        fwrite(buffer, recordSize, 1, rightFile);
    }

    fseek(leftFile, 0, SEEK_SET);
    fseek(rightFile, 0, SEEK_SET);

    int i = 0, j = 0;
    int posicion = left;

    // Mezclar y ordenar los registros de los archivos temporales de vuelta al archivo original
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

        posicion++;
        free(leftRecord);
        free(rightRecord);
    }

    // Copiar los elementos restantes del archivo temporal izquierdo
    while (i < firstArray) {
        fseek(leftFile, i * recordSize, SEEK_SET);
        fread(buffer, recordSize, 1, leftFile);
        fseek(file, posicion * recordSize, SEEK_SET);
        fwrite(buffer, recordSize, 1, file);
        i++;
        posicion++;
    }

    // Copiar los elementos restantes del archivo temporal derecho
    while (j < secondArray) {
        fseek(rightFile, j * recordSize, SEEK_SET);
        fread(buffer, recordSize, 1, rightFile);
        fseek(file, posicion * recordSize, SEEK_SET);
        fwrite(buffer, recordSize, 1, file);
        j++;
        posicion++;
    }

    // Liberar memoria y cerrar archivos temporales
    free(buffer);
    fclose(leftFile);
    fclose(rightFile);
    remove("tempLeft.bin");
    remove("tempRight.bin");
}

void MergeSortFile(FILE *file, int left, int right, int recordSize, int (*compare)(void*, void*)) {
    if (left < right) {
        int medium = left + ((right - left) / 2);

        // Llamada recursiva para la primera mitad
        MergeSortFile(file, left, medium, recordSize, compare);

        // Llamada recursiva para la segunda mitad
        MergeSortFile(file, medium + 1, right, recordSize, compare);

        // Combinar las dos mitades
        MergeFile(file, left, right, medium, recordSize, compare);
    }
}
*/