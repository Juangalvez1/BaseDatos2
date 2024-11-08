#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Tables.h"

/*
void SortBubbleForFile(FILE *fp, int sizeProducts){
	if (fp == NULL) {
		printf("Error abriendo archivo en 'SortBubbleForFile'.\n");
		return;
	}

  	for ( int step = 0; step < sizeProducts - 1; step += 1 ){

    	for ( int i = 0; i < sizeProducts - step - 1; i += 1 ){

      		Products reg1, reg2;
			fseek(fp, sizeof(Products) * i, SEEK_SET);
      		fread(&reg1, sizeof(Products), 1, fp);
			fread(&reg2, sizeof(Products), 1, fp);

      		if ( reg1.id > reg2.id ){
        		fseek(fp, sizeof(Products) * i, SEEK_SET);
        		fwrite(&reg2, sizeof(Products), 1, fp);
				fwrite(&reg1, sizeof(Products), 1, fp);
      		}
    	}
  	}
  	fclose(fp);
}
*/

void BubbleSortOption2(){
    FILE *fpProducts = fopen("ProductsTable", "rb+");
    FILE *fpSales = fopen("SalesTable", "rb+");
    FILE *fpCustomers = fopen("CustomersTable", "rb+");

    if (fpProducts == NULL) {
		printf("Error abriendo archivo en 'ProductsTable'.\n");
		return;
	}
    if (fpSales == NULL) {
		printf("Error abriendo archivo en 'SalesTable'.\n");
		return;
	}
    if (fpCustomers == NULL) {
		printf("Error abriendo archivo en 'CustomersTable'.\n");
		return;
	}

    int sizeProducts = 0;// sizeSales = 0, sizeCustomers = 0;

    sizeProducts = TellNumRecords("ProductsTable", sizeof(Products));
    //sizeCustomers = TellNumRecords(CustomersTable", sizeof(Customers));
    //sizeSales = TellNumRecords("SalesTable", sizeof(Sales));
    printf("\n%i\n", sizeProducts);


  	for ( int step = 0; step < sizeProducts - 1; step += 1 ){
        printf("Llega %i\n", step + 1);
    	for ( int i = 0; i < sizeProducts - step - 1; i += 1 ){
      		Products reg1, reg2;
			fseek(fpProducts, sizeof(Products) * i, SEEK_SET);
      		fread(&reg1, sizeof(Products), 1, fpProducts);

			fseek(fpProducts, sizeof(Products) * (i + 1), SEEK_SET);
            fread(&reg2, sizeof(Products), 1, fpProducts);

      		if ( strcmp(reg1.ProductName, reg2.ProductName) > 0 ){
        		fseek(fpProducts, sizeof(Products) * i, SEEK_SET);
        		fwrite(&reg2, sizeof(Products), 1, fpProducts);

				fseek(fpProducts, sizeof(Products) * (i + 1), SEEK_SET);
                fwrite(&reg1, sizeof(Products), 1, fpProducts);
      		}
        }
    }

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