#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Functions.h"

typedef struct{
	unsigned short int ProductKey;
	char ProductName[100];
	unsigned short int Quantity;
	float UnitPriceUSD;
} ProductsList;


void ShowCustomersPurchases(FILE *fpSales, FILE *fpCustomers, FILE *fpProducts, int numOfCustomers, int typeofSort){
	//printf("%i", typeofSort);
	
	Sales recordSale;				//Used to store a record of SalesTable and get its information
	Customers recordCustomer;		//Used to store a record of CustomersTable and store it temporarely in TemporalFileOption2
	ExchangeRates recordExchange;	//Used to store a record or ExchangeRatesTabke abd get its information
	
	FILE *fpExchangeRates = NULL;
	FILE *fpTemporalSales = NULL;

    char customerName[40] = ""; 						//Used to store the ProductName of each Product in ProductsTable
	unsigned int customerKey = 0; 						//Used to store the ProductKey of each Product in ProductTable

    for(int i = 0; i < numOfCustomers && i < 5; i += 1){
        fseek(fpCustomers, sizeof(Customers) * i, SEEK_SET);
    	fread(&recordCustomer, sizeof(Customers), 1, fpCustomers);

        strcpy(customerName, recordCustomer.Name);
        printf("\nCustomer name: %-40s", customerName);

        customerKey = recordCustomer.CustomerKey;
		printf("%u\n", customerKey);
        int positionSales = BinarySearch(fpSales, customerKey, 4);
        if(positionSales == -1){
            printf(" - No purchases reported\n");
        } else {
			fpTemporalSales = fopen("TemporalFileSalesOption5", "rb+");
			fpExchangeRates = fopen("ExchangeRatesTable", "rb+");

            if (fpTemporalSales == NULL) {
			    printf("Error abriendo archivo temporal Sales.\n");
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
			
			/*
			for(int order = 0; order < numOfOrders; order += 1){
				FILE *fpTemporalProductsList = fopen("TemporalFileProductsListOption5", "wb+");
				
				float exchange = -1.;

				char orderDate[11] = "", tempDate[5] = "";
				sprintf(tempDate, "%d", recordSale.OrderDate.MM);
				strcat(orderDate, tempDate);
				strcat(orderDate, "/");
				sprintf(tempDate, "%d", recordSale.OrderDate.DD);
				strcat(orderDate, tempDate);
				strcat(orderDate, "/");
				sprintf(tempDate, "%d", recordSale.OrderDate.AAAA);
				strcat(orderDate, tempDate);
				//printf("fecha de la orden para buscar en exchange: %s, fecha real %d/%d/%d\n", orderDate, recordSale.OrderDate.MM, recordSale.OrderDate.DD, recordSale.OrderDate.AAAA);
				int positionExchange = BinarySearchExchangeDate(fpExchangeRates, recordSale);
				//printf("\nposition Exchange: %i\n", positionExchange);
				if(positionExchange != -1){
					fseek(fpExchangeRates, sizeof(ExchangeRates) * positionExchange, SEEK_SET);
					fread(&recordExchange, sizeof(ExchangeRates), 1, fpExchangeRates);
					exchange = recordExchange.Exchange;
					//printf("\n\tfecha: %s\tExchange: %f\t'Exchange': %f\n", recordExchange.Date, recordExchange.Exchange, exchange);
				}
				
				long int orderNumber = recordSale.OrderNumber;
				
				printf("\nOrder date: %hu/%u/%u\tOrder Number: %li\n", recordSale.OrderDate.AAAA, recordSale.OrderDate.MM, recordSale.OrderDate.DD, recordSale.OrderNumber);
                printf("%-23s %-100s %-15s %s %s", "ProductKey", "ProductName", "Quantity", "Value", recordSale.CurrencyCode);
                printf("\n_________________________________________________________________________________________________________________________________________________________\n");

				double subTotal = 0;
				int numOfPurchases = 0;
				do{
					//printf("\ntempIndexSale: %i\n", tempSaleIndex);
					fseek(fpTemporalSales, sizeof(Sales) * tempSaleIndex, SEEK_SET);
					fread(&tempRecordSale, sizeof(Sales), 1, fpTemporalSales);
					tempSaleIndex++;
					//printf("lo que cogio el tempIndex:\norder number: %li, order date: %hu/%d/%d, productKey: %d, quantity: %d, currency code: %s", tempRecordSale.OrderNumber, tempRecordSale.OrderDate.AAAA, tempRecordSale.OrderDate.MM, tempRecordSale.OrderDate.DD, tempRecordSale.ProductKey, tempRecordSale.Quantity, tempRecordSale.CurrencyCode);
					productKey = tempRecordSale.ProductKey;

					int positionProducts = BinarySearch(fpProducts, productKey, 1);

					if(positionProducts != -1 && orderNumber == tempRecordSale.OrderNumber){
						numOfPurchases++;
						fseek(fpProducts, positionProducts * sizeof(Products), SEEK_SET);
						fread(&recordProduct, sizeof(Products), 1, fpProducts);

						//printf("\tproductkey: %d, quantity: %d, unit price: %.2f", recordProduct.ProductKey, tempRecordSale.Quantity, recordProduct.UnitPriceUSD);

						tempProductsRecord.ProductKey = recordProduct.ProductKey;
						strcpy(tempProductsRecord.ProductName, recordProduct.ProductName);
						tempProductsRecord.Quantity = tempRecordSale.Quantity;
						tempProductsRecord.UnitPriceUSD = recordProduct.UnitPriceUSD;

						fseek(fpTemporalProductsList, productIndex * sizeof(ProductsList), SEEK_SET);
						fwrite(&tempProductsRecord, sizeof(ProductsList), 1, fpTemporalProductsList);
						productIndex++;
					}
					//printf("\torder number: %li\n", tempRecordSale.OrderNumber);
					
				} while(orderNumber == tempRecordSale.OrderNumber && numOfPurchases < totalNumOfPurchases);
				
				for(int i = 0; i < numOfPurchases; i += 1){
					fseek(fpTemporalProductsList, sizeof(ProductsList) * i, SEEK_SET);
					fread(&tempProductsRecord, sizeof(ProductsList), 1, fpTemporalProductsList);
					float price = tempProductsRecord.UnitPriceUSD * exchange;
					printf("%hu\t\t\t%-105s%hu%-11s%.2f\n", tempProductsRecord.ProductKey, tempProductsRecord.ProductName, tempProductsRecord.Quantity, "", price);
					subTotal += price;
				}
				printf("_________________________________________________________________________________________________________________________________________________________\n");
				printf("%-125s%-16s%.2lf\n", "", "Subtotal", subTotal);
				totalValue += subTotal;
				fclose(fpTemporalProductsList);
				//fseek(fpTemporalSales, sizeof(Sales) * tempSaleIndex, SEEK_SET);
				//fread(&recordSale, sizeof(Sales), 1, fpTemporalSales);
			}
			*/

			int orderIndex = 0; // Inicializar el índice de órdenes
			float totalValue = 0.0;      // Total acumulado para todas las órdenes

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
			    if (positionExchange != -1) {
			        fseek(fpExchangeRates, sizeof(ExchangeRates) * positionExchange, SEEK_SET);
			        fread(&recordExchange, sizeof(ExchangeRates), 1, fpExchangeRates);
			        exchange = recordExchange.Exchange;
			    }

			    // Mostrar encabezado de la orden
			    printf("\nOrder date: %hu/%u/%u\tOrder Number: %li\n", 
			           tempRecordSale1.OrderDate.AAAA, tempRecordSale1.OrderDate.MM, tempRecordSale1.OrderDate.DD, 
			           tempRecordSale1.OrderNumber);
				printf("%-17s%-100s%-15s%s %s", "ProductKey", "ProductName", "Quantity", "Value", tempRecordSale1.CurrencyCode);
			    printf("_________________________________________________________________________________________________________________________________________________________\n");

			    double subTotal = 0.0;
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
	int numRecordsCustomers = TellNumRecords("CustomersTable", sizeof(Customers)); 	//Quantity of products in CustomersTable
	int numRecordsSales = TellNumRecords("SalesTable", sizeof(Sales)); 				//Quantity of products in SalesTable
    int numRecordsProducts = TellNumRecords("ProductsTable", sizeof(Sales)); 		//Quantity of products in ProductsTable

    FILE *fpCustomers = fopen("CustomersTable", "rb+");	//Pointer to CustomersTable
    FILE *fpSales = fopen("SalesTable", "rb+");			//Pointer to SalesTable	
    FILE *fpProducts = fopen("ProductsTable", "rb+");

	if (fpCustomers == NULL){
		printf("Error opening the 'CustomersTable' File");
		return;
	}
	if (fpSales == NULL){
		printf("Error opening the 'SalesTable' File");
		return;
	}

	Customers *recordsCustomers = (Customers *) calloc(numRecordsCustomers, sizeof(Customers));
	for(int i = 0; i < numRecordsCustomers; i += 1){
		//printf("\nArray Products %i", i + 1);
		fseek(fpCustomers, sizeof(Customers) * i, SEEK_SET);
		fread(&recordsCustomers[i], sizeof(Customers), 1, fpCustomers);
	}

	for (int step = 0; step < numRecordsCustomers - 1; step += 1){
		for (int i = 0; i < numRecordsCustomers - step - 1; i += 1){
			if(strcmp(recordsCustomers[i].Name, recordsCustomers[i + 1].Name) > 0){
				Customers temp = recordsCustomers[i];
				recordsCustomers[i] = recordsCustomers[i + 1];
				recordsCustomers[i + 1] = temp;
			}
		}
	}

	for (int i = 0; i < numRecordsCustomers; i += 1){
		//printf("\nArchivo Customers %i", i + 1);
		fseek(fpCustomers, sizeof(Customers) * i, SEEK_SET);
		fwrite(&recordsCustomers[i], sizeof(Customers), 1, fpCustomers);
	}
	//printf("\nHizo customers\n");

	Sales *recordsSales = (Sales *) calloc(numRecordsSales, sizeof(Sales));
	for(int i = 0; i < numRecordsSales; i += 1){
		//printf("\nArray Sales %i", i + 1);
		fseek(fpSales, sizeof(Sales) * i, SEEK_SET);
		fread(&recordsSales[i], sizeof(Sales), 1, fpSales);
	}
	
	for (int step = 0; step < numRecordsSales - 1; step += 1){
		for (int i = 0; i < numRecordsSales - step - 1; i += 1){
			if(recordsSales[i].CustomerKey > recordsSales[i + 1].CustomerKey){
				Sales temp = recordsSales[i];
				recordsSales[i] = recordsSales[i + 1];
				recordsSales[i + 1] = temp;
			}
		}
	}

	for (int i = 0; i < numRecordsSales; i += 1){
		//printf("\nArchivo Sales %i", i + 1);
		fseek(fpSales, sizeof(Sales) * i, SEEK_SET);
		fwrite(&recordsSales[i], sizeof(Sales), 1, fpSales);
	}
	//printf("\nHizo sales\n");

    Products *recordsProducts = (Products *) calloc(numRecordsProducts, sizeof(Products));
	for(int i = 0; i < numRecordsProducts; i += 1){
		//printf("\nArray Products %i", i + 1);
		fseek(fpProducts, sizeof(Products) * i, SEEK_SET);
		fread(&recordsProducts[i], sizeof(Products), 1, fpProducts);
	}

	for (int step = 0; step < numRecordsProducts - 1; step += 1){
		for (int i = 0; i < numRecordsProducts - step - 1; i += 1){
			if(recordsProducts[i].ProductKey > recordsProducts[i + 1].ProductKey){
				Products temp = recordsProducts[i];
				recordsProducts[i] = recordsProducts[i + 1];
				recordsProducts[i + 1] = temp;
			}
		}
	}

	for (int i = 0; i < numRecordsProducts; i += 1){
		//printf("\nArchivo Products %i", i + 1);
		fseek(fpProducts, sizeof(Products) * i, SEEK_SET);
		fwrite(&recordsProducts[i], sizeof(Products), 1, fpProducts);
	}

	free(recordsCustomers); recordsCustomers = NULL;	//Free the diamic memory where recordsCustomers where stored
	free(recordsSales); recordsSales = NULL;			//Free the diamic memory where recordsSales where stored
    free(recordsProducts); recordsProducts = NULL;			//Free the diamic memory where recordsProducts where stored

    ShowCustomersPurchases(fpSales, fpCustomers, fpProducts, numRecordsCustomers, 1);

    fclose(fpCustomers);
    fclose(fpSales);
    fclose(fpProducts);
}

void MergeSortOption5(){
	int numRecordsCustomers = TellNumRecords("CustomersTable", sizeof(Customers)); 	//Quantity of products in CustomersTable
	int numRecordsSales = TellNumRecords("SalesTable", sizeof(Sales)); 				//Quantity of products in SalesTable
	int numRecordsProducts = TellNumRecords("ProductsTable", sizeof(Sales)); 		//Quantity of products in ProductsTable


    FILE *fpCustomers = fopen("CustomersTable", "rb+");	//Pointer to CustomersTable
    FILE *fpSales = fopen("SalesTable", "rb+");			//Pointer to SalesTable	
    FILE *fpProducts = fopen("ProductsTable", "rb+");

	if (fpCustomers == NULL){
		printf("Error opening the 'CustomersTable' File");
		return;
	}
	if (fpSales == NULL){
		printf("Error opening the 'SalesTable' File");
		return;
	}

	Customers *recordsCustomers = (Customers *) calloc(numRecordsCustomers, sizeof(Customers));
	for(int i = 0; i < numRecordsCustomers; i += 1){
		//printf("\nArray Products %i", i + 1);
		fseek(fpCustomers, sizeof(Customers) * i, SEEK_SET);
		fread(&recordsCustomers[i], sizeof(Customers), 1, fpCustomers);
	}

	MergeSortArray(recordsCustomers, 0, numRecordsCustomers - 1, sizeof(Customers), CompareCustomersByName);

	for (int i = 0; i < numRecordsCustomers; i += 1){
		//printf("\nArchivo Customers %i", i + 1);
		fseek(fpCustomers, sizeof(Customers) * i, SEEK_SET);
		fwrite(&recordsCustomers[i], sizeof(Customers), 1, fpCustomers);
	}
	//printf("\nHizo customers\n");

	Sales *recordsSales = (Sales *) calloc(numRecordsSales, sizeof(Sales));
	for(int i = 0; i < numRecordsSales; i += 1){
		//printf("\nArray Sales %i", i + 1);
		fseek(fpSales, sizeof(Sales) * i, SEEK_SET);
		fread(&recordsSales[i], sizeof(Sales), 1, fpSales);
	}
	
	MergeSortArray(recordsSales, 0, numRecordsSales - 1, sizeof(Sales), CompareSalesByCustomerKey);

	for (int i = 0; i < numRecordsSales; i += 1){
		//printf("\nArchivo Sales %i", i + 1);
		fseek(fpSales, sizeof(Sales) * i, SEEK_SET);
		fwrite(&recordsSales[i], sizeof(Sales), 1, fpSales);
	}
	//printf("\nHizo sales\n");

	Products *recordsProducts = (Products *) calloc(numRecordsProducts, sizeof(Products));
	//For to store the records of ProductsTable in the "array" of Products
	for(int i = 0; i < numRecordsProducts; i += 1){
		//printf("\nArray Products %i", i + 1);
		fseek(fpProducts, sizeof(Products) * i, SEEK_SET);
		fread(&recordsProducts[i], sizeof(Products), 1, fpProducts);
	}

	MergeSortArray(recordsProducts, 0, numRecordsProducts - 1, sizeof(Products), CompareProductsByProductKey); //To MergeSortArray the ProductsTable File

	for (int i = 0; i < numRecordsProducts; i += 1){
		//printf("\nArchivo Products %i", i + 1);
		fseek(fpProducts, sizeof(Products) * i, SEEK_SET);
		fwrite(&recordsProducts[i], sizeof(Products), 1, fpProducts);
	}
	//printf("\nHizo products\n");


	free(recordsCustomers); recordsCustomers = NULL;	//Free the diamic memory where recordsCustomers where stored
	free(recordsSales); recordsSales = NULL;			//Free the diamic memory where recordsSales where stored

	ShowCustomersPurchases(fpSales, fpCustomers, fpProducts, numRecordsCustomers, 2);

    fclose(fpCustomers);
    fclose(fpSales);
    fclose(fpProducts);
}
