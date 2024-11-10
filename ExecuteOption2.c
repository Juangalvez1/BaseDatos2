#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Tables.h"

int BinarySearch(FILE *fp, unsigned int valueToSearch, int file){
    unsigned int start = 0, middle = 0, end = 0, sizeOfRecord = 0, key = 0;

    if(file == 1){
		sizeOfRecord = sizeof(Products);
	} else if(file == 2){
		sizeOfRecord = sizeof(Customers);	
	} else if(file == 3){
		sizeOfRecord = sizeof(Sales);
	} else {
		return -1;
	}

    fseek(fp, 0 ,SEEK_END);
    end = ( ftell(fp) / sizeOfRecord ) - 1;
    while(start <= end){
        middle = start + ((end - start) / 2);
        fseek(fp, middle * sizeOfRecord, SEEK_SET);

		if (file == 1){
			Products recordProduct;
			fread(&recordProduct, sizeOfRecord, 1, fp);
			key = (unsigned int) recordProduct.ProductKey;
		} else if (file == 2){
			Customers recordCustomer;
			fread(&recordCustomer, sizeOfRecord, 1, fp);
			key = (unsigned int) recordCustomer.CustomerKey;
		} else if (file == 3){
			Sales recordSale;
			fread(&recordSale, sizeOfRecord, 1, fp);
			key = (unsigned int) recordSale.ProductKey;
		}
        
        if(key == valueToSearch){
            return middle;
        }else if(key < valueToSearch){
			start = middle + 1;
        }else{
            end = middle - 1;
        }
    }
	return-1;
}

int CompareByCustomerKey(void *a, void *b) {
    Customers *custA = (Customers *)a;
    Customers *custB = (Customers *)b;
    return custA->CustomerKey - custB->CustomerKey;
}

int CompareByProductName(void *a, void *b) {
    Products *prodA = (Products *)a;
    Products *prodB = (Products *)b;
    return strcmp(prodA->ProductName, prodB->ProductName);
}

int CompareBySalesProductKey(void *a, void *b){
    Sales *saleA = (Sales *)a;
    Sales *saleB = (Sales *)b;
    return saleA->ProductKey - saleB->ProductKey;
}

int CompareByCustomerLocation(void *a, void*b){
    int result = 0;
    Customers *custA = (Customers *)a;
    Customers *custB = (Customers *)b;

    result = strcmp(custA->Continent, custB->Continent);
    if(result != 0){
        return result;
    }

    result = strcmp(custA->Country, custB->Country);
    if(result != 0){
        return result;
    }

    result = strcmp(custA->State, custB->State);
    if(result != 0){
        return result;
    }

    result = strcmp(custA->City, custB->City);
    return result;

}

int BubbleSortProducts(char fileName[], int numRecordsProducts){

	if (!numRecordsProducts){
		printf("Error tomando la cantidad de registros");
		return 0;
	}

	FILE *fpProducts = fopen(fileName, "rb+");	//Pointer to the file about to be ordered

	if (fpProducts == NULL) {
		printf("Error abriendo archivo en '%s'.\n", fileName);
		return 0;
	}

	//Poiner to the memory where all the ProductsTable's records will be saved
	Products *recordsProducts = (Products *) calloc(numRecordsProducts, sizeof(Products));

	//For to store the records of ProductsTable in the "array" of Products
	for(int i = 0; i < numRecordsProducts; i += 1){
		//printf("\nArray Products %i", i + 1);
		fseek(fpProducts, sizeof(Products) * i, SEEK_SET);
		fread(&recordsProducts[i], sizeof(Products), 1, fpProducts);
	}

	//For to sort Products
  	for ( int step = 0; step < numRecordsProducts - 1; step += 1 ){
		//printf("\nSort Products %i", step + 1);
    	for ( int i = 0; i < numRecordsProducts - step - 1; i += 1 ){
      		if ( strcmp(recordsProducts[i].ProductName, recordsProducts[i + 1].ProductName) > 0 ){
				Products temp = recordsProducts[i]; //Temporal variable to exchange the records
				recordsProducts[i] = recordsProducts[i + 1];
				recordsProducts[i + 1] = temp;
      		}
        }
    }

	//For to write each record alredy ordered in the file ProductsTable
	for (int i = 0; i < numRecordsProducts; i += 1){
		//printf("\nArchivo Products %i", i + 1);
		fseek(fpProducts, sizeof(Products) * i, SEEK_SET);
		fwrite(&recordsProducts[i], sizeof(Products), 1, fpProducts);
	}
	free(recordsProducts);
	recordsProducts = NULL;

	fclose(fpProducts);
	
	return 1;
}

