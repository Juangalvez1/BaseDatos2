#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "Functions.h"

void Graph(char pantalla[24][56], float array[12]){
    float arrAux[12] = {0.0};
    for (int i = 0; i < 12; i++) {
        arrAux[i] = array[i];
    }
    double yMax = arrAux[0];
    if((int)yMax / 1000000 >= 1){
        for(int i = 0; i < 12; i++){
            arrAux[i] = arrAux[i] / 1000000;
        }
        yMax = arrAux[0];
    }
    
    for(int i = 0; i < 12; i++){
        if(arrAux[i] > yMax){
            yMax = arrAux[i];
        }
    }
    double pixelY = (fabs(yMax) == 0) ? 24 :  24.0 / fabs(yMax);

    for(int i = 0; i < 24; i++){
        pantalla[i][9] = 179;
    }
    for(int i = 9; i < 56; i++){
        if(i != 9){
            pantalla[23][i] = 196;
        }else{
            pantalla[23][i] = 197;
        }
    }
    for(int i = 0; i < 12; i++){
        int placeY = (int) 24.0-round(pixelY * arrAux[i]);
        char cadena[12] = "";
        sprintf(cadena, "%.2f", arrAux[i]);
        int longitudCadena = strlen(cadena);
        for(int j = 0; j < longitudCadena; j++){
                pantalla[placeY][j] = cadena[j];
        }
        pantalla[placeY][11 + (4 * i)] = 'x';
    }

    return;
}

float GetProductPrice(unsigned short int productKey) {
    FILE *fpProducts = fopen("ProductsTable", "rb+");
    if (fpProducts == NULL) {
        printf("Error opening file: ProductsOrder.table\n");
        return -1.0;
    }
    Products recordProduct;
    int productsIndex = BinarySearch(fpProducts, productKey, 1);
    fseek(fpProducts, productsIndex * sizeof(Products), SEEK_SET);
    fread(&recordProduct, sizeof(Products), 1, fpProducts);
    fclose(fpProducts);
    return recordProduct.UnitPriceUSD;
}

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


void BubbleSortOption3(){
	int numRecordsProducts = TellNumRecords("ProductsTable", sizeof(Products)); 	//Quantity of products in ProductsTable

	FILE *fpProducts = fopen("ProductsTable", "rb+");	//Pointer to ProductsTable
    
    //CreateSalesTable("Sales.csv");

    FILE *fpSales = fopen("SalesTable", "rb+");

	if (fpProducts == NULL){
		printf("Error opening the 'ProductsTable' File");
		return;
	}
    if(fpSales == NULL){
        printf("Error opening the 'SalesTable' File");
        return;
    }


	for (int step = 0; step < numRecordsProducts - 1; step += 1){
		Products reg1, reg2;
		printf("Ordena Products: %i\n", step + 1);
		for (int i = 0; i < numRecordsProducts - step - 1; i += 1){
			fseek(fpProducts, sizeof(Products) * i, SEEK_SET);
			fread(&reg1, sizeof(Products), 1, fpProducts);

			fseek(fpProducts, sizeof(Products) * (i + 1), SEEK_SET);
			fread(&reg2, sizeof(Products), 1, fpProducts);

			if(reg1.ProductKey > reg2.ProductKey){
				fseek(fpProducts, sizeof(Products) * i, SEEK_SET);
				fwrite(&reg2, sizeof(Products), 1, fpProducts);

				fseek(fpProducts, sizeof(Products) * (i + 1), SEEK_SET);
				fwrite(&reg1, sizeof(Products), 1, fpProducts);
			}
		}
	}


	PrintSeasonalAnalysis(fpSales);

    fclose(fpProducts);
    fclose(fpSales);
}

void MergeSortOption3(){
	int numRecordsProducts = TellNumRecords("ProductsTable", sizeof(Products)); 	//Quantity of products in ProductsTable

	FILE *fpProducts = fopen("ProductsTable", "rb+");	//Pointer to ProductsTable
    
    //CreateSalesTable("Sales.csv");

    FILE *fpSales = fopen("SalesTable", "rb+");

	if (fpProducts == NULL){
		printf("Error opening the 'ProductsTable' File");
		return;
	}
    if(fpSales == NULL){
        printf("Error opening the 'SalesTable' File");
        return;
    }
    

	MergeSortFile(fpProducts, 0, numRecordsProducts - 1, sizeof(Products), CompareProductsByProductKey);


	PrintSeasonalAnalysis(fpSales);

    fclose(fpProducts);
    fclose(fpSales);
}
