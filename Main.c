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
#include "ExecuteOption2Files.c"
#include "ExecuteOption4.c"
#include "ExecuteOption5.c"


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

void PrintMenu(){
    printf("\nCompany Global Electronics Retailer\n");
    printf("Options menu\n");
    printf("0. Exit Program\n");
    printf("1. Construction of the Database with the dataset tables\n");
    printf("2. List of %cWhat types of products does the company sell, and where are costumers located?\n", 168);
    printf("   2.1 Utility bubbleSort\n");
    printf("   2.2 Utility mergeSort\n");
    printf("3. List of %cAre there any seasonal patterns or trends for order volume or revenue?\n", 168);
    printf("   3.1 Utility bubbleSort\n");
    printf("   3.2 Utility mergeSort\n");
    printf("4. List of %cHow long is the average delivery time in days? Has that changed over time?\n", 168);
    printf("   4.1 Utility bubbleSort\n");
    printf("   4.2 Utility mergeSort\n");
    printf("5. List of sales order by 'Costumer Name' + 'Order Date' + 'ProductKey';\n");
    printf("   5.1 Utility bubbleSort\n");
    printf("   5.2 Utility mergeSort\n");
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
    
    FILE *fp = fopen("TemporalFileOption2", "w");
    fclose(fp);

    if(sortType == 1){
        BubbleSortOption2();
    } else if (sortType == 2){
        MergeSortOption2();
    }

    fp = fopen("TemporalFileOption2", "w");
    fclose(fp);

    finish = clock();

    totalSeconds = ((double)(finish - start)) / CLOCKS_PER_SEC;
    
    printf("\n---------------------------------------------------------------------------------------");
    printf("\nTime used to produce this listing: "); 
    PrintExecutionTime(totalSeconds);
    printf("\n********************************LAST LINE OF THE REPORT********************************\n");
}

void Option4(int sortType){
    int start = 0, finish = 0;
    double totalSeconds = 0.;
    start = clock();
    printf("\n---------------------------------------------------------------------------------------");
    printf("\nCompany Global Electronics Retailer\n");
    ShowDateTime();
    //printf("Customer list ordered by Costumer name  + order date for sale + ProductKey\n");

    if(sortType == 1){
        BubbleSortOption4();
    } else if (sortType == 2){
        MergeSortOption4();
    }


    finish = clock();

    totalSeconds = ((double)(finish - start)) / CLOCKS_PER_SEC;
    
    printf("\n---------------------------------------------------------------------------------------");
    printf("\nTime used to produce this listing: "); 
    PrintExecutionTime(totalSeconds);
    printf("\n********************************LAST LINE OF THE REPORT********************************\n");
}

void Option5(int sortType){
    int start = 0, finish = 0;
    double totalSeconds = 0.;
    start = clock();
    printf("\n---------------------------------------------------------------------------------------");
    printf("\nCompany Global Electronics Retailer\n");
    ShowDateTime();
    printf("Customer list ordered by Costumer name  + order date for sale + ProductKey\n");

    FILE *fpS = fopen("TemporalFileSalesOption5", "w");
    fclose(fpS);

    if(sortType == 1){
        BubbleSortOption5();
    } else if (sortType == 2){
        MergeSortOption5();
    }

    fpS = fopen("TemporalFileSalesOption5", "w");
    fclose(fpS);


    finish = clock();

    totalSeconds = ((double)(finish - start)) / CLOCKS_PER_SEC;
    
    printf("\n---------------------------------------------------------------------------------------");
    printf("\nTime used to produce this listing: "); 
    PrintExecutionTime(totalSeconds);
    printf("\n********************************LAST LINE OF THE REPORT********************************\n");
}

int main() {
    int creationOfTables = 0;
    float option = 0.;
    printf("Welcome!\n");

    do {
        SetColor(7);
        PrintMenu();
        scanf("%f", &option);
        option = (option * 10); //Necessary change in the user input to be able to compare it and know what he chosed
        //printf("%f\n", option);
        if (option == 10) {                                 // Execute option 1 of the menu
            CreateTablesWithDataset();
            SetColor(9);
            system("cls");
            printf("\nTables Created Succesfully\n");
            creationOfTables++;
        } else if (option == 21 && creationOfTables) {      // Execute option 2.1 of the menu
            system("cls");
            Option2(1);
            creationOfTables = 0;
        } else if (option == 22 && creationOfTables) {      // Execute option 2.2 of the menu
            system("cls");
            Option2(2);
            creationOfTables = 0;
        } else if (option == 31 && creationOfTables) {      // Execute option 3.1 of the menu
            system("cls");
            printf("Executing option 3.1\n");
            creationOfTables = 0;
        } else if (option == 32 && creationOfTables) {      // Execute option 3.2 of the menu
            system("cls");
            printf("Executing option 3.2\n");
            creationOfTables = 0;
        } else if (option == 41 && creationOfTables) {      // Execute option 4.1 of the menu
            system("cls");
            Option4(1);
            creationOfTables = 0;
        } else if (option == 42 && creationOfTables) {      // Execute option 4.2 of the menu
            system("cls");
            Option4(2);
            creationOfTables = 0;
        } else if (option == 51 && creationOfTables) {      // Execute option 5.1 of the menu
            system("cls");
            Option5(1);
            creationOfTables = 0;
        } else if (option == 52 && creationOfTables) {      // Execute option 5.2 of the menu
            system("cls");
            Option5(2);
            creationOfTables = 0;
        } else if (option == 0) {                           // Execute option 0 of the menu
            system("cls");
            SetColor(4);
            printf("Exiting the program...\n");
        } else {                                            // Invalid Input
            system("cls");
            SetColor(13);
            printf("Invalid option. Please try again.\n");
            SetColor(7);
        }
    } while(option != 0);
    SetColor(11);
    printf("\nBye\n");
    SetColor(7);
    //system("Pause");
    return 0;
}