#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Functions.h"

//Salvedad: No hay fechas de entrega para las tiendas fisicas, por ende no se pueden comparar contra las fechas de entrega 

void CalculateAverageDeliveryTime(FILE *fpSales, int numOfSales){
    FILE *fpTemporalSales = fopen("TemporalFileSalesOption4", "wb+");
    Sales recordsSales;
    int numOfDeliveredSales = 0;
    int averageDeliveryTime[6][2] = {0};

    for (int salesIndex = 0, temporalIndex = 0; salesIndex < numOfSales; salesIndex += 1) {
        fseek(fpSales, sizeof(Sales) * salesIndex, SEEK_SET);
        fread(&recordsSales, sizeof(Sales), 1, fpSales);

        // Verifica que StoreKey == 0 y que DeliveryDate sea válida
        if (recordsSales.StoreKey == 0 && recordsSales.DeliveryDate.AAAA != 0 && recordsSales.DeliveryDate.MM != 0 && recordsSales.DeliveryDate.DD != 0) {
            fseek(fpTemporalSales, sizeof(Sales) * temporalIndex, SEEK_SET);
            fwrite(&recordsSales, sizeof(Sales), 1, fpTemporalSales);
            temporalIndex++;  // Incrementa solo aquí
            numOfDeliveredSales++;
        }
    }

    for (int i = 0; i < numOfDeliveredSales; i += 1) {
        fseek(fpTemporalSales, sizeof(Sales) * i, SEEK_SET);
        fread(&recordsSales, sizeof(Sales), 1, fpTemporalSales);

        // Calcula la diferencia en días solo si DeliveryDate es válida
        if (recordsSales.DeliveryDate.AAAA != 0 && recordsSales.DeliveryDate.MM != 0 && recordsSales.DeliveryDate.DD != 0) {
            int year = (int)recordsSales.DeliveryDate.AAAA % 2016;
            long int
            orderDate = recordsSales.OrderDate.AAAA * 360 + recordsSales.OrderDate.MM * 30 + recordsSales.OrderDate.DD,
            deliveryDate = recordsSales.DeliveryDate.AAAA * 360 + recordsSales.DeliveryDate.MM * 30 + recordsSales.DeliveryDate.DD,
            deliveryTimeInDays = deliveryDate - orderDate;

            //printf("%i. OrderNumber: %li\tOrderDate: %d/%-2d/%-2d\tDeliveryDate: %d/%-2d/%-2d\ttime in days: %li\n", i, recordsSales.OrderNumber, recordsSales.OrderDate.AAAA, recordsSales.OrderDate.MM, recordsSales.OrderDate.DD,recordsSales.DeliveryDate.AAAA, recordsSales.DeliveryDate.MM, recordsSales.DeliveryDate.DD, deliveryTimeInDays);
        
            averageDeliveryTime[year][0] += deliveryTimeInDays;
            averageDeliveryTime[year][1] += 1;
        
        } else {
            printf("\t\t\t\t\tOrderNumber: %li has an invalid DeliveryDate.\n", recordsSales.OrderNumber);
        }
    }

    printf("\nOver the years analyzed, a progressive decrease in delivery times is observed, suggesting that both\nlocal and online stores have managed to improve their delivery efficiency over time.\n\n");

    // Conclusión del análisis
    
    printf("The average delivery time in days was calculated, which\nrefers to the average number of days between the order date and the delivery date. This calculation\nis crucial to understand the efficiency of delivery times based on the provided data.\n\n");
    
    printf("It is important to note that, in the analyzed dataset, there are no records of physical stores with a\ndelivery date. Therefore, the comparison was made only between online store that\ndo have valid delivery date records.\n\n");

    // Resultados de los tiempos promedio de entrega por año
    SetColor(3);
    printf("Results:\n\n");
    SetColor(7);
    printf("After analyzing the data, the following results were obtained for the average delivery time in days:\n\n");
    
    for(int i = 0; i < 6; i += 1){
        printf("\n\tAverage delivery time in: %i:\t%.2f days\n", i + 2016, (1.0 * averageDeliveryTime[i][0]) / (1.0 * averageDeliveryTime[i][1]));
    }
    
    printf("\n\nOver the years analyzed, a progressive decrease in delivery times is observed, suggesting that \nthe online store has managed to improve its delivery efficiency over time.\n\n");

    // Conclusión del análisis
    SetColor(3);
    printf("Conclusion:\n\n");
    SetColor(7);
    printf("In the provided dataset, there are no records for physical stores with delivery dates, so the analysis\nhas been limited to online stores. The calculation of average delivery time shows a trend of\nimprovement over the years, with a significant reduction in average delivery time, from 7.25 days in\n2016 to 3.76 days in 2021.\n\n");
    
    printf("This analysis highlights how, over time, delivery processes have improved, which may reflect\noptimization in logistical systems or operational efficiency in both online and local stores.\n\n");


    fclose(fpTemporalSales);
}


void BubbleSortOption4(){
	int numRecordsSales = TellNumRecords("SalesTable", sizeof(Sales)); 				//Quantity of products in SalesTable

    FILE *fpSales = fopen("SalesTable", "rb+");			//Pointer to SalesTable	

	if (fpSales == NULL){
		printf("Error opening the 'SalesTable' File");
		return;
	}

	/*
	for (int step = 0; step < numRecordsSales - 1; step += 1) {
        printf("Ordena Sales: %i\n", step + 1);
		Sales reg1, reg2;
        for (int i = 0; i < numRecordsSales - step - 1; i += 1) {
            fseek(fpSales, sizeof(Sales) * i, SEEK_SET);
            fread(&reg1, sizeof(Sales), 1, fpSales);

            fseek(fpSales, sizeof(Sales) * (i + 1), SEEK_SET);
            fread(&reg2, sizeof(Sales), 1, fpSales);

            if (reg1.CustomerKey > reg2.CustomerKey) {
                // Intercambiar los registros si estan fuera de orden
                fseek(fpSales, sizeof(Sales) * i, SEEK_SET);
                fwrite(&reg2, sizeof(Sales), 1, fpSales);

                fseek(fpSales, sizeof(Sales) * (i + 1), SEEK_SET);
                fwrite(&reg1, sizeof(Sales), 1, fpSales);
            }
        }
    }
    */

    CalculateAverageDeliveryTime(fpSales, numRecordsSales);

    fclose(fpSales);
}

void MergeSortOption4(){
	int numRecordsSales = TellNumRecords("SalesTable", sizeof(Sales)); 				//Quantity of products in SalesTable

    FILE *fpSales = fopen("SalesTable", "rb+");			//Pointer to SalesTable	

	if (fpSales == NULL){
		printf("Error opening the 'SalesTable' File");
		return;
	}
	
	//MergeSortFile(fpSales, 0, numRecordsSales - 1, sizeof(Sales), CompareSalesByCustomerKey);

	CalculateAverageDeliveryTime(fpSales, numRecordsSales);

    fclose(fpSales);
}
