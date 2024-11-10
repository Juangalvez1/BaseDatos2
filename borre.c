#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "CreateTablesWithDataset.c"

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
*/

void PrintExecutionTime(double time){
    int minutes = 0, seconds = 0;
    char totalTime[6] = "";

    minutes = (int) time / 60;
    seconds = (int) time % 60;

    sprintf(totalTime, "%02d'%02d''", minutes, seconds);
    printf("\n\n%s\n\n", totalTime);
}

int main() {
    // Crear tabla de clientes de ejemplo

    int start = 0, finish = 0;
    double totalSeconds = 0;
    start = clock();


    CreateCustomersTable("Customers.csv");

    // Determinar el tamaño del arreglo basado en el archivo
    int sizeCustomers = TellNumRecords("CustomersTable", sizeof(Customers));
    printf("LLega - Registros a leer: %d\n", sizeCustomers);

    if (sizeCustomers == 0) {
        printf("No hay registros en el archivo.\n");
        return 1;
    }

    FILE *fpCustomers = fopen("CustomersTable", "rb+");
    if (fpCustomers == NULL) {
        printf("Error al abrir el archivo para lectura/escritura.\n");
        return 1;
    }
    
    Customers *regs = (Customers *)malloc(sizeCustomers * sizeof(Customers));
    // Leer los registros del archivo en el arreglo
    for (int i = 0; i < sizeCustomers; i++) {
        printf("Guarda en regs %i\n", i);
        fseek(fpCustomers, sizeof(Customers) * i, SEEK_SET);
        fread(&regs[i], sizeof(Customers), 1, fpCustomers);
    }

    // Ordenar el arreglo en memoria
    for (int step = 0; step < sizeCustomers - 1; step++) {
        printf("2. Llega %i\n", step + 1);
        for (int i = 0; i < sizeCustomers - step - 1; i++) {
            if (regs[i].CustomerKey > regs[i + 1].CustomerKey) {
                // Intercambiar registros
                Customers temp = regs[i];
                regs[i] = regs[i + 1];
                regs[i + 1] = temp;
            }
        }
    }

    // Escribir los registros ordenados de vuelta al archivo
    
    for (int i = 0; i < sizeCustomers; i++) {
        printf("Guarda en archivo %i\n", i + 1);
        fseek(fpCustomers, sizeof(Customers) * i, SEEK_SET);
        fwrite(&regs[i], sizeof(Customers), 1, fpCustomers);
    }

    finish = clock();

    totalSeconds = ((double)(finish - start)) / CLOCKS_PER_SEC;

    PrintExecutionTime(totalSeconds);

    free(regs);
    fclose(fpCustomers);
    return 0;
}