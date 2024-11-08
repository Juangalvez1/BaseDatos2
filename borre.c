#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/*
    Estructuras por archivo
*/

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
        return -1; // Evitar división por cero
    }
    int numRecords = fileSize / recordSize;

    return numRecords;
}

int main(){
    char str[] = "hola", comilla [1] = "";
    comilla[0] = 34;
    strcat(str, comilla);
    printf("%s\n", str);
}