#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdio.h>

// Estructuras
typedef struct {
    long int OrderNumber;
    unsigned char LineItems;
    struct {
        unsigned char MM;
        unsigned char DD;
        unsigned short int AAAA;
    } OrderDate;
    struct {
        unsigned char MM;
        unsigned char DD;
        unsigned short int AAAA;
    } DeliveryDate;
    unsigned int CustomerKey;
    unsigned short int StoreKey;
    unsigned short int ProductKey;
    unsigned short int Quantity;
    char CurrencyCode[3];
} Sales;

typedef struct {
    unsigned int CustomerKey;
    char Gender[8];
    char Name[40];
    char City[40];
    char StateCode[30];
    char State[30];
    unsigned int ZipCode;
    char Country[20];
    char Continent[20];
    struct {
        unsigned char MM;
        unsigned char DD;
        unsigned short int AAAA;
    } Birthday;
} Customers;

typedef struct {
    unsigned short int ProductKey;
    char ProductName[100];
    char Brand[30];
    char Color[15];
    float UnitCostUSD;
    float UnitPriceUSD;
    char SubcategoryKey[5];
    char Subcategory[50];
    char CategoryKey[3];
    char Category[35];
} Products;

typedef struct {
    unsigned short int StoreKey;
    char Country[35];
    char State[35];
    unsigned short int SquareMeters;
    struct {
        unsigned char MM;
        unsigned char DD;
        unsigned short int AAAA;
    } OpenDate;
} Stores;

typedef struct {
    char Date[11];
    char Currency[3];
    float Exchange;
} ExchangeRates;

// Declaraciones de funciones

void SetColor(int color);

int TellNumRecords(char fileName[], int recordSize);

void CreateSalesTable(char originFileName[]);
void CreateCustomersTable(char originFileName[]);
void CreateProductsTable(char originFileName[]);
void CreateStoresTable(char originFileName[]);
void CreateExchangeRatesTable(char originFileName[]);
void CreateDatasetTables();

int BinarySearch(FILE *fp, unsigned long int valueToSearch, int file);
int BinarySearchExchangeDate(FILE *fp, Sales recordSale);

int CompareProductsByProductName(void *a, void *b);
int CompareProductsByProductKey(void *a, void *b);

int CompareCustomersByCustomerKey(void *a, void *b);
int CompareCustomersByCustomerLocation(void *a, void*b);
int CompareCustomersByName(void *a, void *b);

int CompareSalesByProductKey(void *a, void *b);
int CompareSalesByCustomerKey(void *a, void *b);
int CompareSalesByOrderNumber(void *a, void *b);

int CompareExchangeByCurrencyCode(void *a, void *b);

//void BubbleSort(void *array, int numRecords, int recordSize, int (*compare)(void*, void*), void (*swap)(char*, char*));

void MergeArray(void *array, int left, int right, int medium, int recordSize, int (*compare)(void*, void*));
void MergeSortArray(void *array, int left, int right, int recordSize, int (*compare)(void*, void*));


void MergeFile(FILE *fp, int left, int right, int middle, int recordSize, int (*compare)(void*, void*));
void MergeSortFile(FILE *fp, int left, int right, int recordSize, int (*compare)(void*, void*));

#endif // FUNCTIONS_H
