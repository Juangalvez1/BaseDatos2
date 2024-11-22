#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Functions.h"

/* 
    Function: DeterminateCustomersLocation
    Purpose: Determines and displays the geographical locations of customers who purchased a specific product.
    Parameters: 
      - fpProducts: Pointer to the file containing the ProductsTable.
      - fpSales: Pointer to the file containing the SalesTable.
      - fpCustomers: Pointer to the file containing the CustomersTable.
      - numOfProducts: Total number of products in the ProductsTable.
      - typeofSort: Integer specifying the sorting method:
          * 1 for BubbleSort.
          * 2 for MergeSort.
    Usage: 
      - This function iterates through the ProductsTable, finds related sales in the SalesTable,
        and identifies the customers from the CustomersTable who purchased those products.
      - It outputs the customers' geographical information (continent, country, state, city),
        sorted as specified by `typeofSort`.
    Variables:
      - recordProduct: Stores a record of the ProductsTable for processing.
      - recordCustomer: Temporarily stores a record from the CustomersTable for sorting and output.
      - recordSale: Temporarily stores a record from the SalesTable to identify related customers.
      - productName: Stores the name of the current product being analyzed.
      - productKey: Unique key identifier for the product.
      - customerKey: Unique key identifier for the customer who made a purchase.
*/
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
			if (typeofSort == 1) { // Performing BubbleSort on customer data (Option 2.1)
    for (int step = 0; step < numOfBuyers - 1; step += 1) {
        // Temporary variables to store customer records for comparison.
        Customers reg1, reg2;
        for (int i = 0; i < numOfBuyers - step - 1; i += 1) {
            // Read the current customer record (reg1).
            fseek(fpTemporal, i * sizeof(Customers), SEEK_SET);
            fread(&reg1, sizeof(Customers), 1, fpTemporal);

            // Read the next customer record (reg2).
            fseek(fpTemporal, (i + 1) * sizeof(Customers), SEEK_SET);
            fread(&reg2, sizeof(Customers), 1, fpTemporal);

            // Compare by continent and swap if necessary.
            int comparation = strcmp(reg1.Continent, reg2.Continent);
            if (comparation > 0) {
                // Write reg2 into the position of reg1.
                fseek(fpTemporal, i * sizeof(Customers), SEEK_SET);
                fwrite(&reg2, sizeof(Customers), 1, fpTemporal);

                // Write reg1 into the position of reg2.
                fseek(fpTemporal, (i + 1) * sizeof(Customers), SEEK_SET);
                fwrite(&reg1, sizeof(Customers), 1, fpTemporal);
            } else if (comparation == 0) { // If continents are equal, compare by country.
                comparation = strcmp(reg1.Country, reg2.Country);
                if (comparation > 0) {
                    // Write reg2 into the position of reg1.
                    fseek(fpTemporal, i * sizeof(Customers), SEEK_SET);
                    fwrite(&reg2, sizeof(Customers), 1, fpTemporal);

                    // Write reg1 into the position of reg2.
                    fseek(fpTemporal, (i + 1) * sizeof(Customers), SEEK_SET);
                    fwrite(&reg1, sizeof(Customers), 1, fpTemporal);
                } else if (comparation == 0) { // If countries are equal, compare by state.
                    comparation = strcmp(reg1.State, reg2.State);
                    if (comparation > 0) {
                        // Write reg2 into the position of reg1.
                        fseek(fpTemporal, i * sizeof(Customers), SEEK_SET);
                        fwrite(&reg2, sizeof(Customers), 1, fpTemporal);

                        // Write reg1 into the position of reg2.
                        fseek(fpTemporal, (i + 1) * sizeof(Customers), SEEK_SET);
                        fwrite(&reg1, sizeof(Customers), 1, fpTemporal);
                    } else if (comparation == 0) { // If states are equal, compare by city.
                        comparation = strcmp(reg1.City, reg2.City);
                        if (comparation > 0) {
                            // Write reg2 into the position of reg1.
                            fseek(fpTemporal, i * sizeof(Customers), SEEK_SET);
                            fwrite(&reg2, sizeof(Customers), 1, fpTemporal);

                            // Write reg1 into the position of reg2.
                            fseek(fpTemporal, (i + 1) * sizeof(Customers), SEEK_SET);
                            fwrite(&reg1, sizeof(Customers), 1, fpTemporal);
                        }
                    }
                }
            }
        }
    }
} else if (typeofSort == 2) { // Performing MergeSort on customer data (Option 2.2)
    MergeSortFile(fpTemporal, 0, numOfBuyers - 1, sizeof(Customers), CompareCustomersByCustomerLocation);
}

			
			//For to display each location of the customers that have bougth the current product
			for(int i  = 0; i < numOfBuyers ; i += 1){
				fseek(fpTemporal, sizeof(Customers) * i, SEEK_SET);
				fread(&recordCustomer, sizeof(Customers), 1, fpTemporal);

				printf("%-35s %-35s %-35s %-35s\n", recordCustomer.Continent, recordCustomer.Country, recordCustomer.State, recordCustomer.City);
			}

			fclose(fpTemporal);
 		}
	}
}