int BubbleSortCustomers(char fileName[], int numRecordsCustomers){

	if (!numRecordsCustomers){
		printf("Error tomando la cantidad de registros");
		return 0;
	}

	FILE *fpCustomers = fopen(fileName, "rb+");	//Pointer to the file about to be ordered

	if (fpCustomers == NULL) {
		printf("Error abriendo archivo en '%s'.\n", fileName);
		return 0;
	}

	//Poiner to the memory where all the CustomersTable's records will be saved
	Customers *recordsCustomers = (Customers *) calloc(numRecordsCustomers, sizeof(Customers));

	//For to store the records of CustomersTable in the "array" of Customers
	for(int i = 0; i < numRecordsCustomers; i += 1){
		//printf("\nArray Customers %i", i + 1);
		fseek(fpCustomers, sizeof(Customers) * i, SEEK_SET);
		fread(&recordsCustomers[i], sizeof(Customers), 1, fpCustomers);
	}

	//For to sort Customers
  	for ( int step = 0; step < numRecordsCustomers - 1; step += 1 ){
		//printf("\nSort Customers %i", step + 1);
    	for ( int i = 0; i < numRecordsCustomers - step - 1; i += 1 ){
      		if (recordsCustomers[i].CustomerKey > recordsCustomers[i + 1].CustomerKey){
				Customers temp = recordsCustomers[i]; //Temporal variable to exchange the records
				recordsCustomers[i] = recordsCustomers[i + 1];
				recordsCustomers[i + 1] = temp;
      		}
        }
    }

	//For to write each record alredy ordered in the file CustomersTable
	for (int i = 0; i < numRecordsCustomers; i += 1){
		//printf("\nArchivo Customers %i", i + 1);
		fseek(fpCustomers, sizeof(Customers) * i, SEEK_SET);
		fwrite(&recordsCustomers[i], sizeof(Customers), 1, fpCustomers);
	}
	free(recordsCustomers);
	recordsCustomers = NULL;

	fclose(fpCustomers);
	
	return 1;
}

int BubbleSortSales(char fileName[], int numRecordsSales){

	if (!numRecordsSales){
		printf("Error tomando la cantidad de registros");
		return 0;
	}

	FILE *fpSales = fopen(fileName, "rb+");	//Pointer to the file about to be ordered

	if (fpSales == NULL) {
		printf("Error abriendo archivo en '%s'.\n", fileName);
		return 0;
	}

	//Poiner to the memory where all the SalesTable's records will be saved
	Sales *recordsSales = (Sales *) calloc(numRecordsSales, sizeof(Sales)); 

	//For to store the records of SalesTable in the "array" of Sales
	for(int i = 0; i < numRecordsSales; i += 1){
		//printf("\nArray Sales %i", i + 1);
		fseek(fpSales, sizeof(Sales) * i, SEEK_SET);
		fread(&recordsSales[i], sizeof(Sales), 1, fpSales);
	}

	//For to sort Sales
  	for ( int step = 0; step < numRecordsSales - 1; step += 1 ){
		//printf("\nSort Sales %i", step + 1);
    	for ( int i = 0; i < numRecordsSales - step - 1; i += 1 ){
      		if (recordsSales[i].ProductKey > recordsSales[i + 1].ProductKey){
				Sales temp = recordsSales[i]; //Temporal variable to exchange the records
				recordsSales[i] = recordsSales[i + 1];
				recordsSales[i + 1] = temp;
      		}
        }
    }

	//For to write each record alredy ordered in the file SalesTable
	for (int i = 0; i < numRecordsSales; i += 1){
		//printf("\nArchivo Sales %i", i + 1);
		fseek(fpSales, sizeof(Sales) * i, SEEK_SET);
		fwrite(&recordsSales[i], sizeof(Sales), 1, fpSales);
		//printf("%ld, %u, %u/%u/%hu, %u/%u/%hu, %u, %hu, %hu, %hu, %s\n", recordsSales[i].OrderNumber, recordsSales[i].LineItems, recordsSales[i].OrderDate.MM, recordsSales[i].OrderDate.DD, recordsSales[i].OrderDate.AAAA, recordsSales[i].DeliveryDate.MM, recordsSales[i].DeliveryDate.DD, recordsSales[i].DeliveryDate.AAAA, recordsSales[i].CustomerKey, recordsSales[i].StoreKey, recordsSales[i].ProductKey, recordsSales[i].Quantity, recordsSales[i].CurrencyCode);

	}
	free(recordsSales);
	recordsSales = NULL;

	fclose(fpSales);
	
	return 1;
}

