#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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



int TellNumRecords(char fileName[], int recordSize) {
    FILE *fp = fopen(fileName, "rb"); // Abrir en modo binario
    if (fp == NULL) {
        return -1; // Retornar -1 en caso de error al abrir el archivo
    }

    fseek(fp, 0, SEEK_END); // Mover el puntero al final del archivo
    long fileSize = ftell(fp); // Obtener el tamaño del archivo en bytes
    fclose(fp);

    // Calcular el número de registros
    if (recordSize == 0) {
        return -2; // Evitar división por cero
    }
    int numRecords = fileSize / recordSize;

    return numRecords;
}

void BubbleSortOption2(){
    FILE *fpProducts = fopen("ProductsTable", "rb");
    FILE *fpSales = fopen("SalesTable", "rb");
    FILE *fpCustomers = fopen("CustomersTable", "rb");

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

    sizeCustomers = TellNumRecords("ProductsTable", sizeof(Products));
    sizeSales = TellNumRecords("SalesTable", sizeof(Sales));
    sizeProducts = TellNumRecords("CustomersTable", sizeof(Customers));

    int comparation = 0;

  	for ( int step = 0; step < sizeProducts - 1; step += 1 ){
    	for ( int i = 0; i < sizeProducts - step - 1; i += 1 ){
      		Products reg1, reg2;
			fseek(fpProducts, sizeof(Products) * i, SEEK_SET);
      		fread(&reg1, sizeof(Products), 1, fpProducts);
			fread(&reg2, sizeof(Products), 1, fpProducts);
            
            comparation = strcpm(reg2.ProductName, reg1.ProductName);

      		if ( comparation > 0 ){
        		fseek(fpProducts, sizeof(Products) * i, SEEK_SET);
        		fwrite(&reg2, sizeof(Products), 1, fpProducts);
				fwrite(&reg1, sizeof(Products), 1, fpProducts);
      		}
        }
    }
    

    fclose(fpProducts);
    fclose(fpSales);
    fclose(fpCustomers);
}

void MergeSortOption2(){
    FILE *fpProducts = fopen("ProductsTable", "rb");
    FILE *fpSales = fopen("SalesTable", "rb");
    FILE *fpCustomers = fopen("CustomersTable", "rb");

    //Products recordProduct;
    //Sales recordSale;
    //Customers recordCustomer;

    fclose(fpProducts);
    fclose(fpSales);
    fclose(fpCustomers);
}