/* 
    Function: BubbleSortOption2
    Purpose: Sorts the records in the ProductsTable, CustomersTable, and SalesTable using BubbleSort.
    After sorting, it calls DeterminateCustomersLocation to analyze customer locations for products.
    Parameters: None.
    Returns: None.
*/
void BubbleSortOption2() {
    // Get the number of records in each table.
    int numRecordsProducts = TellNumRecords("ProductsTable", sizeof(Products));   // Number of products in ProductsTable.
    int numRecordsCustomers = TellNumRecords("CustomersTable", sizeof(Customers)); // Number of customers in CustomersTable.
    int numRecordsSales = TellNumRecords("SalesTable", sizeof(Sales));           // Number of sales in SalesTable.

    // Open the respective files for reading and writing.
    FILE *fpProducts = fopen("ProductsTable", "rb+");   // Pointer to ProductsTable.
    FILE *fpCustomers = fopen("CustomersTable", "rb+"); // Pointer to CustomersTable.
    FILE *fpSales = fopen("SalesTable", "rb+");         // Pointer to SalesTable.

    // Check for errors while opening the files.
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

    // Perform BubbleSort on ProductsTable based on ProductName.
    for (int step = 0; step < numRecordsProducts - 1; step++) {
        Products reg1, reg2;
        for (int i = 0; i < numRecordsProducts - step - 1; i++) {
            // Read two consecutive records from ProductsTable.
            fseek(fpProducts, sizeof(Products) * i, SEEK_SET);
            fread(&reg1, sizeof(Products), 1, fpProducts);

            fseek(fpProducts, sizeof(Products) * (i + 1), SEEK_SET);
            fread(&reg2, sizeof(Products), 1, fpProducts);

            // Compare ProductName and swap if needed.
            if (strcmp(reg1.ProductName, reg2.ProductName) > 0) {
                fseek(fpProducts, sizeof(Products) * i, SEEK_SET);
                fwrite(&reg2, sizeof(Products), 1, fpProducts);

                fseek(fpProducts, sizeof(Products) * (i + 1), SEEK_SET);
                fwrite(&reg1, sizeof(Products), 1, fpProducts);
            }
        }
    }

    // Perform BubbleSort on CustomersTable based on CustomerKey.
    for (int step = 0; step < numRecordsCustomers - 1; step++) {
        Customers reg1, reg2;
        for (int i = 0; i < numRecordsCustomers - step - 1; i++) {
            // Read two consecutive records from CustomersTable.
            fseek(fpCustomers, sizeof(Customers) * i, SEEK_SET);
            fread(&reg1, sizeof(Customers), 1, fpCustomers);

            fseek(fpCustomers, sizeof(Customers) * (i + 1), SEEK_SET);
            fread(&reg2, sizeof(Customers), 1, fpCustomers);

            // Compare CustomerKey and swap if needed.
            if (reg1.CustomerKey > reg2.CustomerKey) {
                fseek(fpCustomers, sizeof(Customers) * i, SEEK_SET);
                fwrite(&reg2, sizeof(Customers), 1, fpCustomers);

                fseek(fpCustomers, sizeof(Customers) * (i + 1), SEEK_SET);
                fwrite(&reg1, sizeof(Customers), 1, fpCustomers);
            }
        }
    }

    // Perform BubbleSort on SalesTable based on ProductKey.
    for (int step = 0; step < numRecordsSales - 1; step++) {
        Sales reg1, reg2;
        for (int i = 0; i < numRecordsSales - step - 1; i++) {
            // Read two consecutive records from SalesTable.
            fseek(fpSales, sizeof(Sales) * i, SEEK_SET);
            fread(&reg1, sizeof(Sales), 1, fpSales);

            fseek(fpSales, sizeof(Sales) * (i + 1), SEEK_SET);
            fread(&reg2, sizeof(Sales), 1, fpSales);

            // Compare ProductKey and swap if needed.
            if (reg1.ProductKey > reg2.ProductKey) {
                fseek(fpSales, sizeof(Sales) * i, SEEK_SET);
                fwrite(&reg2, sizeof(Sales), 1, fpSales);

                fseek(fpSales, sizeof(Sales) * (i + 1), SEEK_SET);
                fwrite(&reg1, sizeof(Sales), 1, fpSales);
            }
        }
    }

    // Call the DeterminateCustomersLocation function with BubbleSort as the sorting method.
    DeterminateCustomersLocation(fpProducts, fpSales, fpCustomers, numRecordsProducts, 1);

    // Close all file pointers.
    fclose(fpProducts);
    fclose(fpCustomers);
    fclose(fpSales);
}

