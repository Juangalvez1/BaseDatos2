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
			Products record;
			fread(&record, sizeOfRecord, 1, fp);
			key = (unsigned int) record.ProductKey;
		} else if (file == 2){
			Customers record;
			fread(&record, sizeOfRecord, 1, fp);
			key = (unsigned int) record.CustomerKey;
		} else if (file == 3){
			Sales record;
			fread(&record, sizeOfRecord, 1, fp);
			key = (unsigned int) record.ProductKey;
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

void DeterminatedCustomersLocation(FILE *fpProducts, FILE *fpSales, FILE *fpCustomers, int numOfProducts){
  	Products recordProduct;
	Customers recordCustomer;
	Sales recordSale;

	char productName[100] = "";
	unsigned short int productKey = 0;
	unsigned int customerKey = 0;

	for(int i = 0; i < numOfProducts; i++){
    	fseek(fpProducts, sizeof(Products) * i, SEEK_SET);
    	fread(&recordProduct, sizeof(recordProduct), 1, fpProducts);

    	strcpy(productName, recordProduct.ProductName);
    	printf("\n%s", productName);

		productKey = recordProduct.ProductKey;

    	int positionSales = BinarySearch(fpSales, productKey, 3);

    	if( positionSales == -1){
    		printf(" - No sales reported\n");
    	} else {
    		FILE *fpTemporal = fopen("TemporalFileOption2", "wb+");
			if (fpTemporal == NULL) {
			    printf("Error abriendo archivo temporal.\n");
			    return;
			}

    	    printf("\n%-35s %-35s %-35s %-35s\n", "Continent", "Country", "State", "City");
    	    printf("_______________________________________________________________________________________________________________________________________\n");

    	    fseek(fpSales, sizeof(Sales) * (positionSales - 1), SEEK_SET);
    	    fread(&recordSale, sizeof(Sales), 1, fpSales);

    	    for( int i = positionSales - 1; i >= 0 && productKey == recordSale.ProductKey; i -= 1){
    	    	fseek(fpSales, sizeof(Sales) * (i - 1), SEEK_SET);
    	    	fread(&recordSale, sizeof(Sales), 1, fpSales);

    	    	positionSales = i;
    	    }

			fseek(fpSales, sizeof(Sales) * positionSales, SEEK_SET);
			fread(&recordSale, sizeof(Sales), 1, fpSales);

			int index = positionSales, numOfBuyers = 0, positionCustomers = 0;
    	    while (recordSale.ProductKey == productKey && index <= 66283){
				customerKey = recordSale.CustomerKey;

				positionCustomers = BinarySearch(fpCustomers, customerKey, 2);

				if (positionCustomers != -1) {
                    fseek(fpCustomers, sizeof(Customers) * positionCustomers, SEEK_SET);
                    fread(&recordCustomer, sizeof(Customers), 1, fpCustomers);

                    fwrite(&recordCustomer, sizeof(Customers), 1, fpTemporal);
                    numOfBuyers++;
                }

				fseek(fpSales, sizeof(Sales) * (index + 1), SEEK_SET);
				fread(&recordSale, sizeof(Sales), 1, fpSales);

				index++;
			}
			
			Customers *recordsCustomers = (Customers *) calloc(numOfBuyers, sizeof(Customers));

			//For to store the records of CustomersTable in the "array" of Customers
			for(int i = 0; i < numOfBuyers; i += 1){
				//printf("\nArray Customers %i", i + 1);
				fseek(fpTemporal, sizeof(Customers) * i, SEEK_SET);
				fread(&recordsCustomers[i], sizeof(Customers), 1, fpTemporal);
			}
		
			//For to sort Customers
  			for ( int step = 0; step < numOfBuyers - 1; step += 1 ){
				//printf("\nSort Customers %i", step + 1);
    			for ( int i = 0; i < numOfBuyers - step - 1; i += 1 ){

					int comparation = strcmp(recordsCustomers[i].Continent, recordsCustomers[i + 1].Continent);
    		  		if (comparation > 0){
						Customers temp = recordsCustomers[i]; //Temporal variable to exchange the records
						recordsCustomers[i] = recordsCustomers[i + 1];
						recordsCustomers[i + 1] = temp;
    		  		} else if(comparation == 0){

						comparation = strcmp(recordsCustomers[i].Country, recordsCustomers[i + 1].Country);
						if (comparation > 0){
							Customers temp = recordsCustomers[i]; //Temporal variable to exchange the records
							recordsCustomers[i] = recordsCustomers[i + 1];
							recordsCustomers[i + 1] = temp;
						} else if (comparation == 0){

							comparation = strcmp(recordsCustomers[i].State, recordsCustomers[i + 1].State);
							if (comparation > 0){
								Customers temp = recordsCustomers[i]; //Temporal variable to exchange the records
								recordsCustomers[i] = recordsCustomers[i + 1];
								recordsCustomers[i + 1] = temp;
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
		
			//For to write each record alredy ordered in the file CustomersTable
			for (int i = 0; i < numOfBuyers; i += 1){
				//printf("\nArchivo Customers %i", i + 1);
				fseek(fpTemporal, sizeof(Customers) * i, SEEK_SET);
				fwrite(&recordsCustomers[i], sizeof(Customers), 1, fpTemporal);
			}


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
	int numRecordsProducts = TellNumRecords("ProductsTable", sizeof(Products)); //Quantity of products in ProductsTable
	int numRecordsCustomers = TellNumRecords("CustomersTable", sizeof(Customers)); //Quantity of products in CustomersTable
	int numRecordsSales = TellNumRecords("SalesTable", sizeof(Sales)); //Quantity of products in SalesTable

	BubbleSortProducts("ProductsTable", numRecordsProducts);
	BubbleSortCustomers("CustomersTable", numRecordsCustomers);
	BubbleSortSales("SalesTable", numRecordsSales);

	FILE *fpProducts = fopen("ProductsTable", "rb+");	//Pointer to ProductsTable
    FILE *fpCustomers = fopen("CustomersTable", "rb+");	//Pointer to CustomersTable
    FILE *fpSales = fopen("SalesTable", "rb+");			//Pointer to SalesTable	

	DeterminatedCustomersLocation(fpProducts, fpSales, fpCustomers, numRecordsProducts);

    fclose(fpProducts);
    fclose(fpSales);
    fclose(fpCustomers);
}

void MergeSortOption2(){
    FILE *fpProducts = fopen("ProductsTable", "rb+");
    FILE *fpSales = fopen("SalesTable", "rb+");
    FILE *fpCustomers = fopen("CustomersTable", "rb+");

    //Products recordProduct;
    //Sales recordSale;
    //Customers recordCustomer;

    fclose(fpProducts);
    fclose(fpSales);
    fclose(fpCustomers);
}