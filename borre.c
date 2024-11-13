#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "Functions.c"

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

void SwapCustomers(void *a, void *b) {
    Customers *custA = (Customers *)a;
    Customers *custB = (Customers *)b;

    Customers temp = *custA;
    *custA = *custB;
    *custB = temp;
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
    
    Customers *regs = (Customers *)calloc(sizeCustomers, sizeof(Customers));
    // Leer los registros del archivo en el arreglo
    for (int i = 0; i < sizeCustomers; i++) {
        printf("Guarda en regs %i\n", i);
        fseek(fpCustomers, sizeof(Customers) * i, SEEK_SET);
        fread(&regs[i], sizeof(Customers), 1, fpCustomers);
    }


    /*//Ordenar el arreglo en memoria
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
    //MergeSort(regs, 0, sizeCustomers - 1, sizeof(Customers), CompareByCustomerKey);
    
    BubbleSort(regs, sizeCustomers, sizeof(Customers), CompareByCustomerKey, SwapCustomers);

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