#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "Functions.h"

/* 
    Function: Graph
    Purpose: Renders a graph representation of a given dataset in a 2D character array.
    Parameters:
      - pantalla: A 24x56 2D character array where the graph will be drawn.
      - array: An array of 12 float values representing the data points to be plotted.
    Returns: None.
*/
void Graph(char pantalla[24][56], float array[12]) {
    float arrAux[12] = {0.0}; // Auxiliary array to modify data without altering the original.
    
    // Copy data to the auxiliary array for processing.
    for (int i = 0; i < 12; i++) {
        arrAux[i] = array[i];
    }

    double yMax = arrAux[0]; // Variable to track the maximum value in the array.

    // Scale values if the maximum value is in millions.
    if ((int)yMax / 1000000 >= 1) {
        for (int i = 0; i < 12; i++) {
            arrAux[i] = arrAux[i] / 1000000;
        }
        yMax = arrAux[0];
    }
    
    // Update yMax to the highest value in the array.
    for (int i = 0; i < 12; i++) {
        if (arrAux[i] > yMax) {
            yMax = arrAux[i];
        }
    }

    // Calculate the number of pixels per unit of Y-axis.
    double pixelY = (fabs(yMax) == 0) ? 24 : 24.0 / fabs(yMax);

    // Draw the Y-axis line.
    for (int i = 0; i < 24; i++) {
        pantalla[i][9] = 179; // ASCII code for vertical line.
    }

    // Draw the X-axis line.
    for (int i = 9; i < 56; i++) {
        pantalla[23][i] = (i != 9) ? 196 : 197; // Horizontal line or intersection.
    }

    // Plot the data points on the graph.
    for (int i = 0; i < 12; i++) {
        int placeY = (int)(24.0 - round(pixelY * arrAux[i])); // Calculate Y-coordinate.
        char cadena[12] = ""; // Temporary buffer for the numeric string.
        sprintf(cadena, "%.2f", arrAux[i]); // Convert value to string.
        int longitudCadena = strlen(cadena); // Length of the numeric string.
        
        // Place the numeric value on the graph.
        for (int j = 0; j < longitudCadena; j++) {
            pantalla[placeY][j] = cadena[j];
        }

        // Mark the data point with 'x'.
        pantalla[placeY][11 + (4 * i)] = 'x';
    }
}

/* 
    Function: GetProductPrice
    Purpose: Fetches the unit price of a product based on its product key.
    Parameters:
      - productKey: An unsigned short integer representing the unique identifier of the product.
    Returns: The unit price of the product as a float, or -1.0 if the file could not be opened.
*/
float GetProductPrice(unsigned short int productKey) {
    // Open the ProductsTable file.
    FILE *fpProducts = fopen("ProductsTable", "rb+");
    if (fpProducts == NULL) {
        printf("Error opening file: ProductsTable\n");
        return -1.0; // Return error value.
    }

    Products recordProduct; // Structure to hold product details.
    
    // Use BinarySearch to locate the product in the table.
    int productsIndex = BinarySearch(fpProducts, productKey, 1);

    // Read the product record.
    fseek(fpProducts, productsIndex * sizeof(Products), SEEK_SET);
    fread(&recordProduct, sizeof(Products), 1, fpProducts);

    fclose(fpProducts); // Close the file.
    return recordProduct.UnitPriceUSD; // Return the unit price.
}

