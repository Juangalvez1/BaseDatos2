/*
- Fecha de publicación: 22/11/2024
- Hora de publicación: 2:52 AM
- Versión de su código: 5.2
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
  No se garantiza su funcionamiento en otras versiones del sistema operativo, en otros sistemas operativos, en otras versiones del 
  lenguaje, del compilador u otro compilador.
  Primero se debe ejecutar la opcion 1 del menu, creando las tablas, asi mismo, esta debe de ejecutarse antes de cualquier otra opcion
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "Functions.c"
#include "ExecuteOption2Files.c"
#include "ExecuteOption3.c"
#include "ExecuteOption4.c"
#include "ExecuteOption5.c"

// Function ShowDateTime
// Prints the current date and time in a specific format.
// It uses the system's current time and formats it as a readable string.
// Example output: "Valid to 2024-Nov-21 at 14:00 hours".
void ShowDateTime() {
    time_t current_time = time(NULL); // Holds the current time in seconds since epoch.
    struct tm *local_time = localtime(&current_time); // Holds the local time as a structured representation.
    char formattedTime[50] = ""; // Buffer to store the formatted time string.

    // Format the date and time as "Valid to YYYY-MMM-DD at HH:MM hours".
    strftime(formattedTime, 50, "Valid to %Y-%b-%d at %H:%M hours", local_time);
    printf("%s\n", formattedTime); // Print the formatted date and time.
}

// Function PrintExecutionTime
// Prints the total time in minutes and seconds spent executing a task.
// Parameter 'time' represents the duration in seconds.
void PrintExecutionTime(double time) {
    int minutes = 0, seconds = 0; // Variables to hold minutes and seconds.
    char totalTime[6] = ""; // Buffer to store the formatted time as "MM'SS''".

    minutes = (int)time / 60; // Calculate the number of minutes.
    seconds = (int)time % 60; // Calculate the remaining seconds.

    sprintf(totalTime, "%02d'%02d''", minutes, seconds); // Format time as "MM'SS''".
    printf("%s", totalTime); // Print the formatted execution time.
}

// Function PrintMenu
// Displays the options menu to the user for selecting various program functionalities.
void PrintMenu() {
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
    printf("5. List of sales ordered by 'Customer Name' + 'Order Date' + 'ProductKey';\n");
    printf("   5.1 Utility bubbleSort\n");
    printf("   5.2 Utility mergeSort\n");
    printf("What is your option: ");
}

// Function Option2
// Executes the second menu option: listing products sorted by ProductName, location, and more.
// The parameter 'sortType' determines the sorting algorithm to use (1 = Bubble Sort, 2 = Merge Sort).
void Option2(int sortType) {
    int start = 0, finish = 0; // Variables to store the start and end time of execution.
    double totalSeconds = 0.0; // Variable to store the total execution time in seconds.

    start = clock(); // Record the start time.
    printf("\n---------------------------------------------------------------------------------------");
    printf("\nCompany Global Electronics Retailer\n");
    ShowDateTime(); // Print the current date and time.
    printf("Products list ordered by ProductName + Continent + Country + State + City\n");

    // Temporary file to hold sorted data.
    FILE *fp = fopen("TemporalFileOption2", "w");
    fclose(fp);

    if (sortType == 1) {
        BubbleSortOption2(); // Perform Bubble Sort.
    } else if (sortType == 2) {
        MergeSortOption2(); // Perform Merge Sort.
    }

    fp = fopen("TemporalFileOption2", "w");
    fclose(fp);

    finish = clock(); // Record the end time.

    totalSeconds = ((double)(finish - start)) / CLOCKS_PER_SEC; // Calculate execution time.

    printf("\n---------------------------------------------------------------------------------------");
    printf("\nTime used to produce this listing: ");
    PrintExecutionTime(totalSeconds); // Print the total execution time.
    printf("\n********************************LAST LINE OF THE REPORT********************************\n");
}

// Function Option3
// Executes the third menu option: analyzing seasonal patterns or trends in order volume or revenue.
// The parameter 'sortType' determines the sorting algorithm to use (1 = Bubble Sort, 2 = Merge Sort).
void Option3(int sortType) {
    int start = 0, finish = 0; // Variables to store the start and end time of execution.
    double totalSeconds = 0.0; // Variable to store the total execution time in seconds.

    start = clock(); // Record the start time.
    printf("\n---------------------------------------------------------------------------------------");
    printf("\nCompany Global Electronics Retailer\n");
    ShowDateTime(); // Print the current date and time.
    printf("\n");

    if (sortType == 1) {
        BubbleSortOption3(); // Perform Bubble Sort.
    } else if (sortType == 2) {
        MergeSortOption3(); // Perform Merge Sort.
    }

    finish = clock(); // Record the end time.

    totalSeconds = ((double)(finish - start)) / CLOCKS_PER_SEC; // Calculate execution time.

    printf("\n---------------------------------------------------------------------------------------");
    printf("\nTime used to produce this listing: ");
    PrintExecutionTime(totalSeconds); // Print the total execution time.
    printf("\n********************************LAST LINE OF THE REPORT********************************\n");
}

// Function Option4
// Executes the fourth menu option: analyzing delivery times and their changes over time.
// The parameter 'sortType' determines the sorting algorithm to use (1 = Bubble Sort, 2 = Merge Sort).
void Option4(int sortType) {
    int start = 0, finish = 0; // Variables to store the start and end time of execution.
    double totalSeconds = 0.0; // Variable to store the total execution time in seconds.

    start = clock(); // Record the start time.
    printf("\n---------------------------------------------------------------------------------------");
    printf("\nCompany Global Electronics Retailer\n");
    ShowDateTime(); // Print the current date and time.

    if (sortType == 1) {
        BubbleSortOption4(); // Perform Bubble Sort.
    } else if (sortType == 2) {
        MergeSortOption4(); // Perform Merge Sort.
    }

    finish = clock(); // Record the end time.

    totalSeconds = ((double)(finish - start)) / CLOCKS_PER_SEC; // Calculate execution time.

    printf("\n---------------------------------------------------------------------------------------");
    printf("\nTime used to produce this listing: ");
    PrintExecutionTime(totalSeconds); // Print the total execution time.
    printf("\n********************************LAST LINE OF THE REPORT********************************\n");
}

// Function Option5
// Executes the fifth menu option: listing sales ordered by Customer Name, Order Date, and ProductKey.
// The parameter 'sortType' determines the sorting algorithm to use (1 = Bubble Sort, 2 = Merge Sort).
void Option5(int sortType) {
    int start = 0, finish = 0; // Variables to store the start and end time of execution.
    double totalSeconds = 0.0; // Variable to store the total execution time in seconds.

    start = clock(); // Record the start time.
    printf("\n---------------------------------------------------------------------------------------");
    printf("\nCompany Global Electronics Retailer\n");
    ShowDateTime(); // Print the current date and time.
    printf("Customer list ordered by Customer name + order date for sale + ProductKey\n");

    // Temporary file to hold sorted sales data.
    FILE *fpS = fopen("TemporalFileSalesOption5", "w");
    fclose(fpS);

    if (sortType == 1) {
        BubbleSortOption5(); // Perform Bubble Sort.
    } else if (sortType == 2) {
        MergeSortOption5(); // Perform Merge Sort.
    }

    fpS = fopen("TemporalFileSalesOption5", "w");
    fclose(fpS);

    finish = clock(); // Record the end time.

    totalSeconds = ((double)(finish - start)) / CLOCKS_PER_SEC; // Calculate execution time.

    printf("\n---------------------------------------------------------------------------------------");
    printf("\nTime used to produce this listing: ");
    PrintExecutionTime(totalSeconds); // Print the total execution time.
    printf("\n********************************LAST LINE OF THE REPORT********************************\n");
}

// Main function
// Entry point of the program. Displays the menu and executes selected options.
int main() {
    int creationOfTables = 0; // Tracks whether tables have been created.
    float option = 0.0; // Stores the user's menu choice.

    printf("Welcome!\n");

    do {
        SetColor(7); // Reset console text color.
        PrintMenu(); // Display the menu options.
        scanf("%f", &option); // Get user input.
        option = (option * 10); // Adjust input for easier comparison.

        if (option == 10) {
            CreateTablesWithDataset(); // Create database tables.
            SetColor(9);
            system("cls");
            printf("\nTables Created Successfully\n");
            creationOfTables++;
        } else if (option == 21 && creationOfTables) {
            system("cls");
            Option2(1); // Execute Bubble Sort for Option 2.
            creationOfTables = 0;
        } else if (option == 22 && creationOfTables) {
            system("cls");
            Option2(2); // Execute Merge Sort for Option 2.
            creationOfTables = 0;
        } else if (option == 31 && creationOfTables) {
            system("cls");
            Option3(1); // Execute Bubble Sort for Option 3.
            creationOfTables = 0;
        } else if (option == 32 && creationOfTables) {
            system("cls");
            Option3(2); // Execute Merge Sort for Option 3.
            creationOfTables = 0;
        } else if (option == 41 && creationOfTables) {
            system("cls");
            Option4(1); // Execute Bubble Sort for Option 4.
            creationOfTables = 0;
        } else if (option == 42 && creationOfTables) {
            system("cls");
            Option4(2); // Execute Merge Sort for Option 4.
            creationOfTables = 0;
        } else if (option == 51 && creationOfTables) {
            system("cls");
            Option5(1); // Execute Bubble Sort for Option 5.
            creationOfTables = 0;
        } else if (option == 52 && creationOfTables) {
            system("cls");
            Option5(2); // Execute Merge Sort for Option 5.
            creationOfTables = 0;
        } else if (option == 0) {
            system("cls");
            SetColor(4);
            printf("Exiting the program...\n");
        } else {
            system("cls");
            SetColor(13);
            printf("Invalid option. Please try again.\n");
            SetColor(7);
        }
    } while (option != 0);

    SetColor(11);
    printf("\nBye\n");
    SetColor(7);
    return 0;
}
