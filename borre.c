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

int CompareByCustomerKey(void *a, void *b) {
    Customers *custA = (Customers *)a;
    Customers *custB = (Customers *)b;
    return custA->CustomerKey - custB->CustomerKey;
}

int CompareByProductKey(void *a, void *b) {
    Products *prodA = (Products *)a;
    Products *prodB = (Products *)b;
    return prodA->ProductKey - prodB->ProductKey;
}

int CompareBySalesProductKey(void *a, void *b){
    Sales *saleA = (Sales *)a;
    Sales *saleB = (Sales *)b;
    return saleA->ProductKey - saleB->ProductKey;
}

int CompareByCustomerLocation(void *a, void*b){
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
    void *temporalLeft = malloc(firstArray * recordSize);
    void *temporalRight = malloc(secondArray * recordSize);

    // Copiar los valores de la primera mitad
    for (int i = 0; i < firstArray; i++) {
        memcpy((char*)temporalLeft + i * recordSize, (char*)array + (left + i) * recordSize, recordSize);
    }

    // Copiar los valores de la segunda mitad
    for (int j = 0; j < secondArray; j++) {
        memcpy((char*)temporalRight + j * recordSize, (char*)array + (medium + 1 + j) * recordSize, recordSize);
    }

    int i = 0, j = 0;
    int posicion = left;

    // Combinar los dos subarreglos de manera ordenada
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

// Función MergeSort que usa punteros void y funciones de comparación
void MergeSort(void *array, int left, int right, int recordSize, int (*compare)(void*, void*)) {
    if (left < right) {
        int medium = left + ((right - left) / 2);

        // Llamada recursiva para la primera mitad
        MergeSort(array, left, medium, recordSize, compare);

        // Llamada recursiva para la segunda mitad
        MergeSort(array, medium + 1, right, recordSize, compare);

        // Combina las dos mitades ordenadas
        Merge(array, left, right, medium, recordSize, compare);

        printf("Guarda en Array %i\n");
    }
}

/*
Funcion PrintArray que imprime todos los valores del array
- variale size que guarda el tamaño del array
- variable array que contiene los datos del array a imprimir
*/
void PrintArray(int size, int array[]){
    for(int i = 0; i<size; i++){
        printf("%i ", array[i]);
    }
    printf("\n");
}
/*
int main(){
    //variable array que tendrá el array a ajustar
    int array[] = {3453, 4468, 4267, 132, 1634, 3748, 3263, 2436, 3456, 4566, 4567, 4566};
    //variable size que contiene el tamaño del array
    int size = sizeof(array)/sizeof(array[0]);
    printf("Array desordenado: \n");
    PrintArray(size, array);
    printf("Array ordenado: \n");
    MergeSort(0, size-1, array);
    PrintArray(size, array);
    return 0;
}
*/
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
    
    Customers *regs = (Customers *)calloc(sizeCustomers, sizeof(Customers));
    // Leer los registros del archivo en el arreglo
    for (int i = 0; i < sizeCustomers; i++) {
        printf("Guarda en regs %i\n", i);
        fseek(fpCustomers, sizeof(Customers) * i, SEEK_SET);
        fread(&regs[i], sizeof(Customers), 1, fpCustomers);
    }

    MergeSort(regs, 0, sizeCustomers - 1, sizeof(Customers), CompareByCustomerKey);

    /*Ordenar el arreglo en memoria
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
    */
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