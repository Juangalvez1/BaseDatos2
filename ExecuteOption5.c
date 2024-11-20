#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Functions.h"

void ShowCustomersPurchases(FILE *fpSales, FILE *fpCustomers, FILE *fpProducts, int numOfCustomers, int typeofSort){
	//printf("%i", typeofSort);
	
	Sales recordSale;				//Used to store a record of SalesTable and get its information
	Customers recordCustomer;		//Used to store a record of CustomersTable and store it temporarely in TemporalFileOption2
	ExchangeRates recordExchange;
	
	FILE *fpExchangeRates = NULL;
	FILE *fpTemporalSales = NULL;

    char customerName[40] = ""; 						//Used to store the ProductName of each Product in ProductsTable
	unsigned int customerKey = 0; 						//Used to store the ProductKey of each Product in ProductTable

    for(int i = 0; i < numOfCustomers; i += 1){
        fseek(fpCustomers, sizeof(Customers) * i, SEEK_SET);
    	fread(&recordCustomer, sizeof(Customers), 1, fpCustomers);

        strcpy(customerName, recordCustomer.Name);
        printf("\nCustomer name: %-40s", customerName);

        customerKey = recordCustomer.CustomerKey;
		//printf("%u\n", customerKey);
        int positionSales = BinarySearch(fpSales, customerKey, 4);
        if(positionSales == -1){
            printf(" - No purchases reported\n");
        } else {
			fpTemporalSales = fopen("TemporalFileSalesOption5", "rb+");
			fpExchangeRates = fopen("ExchangeRatesTable", "rb+");

			if(fpTemporalSales == NULL){
				printf("Error abiendo el temporal de sales\n");
				return;
			}
			if(fpExchangeRates == NULL){
				printf("Error abriendo el archivo de exchange\n");
				return;
			}

            fseek(fpSales, (positionSales - 1) * sizeof(Sales), SEEK_SET);
            fread(&recordSale, sizeof(Sales), 1, fpSales);

            for( int i = positionSales - 1; i >= 0 && customerKey == recordSale.CustomerKey; i -= 1){
				//Reading of the previous record in sales to verify if its the first one with the currenr CustomerKey
    	    	fseek(fpSales, sizeof(Sales) * (i - 1), SEEK_SET);
    	    	fread(&recordSale, sizeof(Sales), 1, fpSales);

    	    	positionSales = i; //Changing positionSales to be the index of first record in sales with the current CustomerKey
    	    }
            fseek(fpSales, positionSales * sizeof(Sales), SEEK_SET);
            fread(&recordSale, sizeof(Sales), 1, fpSales);

            int numOfOrders = 0, totalNumOfPurchases = 0;
			long orderNumber = recordSale.OrderNumber;
            for(int index = positionSales, i = 0; recordSale.CustomerKey == customerKey && index < TellNumRecords("SalesTable", sizeof(Sales)); index += 1, i += 1){
				//printf("\norder number: %li, order date: %hu/%d/%d, productKey: %d, quantity: %d, currency code: %s\n", recordSale.OrderNumber, recordSale.OrderDate.AAAA, recordSale.OrderDate.MM, recordSale.OrderDate.DD, recordSale.ProductKey, recordSale.Quantity, recordSale.CurrencyCode);
                fseek(fpTemporalSales, i * sizeof(Sales), SEEK_SET);
				fwrite(&recordSale, sizeof(Sales), 1, fpTemporalSales);
				totalNumOfPurchases++;
                fseek(fpSales, (index + 1) * sizeof(Sales), SEEK_SET);
                fread(&recordSale, sizeof(Sales), 1, fpSales);
				if(orderNumber != recordSale.OrderNumber){
					numOfOrders++;
					orderNumber = recordSale.OrderNumber;
				}
            }

			if(typeofSort == 1){
				for(int step = 0; step < totalNumOfPurchases - 1; step += 1){
					Sales reg1, reg2;
					for(int i = 0; i < totalNumOfPurchases - step - 1; i += 1){
						fseek(fpTemporalSales, sizeof(Sales) * i, SEEK_SET);
						fread(&reg1, sizeof(Sales), 1, fpTemporalSales);

						fseek(fpTemporalSales, sizeof(Sales) * (i + 1), SEEK_SET);
						fread(&reg2, sizeof(Sales), 1, fpTemporalSales);
						int comparation = reg1.OrderNumber - reg2.OrderNumber;
						if(comparation == 0){
							comparation = reg1.ProductKey - reg2.ProductKey;
						}
						if(comparation > 0){
							fseek(fpTemporalSales, sizeof(Sales) * i, SEEK_SET);
							fwrite(&reg2, sizeof(Sales), 1, fpTemporalSales);

							fseek(fpTemporalSales, sizeof(Sales) * (i + 1), SEEK_SET);
							fwrite(&reg1, sizeof(Sales), 1, fpTemporalSales);
						}
					}
				}
			} else if(typeofSort == 2){
				MergeSortFile(fpTemporalSales, 0, totalNumOfPurchases - 1, sizeof(Sales), CompareSalesByOrderNumber);
			}

			
			Sales tempRecordSale1, tempRecordSale2;

			fseek(fpSales, positionSales * sizeof(Sales), SEEK_SET);
            fread(&recordSale, sizeof(Sales), 1, fpSales);
			
			int orderIndex = 0; 			// Inicializar el índice de órdenes
			float totalValue = 0.0;      	// Total acumulado para todas las órdenes

			for (int order = 0; order < numOfOrders; order += 1) {
			    // Leer el primer registro de la orden actual
			    fseek(fpTemporalSales, sizeof(Sales) * orderIndex, SEEK_SET);
			    fread(&tempRecordSale1, sizeof(Sales), 1, fpTemporalSales);

			    // Leer el siguiente registro para comparación
			    fseek(fpTemporalSales, sizeof(Sales) * (orderIndex + 1), SEEK_SET);
			    fread(&tempRecordSale2, sizeof(Sales), 1, fpTemporalSales);

			    float exchange = -1.0;

				// Buscar el tipo de cambio para esta orden
				int positionExchange = BinarySearchExchangeDate(fpExchangeRates, tempRecordSale1);
				if (positionExchange != -1){
					ExchangeRates staticExchangeRecord;
					fseek(fpExchangeRates, sizeof(ExchangeRates) * positionExchange, SEEK_SET);
					fread(&staticExchangeRecord, sizeof(ExchangeRates), 1, fpExchangeRates);
					int index = positionExchange;
					// printf("DATE:%s   currency:%s \n", staticExchangeRecord.Date, staticExchangeRecord.Currency);
					if (strcmp("USD", staticExchangeRecord.Currency) != 0){
						fseek(fpExchangeRates, sizeof(ExchangeRates) * positionExchange, SEEK_SET);
						fread(&recordExchange, sizeof(ExchangeRates), 1, fpExchangeRates);
						for (; strcmp("USD", recordExchange.Currency) != 0 && index > 0; index--){
							fseek(fpExchangeRates, sizeof(ExchangeRates) * index, SEEK_SET);
							fread(&recordExchange, sizeof(ExchangeRates), 1, fpExchangeRates);
						}
						index++;
					}

					// printf("date:%s   currency:%s" , recordExchange.Date , recordExchange.Currency );
					FILE *fpTemporalExchange = tmpfile();
					for (int i = 0; i < 5; i++, index++){
						fseek(fpExchangeRates, sizeof(ExchangeRates) * index, SEEK_SET);
						fread(&recordExchange, sizeof(ExchangeRates), 1, fpExchangeRates);
						fseek(fpTemporalExchange, sizeof(ExchangeRates) * i, SEEK_SET);
						fwrite(&recordExchange, sizeof(ExchangeRates), 1, fpExchangeRates);
						// printf("date:%s   currency:%s exchange:%f \n", recordExchange.Date, recordExchange.Currency, recordExchange.Exchange);
					}

					int indexTemoralExchange = -1;
					char currentCurrencyCode[3] = "";
					strcpy(currentCurrencyCode, tempRecordSale1.CurrencyCode);
					if (strcmp("USD", currentCurrencyCode) == 0){
						indexTemoralExchange = 0;
					}
					else if (strcmp("CAD", currentCurrencyCode) == 0){
						indexTemoralExchange = 1;
					}
					else if (strcmp("AUD", currentCurrencyCode) == 0){
						indexTemoralExchange = 2;
					}
					else if (strcmp("EUR", currentCurrencyCode) == 0){
						indexTemoralExchange = 3;
					}
					else if (strcmp("GBP", currentCurrencyCode) == 0){
						indexTemoralExchange = 4;
					}
					fseek(fpTemporalExchange, sizeof(ExchangeRates) * indexTemoralExchange, SEEK_SET);
					fread(&recordExchange, sizeof(ExchangeRates), 1, fpTemporalExchange);

					exchange = recordExchange.Exchange;

					fclose(fpTemporalExchange);
				}

			    // Mostrar encabezado de la orden
			    printf("\nOrder date: %hu/%u/%u\tOrder Number: %li\n", 
			           tempRecordSale1.OrderDate.AAAA, tempRecordSale1.OrderDate.MM, tempRecordSale1.OrderDate.DD, 
			           tempRecordSale1.OrderNumber);
				printf("%-17s%-100s%-15s%s %s", "ProductKey", "ProductName", "Quantity", "Value", tempRecordSale1.CurrencyCode);
			    printf("_________________________________________________________________________________________________________________________________________________________\n");

			    float subTotal = 0.0;
			    Products tempProductRecord;

			    // Iterar sobre todos los productos de la misma orden
			    while (orderIndex < totalNumOfPurchases - 1 && tempRecordSale1.OrderNumber == tempRecordSale2.OrderNumber) {
			        unsigned int productKey = tempRecordSale1.ProductKey;

			        // Buscar el producto en el archivo de productos
			        int positionProducts = BinarySearch(fpProducts, productKey, 1);
			        if (positionProducts != -1) {
			            fseek(fpProducts, sizeof(Products) * positionProducts, SEEK_SET);
			            fread(&tempProductRecord, sizeof(Products), 1, fpProducts);

			            // Calcular el valor del producto
			            float price = tempProductRecord.UnitPriceUSD * exchange * tempRecordSale1.Quantity;
			            printf("%-17hu%-100s%-15hu%.2f\n", tempProductRecord.ProductKey, tempProductRecord.ProductName, tempRecordSale1.Quantity, price);
			            subTotal += price;
			        }

			        // Avanzar al siguiente registro
			        orderIndex++;

			        // Actualizar los datos para la próxima iteración
			        fseek(fpTemporalSales, sizeof(Sales) * orderIndex, SEEK_SET);
			        fread(&tempRecordSale1, sizeof(Sales), 1, fpTemporalSales);

			        fseek(fpTemporalSales, sizeof(Sales) * (orderIndex + 1), SEEK_SET);
			        fread(&tempRecordSale2, sizeof(Sales), 1, fpTemporalSales);
			    }

			    // Procesar el último producto de la orden
			    unsigned int productKey = tempRecordSale1.ProductKey;

			    int positionProducts = BinarySearch(fpProducts, productKey, 1);
			    if (positionProducts != -1) {
			        fseek(fpProducts, sizeof(Products) * positionProducts, SEEK_SET);
			        fread(&tempProductRecord, sizeof(Products), 1, fpProducts);

			        float price = tempProductRecord.UnitPriceUSD * exchange * tempRecordSale1.Quantity;
			        printf("%-17hu%-100s%-15hu%.2f\n", tempProductRecord.ProductKey, tempProductRecord.ProductName, tempRecordSale1.Quantity, price);
			        subTotal += price;
			    }

			    orderIndex++; // Avanzar al siguiente índice
			    printf("_________________________________________________________________________________________________________________________________________________________\n");
				printf("%-117s%-15s%.2lf\n", "", "Subtotal", subTotal);
			    totalValue += subTotal;
			}

			// Mostrar el total acumulado
			printf("%-117s%-15s%.2lf\n", "", "TOTAL", totalValue);

		}
        printf("\n/--------------------------------------------------------------------------------------------------------------------------------------------------------/\n");
    	fclose(fpTemporalSales); fclose(fpExchangeRates);
	}
}


