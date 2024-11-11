/*
- Fecha de publicación: 
- Hora de publicación: 
- Versión de su código: 1.0
- Autor. Ing(c) Juan José Gálvez López
- Nombre del lenguaje utilizado: C
- Versión del lenguaje utilizado: C11
- Versión del compilador utilizado: MinGW.org GCC-6.3.0-1
- Versión del S.O sobre el que compilo y probo su código: Windows 11 Home Single Language, 23H2
- Presentado a: Doctor Ricardo Moreno Laverde 
- Universidad Tecnológica de Pereira 
- Programa de Ingeniería de Sistemas y Computación 
- Asignatura IS284 Programación II 
- Un descriptivo de que hace el programa:
    
- Salvedades:
  Para pruebas hechas en otros sistemas operativos y/o otras versiones del lenguaje, no se garantizan resultados
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "Functions.c"
#include "ExecuteOption2.c"

void ShowDateTime() {
    // Obtener el tiempo actual
    time_t current_time = time(NULL);
    struct tm *local_time = localtime(&current_time);
    char formattedTime[50] = "";

    // Formatear la fecha y hora
    strftime(formattedTime, 50, "Valid to %Y-%b-%d at %H:%M hours", local_time);
    printf("%s\n", formattedTime);

}

void PrintExecutionTime(double time){
    int minutes = 0, seconds = 0;
    char totalTime[6] = "";

    minutes = (int) time / 60;
    seconds = (int) time % 60;

    sprintf(totalTime, "%02d'%02d''", minutes, seconds);
    printf("%s", totalTime);
}

void PrintMenu() {
    printf("\nCompany Global Electronics Retailer\n");
    printf("Options menu\n");
    printf("0. Exit Program\n");
    printf("1. Construction of the Database with the dataset tables\n");
    printf("2. List of %cWhat types of products does the company sell, and where are costumers located?\n", 168);
    printf("   2.1 Utility bubbleSort\n");
    printf("   2.2 Utility bubbleSort\n");
    printf("3. List of %cAre there any seasonal patterns or trends for order volume or revenue?\n", 168);
    printf("   3.1 Utility bubbleSort\n");
    printf("   3.2 Utility bubbleSort\n");
    printf("4. List of %cHow long is the average delivery time in days? Has that changed over time?\n", 168);
    printf("   4.1 Utility bubbleSort\n");
    printf("   4.2 Utility bubbleSort\n");
    printf("5. List of sales order by 'Costumer Name' + 'Order Date' + 'ProductKey';\n");
    printf("   5.1 Utility bubbleSort\n");
    printf("   5.2 Utility bubbleSort\n");
    printf("What is your option: ");
}

void Option2(int sortType){
    int start = 0, finish = 0;
    double totalSeconds = 0.;
    start = clock();
    printf("\n---------------------------------------------------------------------------------------");
    printf("\nCompany Global Electronics Retailer\n");
    ShowDateTime();
    printf("Products list ordered by ProductName  + Continent + Country + State + City\n");
    
    if(sortType == 1){
        BubbleSortOption2();
    } else if (sortType == 2){
        MergeSortOption2();
    }
    FILE *fp = fopen("TemporalFileOption2", "w");
    fclose(fp);

    finish = clock();

    totalSeconds = ((double)(finish - start)) / CLOCKS_PER_SEC;
    
    printf("\n---------------------------------------------------------------------------------------");
    printf("\nTime used to produce this listing: "); 
    PrintExecutionTime(totalSeconds);
    printf("\n********************************LAST LINE OF THE REPORT********************************\n");
}

int main() {
    float option = 0.;
    printf("Welcome!\n");

    do {
        PrintMenu();
        scanf("%f", &option);
        option = (option * 10); //Necessary change in the user input to be able to compare it and know what he chosed

        if (option == 10) {                             // Execute option 1 of the menu
            CreateTablesWithDataset();
            printf("\nTables Created Succesfully\n");
        } else if (option == 21) {                      // Execute option 2.1 of the menu
            Option2(1);
        } else if (option == 22) {                      // Execute option 2.2 of the menu
            Option2(2);
        } else if (option == 31) {                      // Execute option 3.1 of the menu
            printf("Executing option 3.1\n");
        } else if (option == 32) {                      // Execute option 3.2 of the menu
            printf("Executing option 3.2\n");
        } else if (option == 41) {                      // Execute option 4.1 of the menu
            printf("Executing option 4.1\n");
        } else if (option == 42) {                      // Execute option 4.2 of the menu
            printf("Executing option 4.2\n");
        } else if (option == 51) {                      // Execute option 5.1 of the menu
            printf("Executing option 5.1\n");
        } else if (option == 52) {                      // Execute option 5.2 of the menu
            printf("Executing option 5.2\n");
        } else if (option == 0) {                       // Execute option 0 of the menu
            printf("Exiting the program...\n");
        } else                                          // Invalid Input
            printf("Invalid option. Please try again.\n");

    } while(option != 0);
    
    printf("\nBye\n");

    //system("Pause");
    return 0;
}