void Merge(void *array, int left, int right, int medium, int recordSize, int (*compare)(void*, void*)) {
    int firstArray = medium - left + 1;
    int secondArray = right - medium;
    void *temporalLeft = calloc(firstArray,  recordSize);
    void *temporalRight = calloc(secondArray,  recordSize);

    //Copy the values of the first half
    for (int i = 0; i < firstArray; i++) {
        memcpy((char*)temporalLeft + i * recordSize, (char*)array + (left + i) * recordSize, recordSize);
    }

    //Copy the values of the second half
    for (int j = 0; j < secondArray; j++) {
        memcpy((char*)temporalRight + j * recordSize, (char*)array + (medium + 1 + j) * recordSize, recordSize);
    }

    int i = 0, j = 0;
    int posicion = left;

    //Order and combine the two temporal arrays
    while (i < firstArray && j < secondArray) {
        if (compare((char*)temporalLeft + i * recordSize, (char*)temporalRight + j * recordSize) <= 0) {
            memcpy((char*)array + posicion * recordSize, (char*)temporalLeft + i * recordSize, recordSize);
            i++;
        } else {
            memcpy((char*)array + posicion * recordSize, (char*)temporalRight + j * recordSize, recordSize);
            j++;
        }
        posicion++;
    }

    // Copiar los elementos restantes de temporalLeft
    while (i < firstArray) {
        memcpy((char*)array + posicion * recordSize, (char*)temporalLeft + i * recordSize, recordSize);
        i++;
        posicion++;
    }

    // Copiar los elementos restantes de temporalRight
    while (j < secondArray) {
        memcpy((char*)array + posicion * recordSize, (char*)temporalRight + j * recordSize, recordSize);
        j++;
        posicion++;
    }

    free(temporalLeft);
    free(temporalRight);
}

void MergeSort(void *array, int left, int right, int recordSize, int (*compare)(void*, void*)) {
    if (left < right) {
        int medium = left + ((right - left) / 2);

        //Recursive call for the first half
        MergeSort(array, left, medium, recordSize, compare);

        //Recursive call for the second half
        MergeSort(array, medium + 1, right, recordSize, compare);

        //Order and combine the two halfs
        Merge(array, left, right, medium, recordSize, compare);

    }
}