/* 
    Function: PrintSeasonalAnalysis
    Purpose: Analyzes monthly order volumes and income, identifying seasonal patterns in the data.
    Parameters:
      - fpSales: A file pointer to the SalesTable file.
    Returns: None.
*/
void PrintSeasonalAnalysis(FILE *fpSales) {
    float monthlyOrders[12] = {0.0};
    float monthlyIncome[12] = {0.0};
    Sales recordSale;

    if (fpSales == NULL) {
        printf("Error opening file: Sales.table\n");
        return;
    }

    Sales recordsStartDate, recordFinalDate;
    int yearsAnalyzed = 0;
    fread(&recordsStartDate, sizeof(Sales), 1, fpSales);
    
    fseek(fpSales, -1 * sizeof(Sales), SEEK_END);
    fread(&recordFinalDate, sizeof(Sales), 1, fpSales);
    
    fseek(fpSales, 0, SEEK_SET);
    yearsAnalyzed = recordFinalDate.OrderDate.AAAA - recordsStartDate.OrderDate.AAAA + 1;

    int monthIndex = 0;
    while (fread(&recordSale, sizeof(Sales), 1, fpSales)) {
        monthIndex = recordSale.OrderDate.MM - 1;
        monthlyOrders[monthIndex] += recordSale.Quantity;
        monthlyIncome[monthIndex] += recordSale.Quantity * GetProductPrice(recordSale.ProductKey);
    }
    fclose(fpSales);

    float averageMonthlyOrders[12];
    float averageMonthlyIncome[12];
    for (int i = 0; i < 12; i++) {
        averageMonthlyOrders[i] = monthlyOrders[i] / yearsAnalyzed;
        averageMonthlyIncome[i] = monthlyIncome[i] / yearsAnalyzed;
    }

    char pantalla[24][56] = {{}};
    memset(pantalla, ' ', sizeof(pantalla));
    char meses[] = " ene feb mar abr may jun jul ago sep oct nov dic";

    printf("Title: Analysis of Seasonal Patterns in Orders and Income for Company Global Electronics Retailer\n");
    printf("This report aims to analyze whether there are seasonal patterns or trends in order volume and Income.\n");
    printf("Methodology: Data collection: Historical order and revenue data is collected, with an appropriate time range to identify seasonal patterns.\n");
    printf("Data preparation: The data will be organized to facilitate analysis.\n\n");
    printf("Order Volume and Monthly income Analysis\nWhen examining the order volume and monthly income data, a direct relationship between the two is observed, making it easy to identify the increases and\ndecreases in both income and the number of orders. This analysis is especially useful when visualizing these patterns through charts and tables\nfacilitating the understanding of the peaks and drops that affect both sales volume and income. Additionally, there is a noticeable stability\nin certain months, highlighting the seasonality of the business.");
    printf("Order and income Peaks\nFebruary and December stand out with the highest order volumes (%.0f and %.0f orders) and incomes ($%.2f and $%.2f)\ndue to year-end promotions and January sales.\n", monthlyOrders[1], monthlyOrders[11], monthlyIncome[1], monthlyIncome[11]);
    printf("Months with Fewer Orders and income\nIn March and April, both orders and income drop significantly, with %.0f and %.0f orders respectively. This is due to lower demand after the holidays\nand the lack of promotions or incentives.\n", monthlyOrders[2], monthlyOrders[3]);
    printf("Stability in the Intermediate Months\nMay, June, and July show steady demand, with 4,000 to 5,000 orders per month, suggesting that activity remains balanced during these intermediate months.\n");
    printf("Monthly Averages\nThe average order volumes follow the same seasonal trend, with February and December reaching the highest peaks, while March and April show the\nlowest values, confirming the post-holiday drop.\n\n");
    printf("Results:\nChart 1: Order volume per month from %i/%i/%i to %i/%i/%i\n", recordsStartDate.OrderDate.MM, recordsStartDate.OrderDate.DD, recordsStartDate.OrderDate.AAAA, recordFinalDate.OrderDate.MM, recordFinalDate.OrderDate.DD, recordFinalDate.OrderDate.AAAA);
    printf("-------------------------\n|Month\t|Order Volume\t|\n-------------------------\t");
    Graph(pantalla, monthlyOrders);
    for (int i = 0; i < 24; i++) {
        for (int j = 0; j < 56; j++) {
            printf("%c", pantalla[i][j]);
        }
        printf("\n");
        if (i % 2 == 0) {
            printf("|%d\t|%.0f\t\t|\t", (i / 2) + 1, monthlyOrders[(i / 2)]);
        } else if (i == 23) {
            printf("-------------------------\t         %s", meses);
        } else {
            printf("-------------------------\t");
        }
    }
    printf("\n\nChart 2: Income per month from %i/%i/%i to %i/%i/%i\n", recordsStartDate.OrderDate.MM, recordsStartDate.OrderDate.DD, recordsStartDate.OrderDate.AAAA, recordFinalDate.OrderDate.MM, recordFinalDate.OrderDate.DD, recordFinalDate.OrderDate.AAAA);
    printf("-------------------------\n|Month\t|Total Income\t|\t\tMillions USD\n-------------------------\t");
    memset(pantalla, ' ', sizeof(pantalla));
    Graph(pantalla, monthlyIncome);
    for (int i = 0; i < 24; i++) {
        for (int j = 0; j < 56; j++) {
            printf("%c", pantalla[i][j]);
        }
        printf("\n");
        if (i % 2 == 0) {
            printf("|%d\t|%.2f\t|\t", (i / 2) + 1, monthlyIncome[(i / 2)]);
        } else if (i == 23) {
            printf("-------------------------\t         %s", meses);
        } else {
            printf("-------------------------\t");
        }
    }
    printf("\n\nChart 3: Monthly average order volume from %i/%i/%i to %i/%i/%i\n", recordsStartDate.OrderDate.MM, recordsStartDate.OrderDate.DD, recordsStartDate.OrderDate.AAAA, recordFinalDate.OrderDate.MM, recordFinalDate.OrderDate.DD, recordFinalDate.OrderDate.AAAA);
    printf("-------------------------\n|Month\t|Monthly Average|\n-------------------------\t");
    memset(pantalla, ' ', sizeof(pantalla));
    Graph(pantalla, averageMonthlyOrders);
    for (int i = 0; i < 24; i++) {
        for (int j = 0; j < 56; j++) {
            printf("%c", pantalla[i][j]);
        }
        printf("\n");
        if (i % 2 == 0) {
            printf("|%d\t|%.0f\t\t|\t", (i / 2) + 1, averageMonthlyOrders[(i / 2)]);
        } else if (i == 23) {
            printf("-------------------------\t         %s", meses);
        } else {
            printf("-------------------------\t");
        }
    }
    printf("\n\nChart 4: Monthly average income from %i/%i/%i to %i/%i/%i\n", recordsStartDate.OrderDate.MM, recordsStartDate.OrderDate.DD, recordsStartDate.OrderDate.AAAA, recordFinalDate.OrderDate.MM, recordFinalDate.OrderDate.DD, recordFinalDate.OrderDate.AAAA);
    printf("-------------------------\n|Month\t|Total Income\t|\t\tMillions USD\n-------------------------\t");
    memset(pantalla, ' ', sizeof(pantalla));
    Graph(pantalla, averageMonthlyIncome);
    for (int i = 0; i < 24; i++) {
        for (int j = 0; j < 56; j++) {
            printf("%c", pantalla[i][j]);
        }
        printf("\n");
        if (i % 2 == 0) {
            printf("|%d\t|%.2f\t|\t", (i / 2) + 1, monthlyIncome[(i / 2)]);
        } else if (i == 23) {
            printf("-------------------------\t         %s", meses);
        } else {
            printf("-------------------------\t");
        }
    }
    printf("\nConclusions:\nClear seasonal patterns show sales peaks in December and February, followed by drops in March and April.\n");
    printf("It is recommended to take advantage of demand peaks with marketing strategies and promotions, while managing the low-demand months with offers and\ndiscounts to maintain income flow.\n");
    printf("\nRecommendations:\nStrengthen marketing efforts during low-demand months (March and April) with promotions and discounts.\nIncrease inventory during high-demand months (November, December, and January) to meet the sales peaks.\n");
    printf("Reduce inventory during months of lower activity (March and April) to avoid overstocking and additional costs.\nBuild customer loyalty with reward programs that encourage purchases in low months\n");

}

