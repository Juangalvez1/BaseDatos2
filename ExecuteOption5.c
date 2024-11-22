#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Functions.h"

/* 
    Function: ShowCustomersPurchases
    Purpose: Displays the purchase history for each customer, including order details, products bought, quantities, and total prices.
    The purchases are sorted based on the specified sorting method and include currency conversion.
    Parameters:
      - fpSales: File pointer to the SalesTable, containing sales data.
      - fpCustomers: File pointer to the CustomersTable, containing customer data.
      - fpProducts: File pointer to the ProductsTable, containing product details.
      - numOfCustomers: Total number of customers to analyze.
      - typeofSort: Specifies the sorting method (1 for BubbleSort, 2 for MergeSort).
    Returns: None.
*/
void ShowCustomersPurchases(FILE *fpSales, FILE *fpCustomers, FILE *fpProducts, int numOfCustomers, int typeofSort){
	//printf("%i", typeofSort);
	
	Sales recordSale;				//Used to store a record of SalesTable and get its information
	Customers recordCustomer;		//Used to store a record of CustomersTable and store it temporarely in TemporalFileOption2
	ExchangeRates recordExchange;
	
	FILE *fpExchangeRates = NULL;
	FILE *fpTemporalSales = NULL;

    char customerName[40] = ""; 		//Used to store the ProductName of each Product in ProductsTable
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
					//printf("DATE:%s   currency:%s \n", staticExchangeRecord.Date, staticExchangeRecord.Currency);
					if (strcmp("USD", staticExchangeRecord.Currency) != 0){
						fseek(fpExchangeRates, sizeof(ExchangeRates) * positionExchange, SEEK_SET);
						fread(&recordExchange, sizeof(ExchangeRates), 1, fpExchangeRates);
						for (; strcmp("USD", recordExchange.Currency) != 0 && index > 0; index--){
							fseek(fpExchangeRates, sizeof(ExchangeRates) * index, SEEK_SET);
							fread(&recordExchange, sizeof(ExchangeRates), 1, fpExchangeRates);
						}
						index++;
					}

					//printf("\tdate:%s   currency:%s\n" , recordExchange.Date , recordExchange.Currency );
					FILE *fpTemporalExchange = tmpfile();
					for (int i = 0; i < 5; i++, index++){
						fseek(fpExchangeRates, sizeof(ExchangeRates) * index, SEEK_SET);
						fread(&recordExchange, sizeof(ExchangeRates), 1, fpExchangeRates);
						fseek(fpTemporalExchange, sizeof(ExchangeRates) * i, SEEK_SET);
						fwrite(&recordExchange, sizeof(ExchangeRates), 1, fpTemporalExchange);
						//printf("date:%s   currency:%s exchange:%f \n", recordExchange.Date, recordExchange.Currency, recordExchange.Exchange);
					}

					int indexTemoralExchange = -1;
					if (tempRecordSale1.CurrencyCode[0] == 'U'){
						indexTemoralExchange = 0;
					}
					else if (tempRecordSale1.CurrencyCode[0] == 'C'){
						indexTemoralExchange = 1;
					}
					else if (tempRecordSale1.CurrencyCode[0] == 'A'){
						indexTemoralExchange = 2;
					}
					else if (tempRecordSale1.CurrencyCode[0] == 'E'){
						indexTemoralExchange = 3;
					}
					else if (tempRecordSale1.CurrencyCode[0] == 'G'){
						indexTemoralExchange = 4;
					}
					//printf ("\n\t indextemporal : %i" , indexTemoralExchange);
					fseek(fpTemporalExchange, sizeof(ExchangeRates) * indexTemoralExchange, SEEK_SET);
					fread(&recordExchange, sizeof(ExchangeRates), 1, fpTemporalExchange);
                
					exchange = recordExchange.Exchange;
					//printf("\n fecha : %s     exchange: %f\n", recordExchange.Date ,  exchange);
                   
					fclose(fpTemporalExchange);
				}

			    // Mostrar encabezado de la orden
			    printf("\nOrder date: %hu/%u/%u\tOrder Number: %li\n", 
			           tempRecordSale1.OrderDate.AAAA, tempRecordSale1.OrderDate.MM, tempRecordSale1.OrderDate.DD, 
			           tempRecordSale1.OrderNumber);
				printf("%-17s%-100s%-15s%s %s", "ProductKey", "ProductName", "Quantity", "Value", tempRecordSale1.CurrencyCode);
			    printf("\n_________________________________________________________________________________________________________________________________________________________\n");

			    float subTotal = 0.0;
			    Products tempProductRecord;

			    // Iterar sobre todos los productos de la misma orden
			    while (orderIndex < totalNumOfPurchases - 1 && tempRecordSale1.OrderNumber == tempRecordSale2.OrderNumber) {
			        unsigned int productKey = tempRecordSale1.ProductKey;

			        // Buscar el producto en el archivo de productos
			        int positionProducts = BinarySearch(fpProducts, productKey, 1);
					printf("Salio binary.\n");
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


/* 
    Function: BubbleSortOption5
    Purpose: Sorts the ProductsTable, CustomersTable, and SalesTable using BubbleSort based on specific fields (ProductKey, Customer Name, and CustomerKey). 
    It then displays customer purchase details.
    Parameters: None.
    Returns: None.
*/
void BubbleSortOption5() {
    // Get the number of records in each table.
    int numRecordsProducts = TellNumRecords("ProductsTable", sizeof(Products)); // Number of products in ProductsTable.
    int numRecordsCustomers = TellNumRecords("CustomersTable", sizeof(Customers)); // Number of customers in CustomersTable.
    int numRecordsSales = TellNumRecords("SalesTable", sizeof(Sales)); // Number of sales in SalesTable.

    // Open the respective files for reading and writing.
    FILE *fpProducts = fopen("ProductsTable", "rb+");  // Pointer to ProductsTable.
    FILE *fpCustomers = fopen("CustomersTable", "rb+"); // Pointer to CustomersTable.
    FILE *fpSales = fopen("SalesTable", "rb+");  // Pointer to SalesTable.

    // Check if files are opened successfully.
    if (fpProducts == NULL) {
        printf("Error opening the 'ProductsTable' File");
        return;
    }
    if (fpCustomers == NULL) {
        printf("Error opening the 'CustomersTable' File");
        return;
    }
    if (fpSales == NULL) {
        printf("Error opening the 'SalesTable' File");
        return;
    }

    // Perform BubbleSort on ProductsTable based on ProductKey.
    for (int step = 0; step < numRecordsProducts - 1; step++) {
        Products reg1, reg2;
        printf("Sorting Products: %i\n", step + 1);
        for (int i = 0; i < numRecordsProducts - step - 1; i++) {
            fseek(fpProducts, sizeof(Products) * i, SEEK_SET);  // Move to the i-th product.
            fread(&reg1, sizeof(Products), 1, fpProducts);  // Read the i-th product.

            fseek(fpProducts, sizeof(Products) * (i + 1), SEEK_SET);  // Move to the next product.
            fread(&reg2, sizeof(Products), 1, fpProducts);  // Read the (i+1)-th product.

            // Compare and swap if needed.
            if (reg1.ProductKey > reg2.ProductKey) {
                fseek(fpProducts, sizeof(Products) * i, SEEK_SET);
                fwrite(&reg2, sizeof(Products), 1, fpProducts);
                fseek(fpProducts, sizeof(Products) * (i + 1), SEEK_SET);
                fwrite(&reg1, sizeof(Products), 1, fpProducts);
            }
        }
    }

    // Perform BubbleSort on CustomersTable based on customer names.
    for (int step = 0; step < numRecordsCustomers - 1; step++) {
        Customers reg1, reg2;
        printf("Sorting Customers: %i\n", step + 1);
        for (int i = 0; i < numRecordsCustomers - step - 1; i++) {
            fseek(fpCustomers, sizeof(Customers) * i, SEEK_SET);  // Move to the i-th customer.
            fread(&reg1, sizeof(Customers), 1, fpCustomers);  // Read the i-th customer.

            fseek(fpCustomers, sizeof(Customers) * (i + 1), SEEK_SET);  // Move to the next customer.
            fread(&reg2, sizeof(Customers), 1, fpCustomers);  // Read the (i+1)-th customer.

            // Compare and swap if needed.
            if (strcmp(reg1.Name, reg2.Name) > 0) {
                fseek(fpCustomers, sizeof(Customers) * i, SEEK_SET);
                fwrite(&reg2, sizeof(Customers), 1, fpCustomers);
                fseek(fpCustomers, sizeof(Customers) * (i + 1), SEEK_SET);
                fwrite(&reg1, sizeof(Customers), 1, fpCustomers);
            }
        }
    }

    // Perform BubbleSort on SalesTable based on CustomerKey.
    for (int step = 0; step < numRecordsSales - 1; step++) {
        printf("Sorting Sales: %i\n", step + 1);
        Sales reg1, reg2;
        for (int i = 0; i < numRecordsSales - step - 1; i++) {
            fseek(fpSales, sizeof(Sales) * i, SEEK_SET);  // Move to the i-th sale.
            fread(&reg1, sizeof(Sales), 1, fpSales);  // Read the i-th sale.

            fseek(fpSales, sizeof(Sales) * (i + 1), SEEK_SET);  // Move to the next sale.
            fread(&reg2, sizeof(Sales), 1, fpSales);  // Read the (i+1)-th sale.

            // Compare and swap if needed.
            if (reg1.CustomerKey > reg2.CustomerKey) {
                fseek(fpSales, sizeof(Sales) * i, SEEK_SET);
                fwrite(&reg2, sizeof(Sales), 1, fpSales);
                fseek(fpSales, sizeof(Sales) * (i + 1), SEEK_SET);
                fwrite(&reg1, sizeof(Sales), 1, fpSales);
            }
        }
    }

    // Display the sorted customer purchase details.
    ShowCustomersPurchases(fpSales, fpCustomers, fpProducts, numRecordsCustomers, 1);

    // Close the file pointers after the operation is completed.
    fclose(fpProducts);
    fclose(fpCustomers);
    fclose(fpSales);
}

/* 
    Function: MergeSortOption5
    Purpose: Sorts the ProductsTable, CustomersTable, and SalesTable using MergeSort based on specific fields (ProductKey, Customer Name, and CustomerKey). 
    It then displays customer purchase details.
    Parameters: None.
    Returns: None.
*/
void MergeSortOption5() {
    // Get the number of records in each table.
    int numRecordsCustomers = TellNumRecords("CustomersTable", sizeof(Customers)); // Number of customers in CustomersTable.
    int numRecordsSales = TellNumRecords("SalesTable", sizeof(Sales)); // Number of sales in SalesTable.
    int numRecordsProducts = TellNumRecords("ProductsTable", sizeof(Products)); // Number of products in ProductsTable.

    // Open the respective files for reading and writing.
    FILE *fpCustomers = fopen("CustomersTable", "rb+"); // Pointer to CustomersTable.
    FILE *fpSales = fopen("SalesTable", "rb+"); // Pointer to SalesTable.
    FILE *fpProducts = fopen("ProductsTable", "rb+"); // Pointer to ProductsTable.

    // Check if files are opened successfully.
    if (fpProducts == NULL) {
        printf("Error opening the 'ProductsTable' File");
        return;
    }
    if (fpCustomers == NULL) {
        printf("Error opening the 'CustomersTable' File");
        return;
    }
    if (fpSales == NULL) {
        printf("Error opening the 'SalesTable' File");
        return;
    }

    // Perform MergeSort on CustomersTable based on Name.
    MergeSortFile(fpCustomers, 0, numRecordsCustomers - 1, sizeof(Customers), CompareCustomersByName);

    // Perform MergeSort on SalesTable based on CustomerKey.
    MergeSortFile(fpSales, 0, numRecordsSales - 1, sizeof(Sales), CompareSalesByCustomerKey);

    // Perform MergeSort on ProductsTable based on ProductKey.
    MergeSortFile(fpProducts, 0, numRecordsProducts - 1, sizeof(Products), CompareProductsByProductKey);

    // Display the sorted customer purchase details.
    ShowCustomersPurchases(fpSales, fpCustomers, fpProducts, numRecordsCustomers, 2);

    // Close the file pointers after the operation is completed.
    fclose(fpCustomers);
    fclose(fpSales);
    fclose(fpProducts);
}