void DeterminatedCustomersLocation(FILE *fpProducts, FILE *fpSales, FILE *fpCustomers, int numOfProducts, int typeofSort){
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
			FILE *fpTemporal = fopen("TemporalFileOption2", "wb+");
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
    	    while (recordSale.ProductKey == productKey && index <= 66283){
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
		
			int comparation = 0;
			if(typeofSort == 1){ //Executing option 2.1
				//For to sort Customers
  				for ( int step = 0; step < numOfBuyers - 1; step += 1 ){
					//printf("\nSort Customers %i", step + 1);
    				for ( int i = 0; i < numOfBuyers - step - 1; i += 1 ){

						comparation = strcmp(recordsCustomers[i].Continent, recordsCustomers[i + 1].Continent);
    			  		if (comparation > 0){
							Customers temp = recordsCustomers[i]; //Temporal variable to exchange the records
							recordsCustomers[i] = recordsCustomers[i + 1];
							recordsCustomers[i + 1] = temp;

						//Compare if the continents are equal
    			  		} else if(comparation == 0){

							comparation = strcmp(recordsCustomers[i].Country, recordsCustomers[i + 1].Country);
							if (comparation > 0){
								Customers temp = recordsCustomers[i]; //Temporal variable to exchange the records
								recordsCustomers[i] = recordsCustomers[i + 1];
								recordsCustomers[i + 1] = temp;

							//Compare if the countries are equal
							} else if (comparation == 0){

								comparation = strcmp(recordsCustomers[i].State, recordsCustomers[i + 1].State);
								if (comparation > 0){
									Customers temp = recordsCustomers[i]; //Temporal variable to exchange the records
									recordsCustomers[i] = recordsCustomers[i + 1];
									recordsCustomers[i + 1] = temp;

								//Compare if the States are equal
								} else if (comparation == 0){

									comparation = strcmp(recordsCustomers[i].City, recordsCustomers[i + 1].City);
									if (comparation > 0){
										Customers temp = recordsCustomers[i]; //Temporal variable to exchange the records
										recordsCustomers[i] = recordsCustomers[i + 1];
										recordsCustomers[i + 1] = temp;
									}
								}
							}
						}
    			    }
    			}
			} else if (typeofSort == 2){ //Executing option 2.2
				MergeSort(recordsCustomers, 0, numOfBuyers - 1, sizeof(Customers), CompareByCustomerLocation);
			}
		
			//For to write each record alredy ordered in the file CustomersTable
			for (int i = 0; i < numOfBuyers; i += 1){
				//printf("\nArchivo Customers %i", i + 1);
				fseek(fpTemporal, sizeof(Customers) * i, SEEK_SET);
				fwrite(&recordsCustomers[i], sizeof(Customers), 1, fpTemporal);
			}

			//For to display each location of the customers that have bougth the current product
			for(int i  = 0; i < numOfBuyers ; i++){
				fseek(fpTemporal, sizeof(Customers) * i, SEEK_SET);
				fread(&recordCustomer, sizeof(Customers), 1, fpTemporal);

				printf("%-35s %-35s %-35s %-35s\n", recordCustomer.Continent, recordCustomer.Country, recordCustomer.State, recordCustomer.City);
			}

			fclose(fpTemporal);
			free(recordsCustomers);
			recordsCustomers = NULL;
 		}
	}
}

void BubbleSortOption2(){
	int numRecordsProducts = TellNumRecords("ProductsTable", sizeof(Products)); 	//Quantity of products in ProductsTable
	int numRecordsCustomers = TellNumRecords("CustomersTable", sizeof(Customers)); 	//Quantity of products in CustomersTable
	int numRecordsSales = TellNumRecords("SalesTable", sizeof(Sales)); 				//Quantity of products in SalesTable

	BubbleSortProducts("ProductsTable", numRecordsProducts);	//To BubbleSort the ProductsTable File
	BubbleSortCustomers("CustomersTable", numRecordsCustomers);	//To BubbleSort the CustomersTable File
	BubbleSortSales("SalesTable", numRecordsSales);				//To BubbleSort the SalesTable File

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
	
	DeterminatedCustomersLocation(fpProducts, fpSales, fpCustomers, numRecordsProducts, 1);

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

	MergeSort(recordsProducts, 0, numRecordsProducts - 1, sizeof(Products), CompareByProductName); //To MergeSort the ProductsTable File

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

	MergeSort(recordsCustomers, 0,numRecordsCustomers - 1, sizeof(Customers), CompareByCustomerKey); //To MergeSort the CustomersTable File

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
	
	MergeSort(recordsSales, 0,numRecordsSales - 1, sizeof(Sales), CompareBySalesProductKey); //To MergeSort the SalesTable File

	for (int i = 0; i < numRecordsSales; i += 1){
		//printf("\nArchivo Sales %i", i + 1);
		fseek(fpSales, sizeof(Sales) * i, SEEK_SET);
		fwrite(&recordsSales[i], sizeof(Sales), 1, fpSales);
	}
	//printf("\nHizo sales\n");

	DeterminatedCustomersLocation(fpProducts, fpSales, fpCustomers, numRecordsProducts, 2);

	free(recordsProducts); recordsProducts = NULL;		//Free the diamic memory where recordsProducts where stored
	free(recordsCustomers); recordsCustomers = NULL;	//Free the diamic memory where recordsCustomers where stored
	free(recordsSales); recordsSales = NULL;			//Free the diamic memory where recordsSales where stored

    fclose(fpProducts);
    fclose(fpCustomers);
    fclose(fpSales);
}