/* 
    Function: BubbleSortOption3
    Purpose: Sorts the ProductsTable using BubbleSort based on ProductKey and prints a seasonal analysis of sales data.
    Parameters: None.
    Returns: None.
*/
void BubbleSortOption3() {
    // Get the number of products in the ProductsTable.
    int numRecordsProducts = TellNumRecords("ProductsTable", sizeof(Products)); 

    // Open the ProductsTable and SalesTable files for reading and writing.
    FILE *fpProducts = fopen("ProductsTable", "rb+"); 
    FILE *fpSales = fopen("SalesTable", "rb+");

    // Check if files are successfully opened.
    if (fpProducts == NULL) {
        printf("Error opening the 'ProductsTable' File");
        return; // Exit function if ProductsTable file can't be opened.
    }
    if (fpSales == NULL) {
        printf("Error opening the 'SalesTable' File");
        return; // Exit function if SalesTable file can't be opened.
    }

    // Perform BubbleSort to sort ProductsTable by ProductKey.
    for (int step = 0; step < numRecordsProducts - 1; step++) {
        Products reg1, reg2; // Structures to hold two product records for comparison.
        printf("Sorting Products: %i\n", step + 1); // Print the current sorting step.
        
        // Perform BubbleSort on ProductKey field.
        for (int i = 0; i < numRecordsProducts - step - 1; i++) {
            fseek(fpProducts, sizeof(Products) * i, SEEK_SET); // Move file pointer to the i-th product.
            fread(&reg1, sizeof(Products), 1, fpProducts); // Read the i-th product.

            fseek(fpProducts, sizeof(Products) * (i + 1), SEEK_SET); // Move file pointer to the next product.
            fread(&reg2, sizeof(Products), 1, fpProducts); // Read the (i+1)-th product.

            // Compare the ProductKey of two products and swap if necessary.
            if (reg1.ProductKey > reg2.ProductKey) {
                fseek(fpProducts, sizeof(Products) * i, SEEK_SET); // Move pointer to i-th product.
                fwrite(&reg2, sizeof(Products), 1, fpProducts); // Write reg2 to i-th position.

                fseek(fpProducts, sizeof(Products) * (i + 1), SEEK_SET); // Move pointer to (i+1)-th product.
                fwrite(&reg1, sizeof(Products), 1, fpProducts); // Write reg1 to (i+1)-th position.
            }
        }
    }

    // Call the function to print seasonal analysis using sales data from SalesTable.
    PrintSeasonalAnalysis(fpSales);

    // Close the file pointers after the operation is completed.
    fclose(fpProducts);
    fclose(fpSales);
}