void BubbleSortOption5(){
	int numRecordsProducts = TellNumRecords("ProductsTable", sizeof(Products)); 	//Quantity of products in ProductsTable
	int numRecordsCustomers = TellNumRecords("CustomersTable", sizeof(Customers)); 	//Quantity of products in CustomersTable
	int numRecordsSales = TellNumRecords("SalesTable", sizeof(Sales)); 				//Quantity of products in SalesTable

	FILE *fpProducts = fopen("ProductsTable", "rb+");	//Pointer to ProductsTable
    FILE *fpCustomers = fopen("CustomersTable", "rb+");	//Pointer to CustomersTable
    FILE *fpSales = fopen("SalesTable", "rb+");			//Pointer to SalesTable	

	if (fpProducts == NULL){
		printf("Error opening the 'ProductsTable' File");
		return;
	}
	if (fpCustomers == NULL){
		printf("Error opening the 'CustomersTable' File");
		return;
	}
	if (fpSales == NULL){
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


	for (int step = 0; step < numRecordsCustomers - 1; step += 1) {
		Customers reg1, reg2;
        printf("Ordena Customers: %i\n", step + 1);
        for (int i = 0; i < numRecordsCustomers - step - 1; i += 1) {
            fseek(fpCustomers, sizeof(Customers) * i, SEEK_SET);
            fread(&reg1, sizeof(Customers), 1, fpCustomers);

            fseek(fpCustomers, sizeof(Customers) * (i + 1), SEEK_SET);
            fread(&reg2, sizeof(Customers), 1, fpCustomers);

            if (strcmp(reg1.Name, reg2.Name) > 0) {
                // Intercambiar los registros si están fuera de orden
                fseek(fpCustomers, sizeof(Customers) * i, SEEK_SET);
                fwrite(&reg2, sizeof(Customers), 1, fpCustomers);

                fseek(fpCustomers, sizeof(Customers) * (i + 1), SEEK_SET);
                fwrite(&reg1, sizeof(Customers), 1, fpCustomers);
            }
        }
    }

	
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

	ShowCustomersPurchases(fpSales, fpCustomers, fpProducts, numRecordsCustomers, 1);

    fclose(fpProducts);
    fclose(fpCustomers);
    fclose(fpSales);
}

void MergeSortOption5(){
	int numRecordsCustomers = TellNumRecords("CustomersTable", sizeof(Customers)); 	//Quantity of products in CustomersTable
	int numRecordsSales = TellNumRecords("SalesTable", sizeof(Sales)); 				//Quantity of products in SalesTable
	int numRecordsProducts = TellNumRecords("ProductsTable", sizeof(Sales)); 		//Quantity of products in ProductsTable


    FILE *fpCustomers = fopen("CustomersTable", "rb+");	//Pointer to CustomersTable
    FILE *fpSales = fopen("SalesTable", "rb+");			//Pointer to SalesTable	
    FILE *fpProducts = fopen("ProductsTable", "rb+");

	if(fpProducts == NULL){
		printf("Error opening the 'ProductsTable' File");
		return;
	}
	if (fpCustomers == NULL){
		printf("Error opening the 'CustomersTable' File");
		return;
	}
	if (fpSales == NULL){
		printf("Error opening the 'SalesTable' File");
		return;
	}


	MergeSortFile(fpCustomers, 0, numRecordsCustomers - 1, sizeof(Customers), CompareCustomersByName);

	MergeSortFile(fpSales, 0, numRecordsSales - 1, sizeof(Sales), CompareSalesByCustomerKey);

	MergeSortFile(fpProducts, 0, numRecordsProducts - 1, sizeof(Products), CompareProductsByProductKey); //To MergeSortArray the ProductsTable File


	ShowCustomersPurchases(fpSales, fpCustomers, fpProducts, numRecordsCustomers, 2);

    fclose(fpCustomers);
    fclose(fpSales);
    fclose(fpProducts);
}
