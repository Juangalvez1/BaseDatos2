#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Functions.h"

void DeterminateCustomersLocation(FILE *fpProducts, FILE *fpSales, FILE *fpCustomers, int numOfProducts, int typeofSort){
  	Products recordProduct;		//Used to store a record of ProductsTable and get its information
	Customers recordCustomer;	//Used to store a record of CustomersTable and store it temporarely in TemporalFileOption2
	Sales recordSale;			//Used to store a record of SalesTable and get its information

	char productName[100] = ""; 		//Used to store the ProductName of each Product in ProductsTable
	unsigned short int productKey = 0; 	//Used to store the ProductKey of each Product in ProductTable
	unsigned int customerKey = 0;		//Used to store the Customer key of a customer which has bougth a Product

	//For to loop all of the Products and search if they have any sales reported
	for(int i = 0; i < numOfProducts; i++){
		//Reading and storage of the Product[i] in ProductsTable
    	fseek(fpProducts, sizeof(Products) * i, SEEK_SET);
    	fread(&recordProduct, sizeof(recordProduct), 1, fpProducts);

		//Copy into productName the ProductName of each product and display it
    	strcpy(productName, recordProduct.ProductName);
    	printf("\n%s", productName);

		//Getting the current ProductKey to do the searchs and comparations
		productKey = recordProduct.ProductKey;

		//Getting the "Position" of a sale with the current productKey, it may not be the first one
    	int positionSales = BinarySearch(fpSales, productKey, 3);

    	if( positionSales == -1){
    		printf(" - No sales reported\n");
    	} else {
			//TemporalFile, will be used for each product to store te record of the customers who have bougth it
			FILE *fpTemporal = fopen("TemporalFileOption2", "rb+");
			if (fpTemporal == NULL) {
			    printf("Error abriendo archivo temporal.\n");
			    return;
			}

    	    printf("\n%-35s %-35s %-35s %-35s\n", "Continent", "Country", "State", "City");
    	    printf("_______________________________________________________________________________________________________________________________________\n");

			//Reading the record in sales with the current product key
    	    fseek(fpSales, sizeof(Sales) * (positionSales - 1), SEEK_SET);
    	    fread(&recordSale, sizeof(Sales), 1, fpSales);

			//for to loop and check if the recordSale[positionSales] is the first record in sales with the currrent productkey
    	    for( int i = positionSales - 1; i >= 0 && productKey == recordSale.ProductKey; i -= 1){
				//Reading of the previous record in sales to verify if its the first one with the currenr productKey
    	    	fseek(fpSales, sizeof(Sales) * (i - 1), SEEK_SET);
    	    	fread(&recordSale, sizeof(Sales), 1, fpSales);

    	    	positionSales = i; //Changing positionSales to be the index of first record in sales with the current productKey
    	    }

			//Reading the first record in sales with the current ProductKey
			fseek(fpSales, sizeof(Sales) * positionSales, SEEK_SET);
			fread(&recordSale, sizeof(Sales), 1, fpSales);

			/*
			index: will be the positionSales, to get each sale with the current productKey
			numOfBuyers: num of people that has bougth the current product, will be the amount of records in TemporalFileOption2
			positionCustomers: index of CustomersTable of the customerKey of each customer that has bougth the current product 
			*/
			int index = positionSales, numOfBuyers = 0, positionCustomers = 0;

			//While to store each customer that has bougth the current product into TemporalFileOption2
			//It will loop till the productKey its a different one or it reached the end of the SalesTable file
    	    while (recordSale.ProductKey == productKey && index < TellNumRecords("SalesTable", sizeof(Sales))){
				customerKey = recordSale.CustomerKey;

				//Getting the position in CustomersTable of each customer that has bougth the current product
				positionCustomers = BinarySearch(fpCustomers, customerKey, 2);

				if (positionCustomers != -1) {
					//Reading the record[positionCustomers] 
                    fseek(fpCustomers, sizeof(Customers) * positionCustomers, SEEK_SET);
                    fread(&recordCustomer, sizeof(Customers), 1, fpCustomers);
					//writing the recordCsutomer in the TemporalFileOption2
                    fwrite(&recordCustomer, sizeof(Customers), 1, fpTemporal);
                    numOfBuyers++;
                }
				//Reading the next recordSale to veridy if its about the same product
				fseek(fpSales, sizeof(Sales) * (index + 1), SEEK_SET);
				fread(&recordSale, sizeof(Sales), 1, fpSales);

				index++;
			}
			//Creating the pointer to the part of tha dinamic memory where the records of TemporalFileOption2 will be temporarely saved
			Customers *recordsCustomers = (Customers *) calloc(numOfBuyers, sizeof(Customers));

			//For to store the records of CustomersTable in the "array" of Customers
			for(int i = 0; i < numOfBuyers; i += 1){
				//printf("\nArray Customers %i", i + 1);
				fseek(fpTemporal, sizeof(Customers) * i, SEEK_SET);
				fread(&recordsCustomers[i], sizeof(Customers), 1, fpTemporal);
			}
			
			if(typeofSort == 1){ //Executing option 2.1
				for(int step = 0; step < numOfBuyers - 1; step += 1){
					for(int i = 0; i < numOfBuyers - step - 1; i += 1){
						if(CompareCustomersByCustomerLocation(&recordsCustomers[i], &recordsCustomers[i + 1]) > 0){
							Customers temp = recordsCustomers[i];
                            recordsCustomers[i] = recordsCustomers[i + 1];
                            recordsCustomers[i + 1] = temp;
						}
					}
				}
			} else if (typeofSort == 2){ //Executing option 2.2
				MergeSortArray(recordsCustomers, 0, numOfBuyers - 1, sizeof(Customers), CompareCustomersByCustomerLocation);
			}
		
			//For to write each record alredy ordered in the file CustomersTable
			for (int i = 0; i < numOfBuyers; i += 1){
				//printf("\nArchivo Customers %i", i + 1);
				fseek(fpTemporal, sizeof(Customers) * i, SEEK_SET);
				fwrite(&recordsCustomers[i], sizeof(Customers), 1, fpTemporal);
			}
			
			//For to display each location of the customers that have bougth the current product
			for(int i  = 0; i < numOfBuyers ; i += 1){
				fseek(fpTemporal, sizeof(Customers) * i, SEEK_SET);
				fread(&recordCustomer, sizeof(Customers), 1, fpTemporal);

				printf("%-35s %-35s %-35s %-35s\n", recordCustomer.Continent, recordCustomer.Country, recordCustomer.State, recordCustomer.City);
			}

			fclose(fpTemporal);
			free(recordsCustomers); recordsCustomers = NULL;
 		}
	}
}