/* 
    Function: MergeSortOption2
    Purpose: Sorts the records in the ProductsTable, CustomersTable, and SalesTable using MergeSort.
    After sorting, it calls DeterminateCustomersLocation to analyze customer locations for products.
    Parameters: None.
    Returns: None.
*/
void MergeSortOption2() {
    // Get the number of records in each table.
    int numRecordsProducts = TellNumRecords("ProductsTable", sizeof(Products));   // Number of products in ProductsTable.
    int numRecordsCustomers = TellNumRecords("CustomersTable", sizeof(Customers)); // Number of customers in CustomersTable.
    int numRecordsSales = TellNumRecords("SalesTable", sizeof(Sales));           // Number of sales in SalesTable.

    // Open the respective files for reading and writing.
    FILE *fpProducts = fopen("ProductsTable", "rb+");
    FILE *fpSales = fopen("SalesTable", "rb+");
    FILE *fpCustomers = fopen("CustomersTable", "rb+");

    // Check for errors while opening the files.
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

    // Perform MergeSort on ProductsTable based on ProductName.
    MergeSortFile(fpProducts, 0, numRecordsProducts - 1, sizeof(Products), CompareProductsByProductName);

    // Perform MergeSort on CustomersTable based on CustomerKey.
    MergeSortFile(fpCustomers, 0, numRecordsCustomers - 1, sizeof(Customers), CompareCustomersByCustomerKey);

    // Perform MergeSort on SalesTable based on ProductKey.
    MergeSortFile(fpSales, 0, numRecordsSales - 1, sizeof(Sales), CompareSalesByProductKey);

    // Call the DeterminateCustomersLocation function with MergeSort as the sorting method.
    DeterminateCustomersLocation(fpProducts, fpSales, fpCustomers, numRecordsProducts, 2);

    // Close all file pointers.
    fclose(fpProducts);
    fclose(fpCustomers);
    fclose(fpSales);
}
