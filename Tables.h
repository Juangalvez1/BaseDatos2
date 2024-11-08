#ifndef TABLES_H
#define TABLES_H

#include <stdio.h>

// Estructuras
typedef struct {
    long int OrderNumber;
    unsigned char LineItems;
    struct {
        unsigned char DD;
        unsigned char MM;
        unsigned short int AAAA;
    } OrderDate;
    struct {
        unsigned char DD;
        unsigned char MM;
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
        unsigned char DD;
        unsigned char MM;
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
    char Category[20];
} Products;

typedef struct {
    unsigned short int StoreKey;
    char Country[35];
    char State[35];
    unsigned short int SquareMeters;
    struct {
        unsigned char DD;
        unsigned char MM;
        unsigned short int AAAA;
    } OpenDate;
} Stores;

typedef struct {
    char Date[11];
    char Currency[3];
    float Exchange;
} ExchangeRates;

// Declaraciones de funciones
int TellNumRecords(char fileName[], int recordSize);
void CreateSalesTable(char originFileName[]);
void CreateCustomersTable(char originFileName[]);
void CreateProductsTable(char originFileName[]);
void CreateStoresTable(char originFileName[]);
void CreateExchangeRatesTable(char originFileName[]);
void CreateDatasetTables();

#endif // TABLES_H