void BubbleSortOption2(){
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

	Products *recordsProducts = (Products *) calloc(numRecordsProducts, sizeof(Products));
	//For to store the records of ProductsTable in the "array" of Products
	for(int i = 0; i < numRecordsProducts; i += 1){
		//printf("\nArray Products %i", i + 1);
		fseek(fpProducts, sizeof(Products) * i, SEEK_SET);
		fread(&recordsProducts[i], sizeof(Products), 1, fpProducts);
	}

	for (int step = 0; step < numRecordsProducts - 1; step += 1){
		for (int i = 0; i < numRecordsProducts - step - 1; i += 1){
			if(strcmp(recordsProducts[i].ProductName, recordsProducts[i + 1].ProductName) > 0){
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
	//printf("\nHizo products\n");

	Customers *recordsCustomers = (Customers *) calloc(numRecordsCustomers, sizeof(Customers));
	for(int i = 0; i < numRecordsCustomers; i += 1){
		//printf("\nArray Products %i", i + 1);
		fseek(fpCustomers, sizeof(Customers) * i, SEEK_SET);
		fread(&recordsCustomers[i], sizeof(Customers), 1, fpCustomers);
	}

	for (int step = 0; step < numRecordsCustomers - 1; step += 1){
		for (int i = 0; i < numRecordsCustomers - step - 1; i += 1){
			if(recordsCustomers[i].CustomerKey > recordsCustomers[i + 1].CustomerKey){
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
			if(recordsSales[i].ProductKey > recordsSales[i + 1].ProductKey){
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

	free(recordsProducts); recordsProducts = NULL;		//Free the diamic memory where recordsProducts where stored
	free(recordsCustomers); recordsCustomers = NULL;	//Free the diamic memory where recordsCustomers where stored
	free(recordsSales); recordsSales = NULL;			//Free the diamic memory where recordsSales where stored

	DeterminateCustomersLocation(fpProducts, fpSales, fpCustomers, numRecordsProducts, 1);

    fclose(fpProducts);
    fclose(fpCustomers);
    fclose(fpSales);
}

void MergeSortOption2(){
	int numRecordsProducts = TellNumRecords("ProductsTable", sizeof(Products)); //Quantity of products in ProductsTable
	int numRecordsCustomers = TellNumRecords("CustomersTable", sizeof(Customers)); //Quantity of products in CustomersTable
	int numRecordsSales = TellNumRecords("SalesTable", sizeof(Sales)); //Quantity of products in SalesTable

    FILE *fpProducts = fopen("ProductsTable", "rb+");
    FILE *fpSales = fopen("SalesTable", "rb+");
    FILE *fpCustomers = fopen("CustomersTable", "rb+");

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

	Products *recordsProducts = (Products *) calloc(numRecordsProducts, sizeof(Products));
	//For to store the records of ProductsTable in the "array" of Products
	for(int i = 0; i < numRecordsProducts; i += 1){
		//printf("\nArray Products %i", i + 1);
		fseek(fpProducts, sizeof(Products) * i, SEEK_SET);
		fread(&recordsProducts[i], sizeof(Products), 1, fpProducts);
	}

	MergeSortArray(recordsProducts, 0, numRecordsProducts - 1, sizeof(Products), CompareProductsByProductName); //To MergeSortArray the ProductsTable File

	for (int i = 0; i < numRecordsProducts; i += 1){
		//printf("\nArchivo Products %i", i + 1);
		fseek(fpProducts, sizeof(Products) * i, SEEK_SET);
		fwrite(&recordsProducts[i], sizeof(Products), 1, fpProducts);
	}
	//printf("\nHizo products\n");

	Customers *recordsCustomers = (Customers *) calloc(numRecordsCustomers, sizeof(Customers));
	for(int i = 0; i < numRecordsCustomers; i += 1){
		//printf("\nArray Products %i", i + 1);
		fseek(fpCustomers, sizeof(Customers) * i, SEEK_SET);
		fread(&recordsCustomers[i], sizeof(Customers), 1, fpCustomers);
	}

	MergeSortArray(recordsCustomers, 0,numRecordsCustomers - 1, sizeof(Customers), CompareCustomersByCustomerKey); //To MergeSortArray the CustomersTable File

	for (int i = 0; i < numRecordsCustomers; i += 1){
		//printf("\nArchivo Customers %i", i + 1);
		fseek(fpCustomers, sizeof(Customers) * i, SEEK_SET);
		fwrite(&recordsCustomers[i], sizeof(Customers), 1, fpCustomers);
	}
	//printf("\nHizo customers\n");

	Sales *recordsSales = (Sales *) calloc(numRecordsSales, sizeof(Sales));
	for(int i = 0; i < numRecordsSales; i += 1){
		//printf("\nArray Sales %i", i + 1);.,
		fseek(fpSales, sizeof(Sales) * i, SEEK_SET);
		fread(&recordsSales[i], sizeof(Sales), 1, fpSales);
	}
	
	MergeSortArray(recordsSales, 0,numRecordsSales - 1, sizeof(Sales), CompareSalesByProductKey); //To MergeSortArray the SalesTable File

	for (int i = 0; i < numRecordsSales; i += 1){
		//printf("\nArchivo Sales %i", i + 1);
		fseek(fpSales, sizeof(Sales) * i, SEEK_SET);
		fwrite(&recordsSales[i], sizeof(Sales), 1, fpSales);
	}
	//printf("\nHizo sales\n");

	free(recordsProducts); recordsProducts = NULL;		//Free the diamic memory where recordsProducts where stored
	free(recordsCustomers); recordsCustomers = NULL;	//Free the diamic memory where recordsCustomers where stored
	free(recordsSales); recordsSales = NULL;			//Free the diamic memory where recordsSales where stored

	DeterminateCustomersLocation(fpProducts, fpSales, fpCustomers, numRecordsProducts, 2);


    fclose(fpProducts);
    fclose(fpCustomers);
    fclose(fpSales);
}