/* 
    Function: MergeSortOption3
    Purpose: Sorts the ProductsTable using MergeSort based on ProductKey and prints a seasonal analysis of sales data.
    Parameters: None.
    Returns: None.
*/
void MergeSortOption3() {
    // Get the number of products in the ProductsTable.
    int numRecordsProducts = TellNumRecords("ProductsTable", sizeof(Products)); 

    // Open the ProductsTable and SalesTable files for reading and writing.
    FILE *fpProducts = fopen("ProductsTable", "rb+");
    FILE *fpSales = fopen("SalesTable", "rb+");

    // Check if files are successfully opened.
    if (fpProducts == NULL) {
        printf("Error opening the 'ProductsTable' File");
        return; // Exit function if ProductsTable file can't be opened.
    }
    if (fpSales == NULL) {
        printf("Error opening the 'SalesTable' File");
        return; // Exit function if SalesTable file can't be opened.
    }

    // Perform MergeSort to sort ProductsTable by ProductKey.
    MergeSortFile(fpProducts, 0, numRecordsProducts - 1, sizeof(Products), CompareProductsByProductKey);

    // Call the function to print seasonal analysis using sales data from SalesTable.
    PrintSeasonalAnalysis(fpSales);

    // Close the file pointers after the operation is completed.
    fclose(fpProducts);
    fclose(fpSales);
}
