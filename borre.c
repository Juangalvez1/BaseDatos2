#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "Functions.c"


// Función para búsqueda binaria
int binarySearch(FILE *file, const char *targetDate, ExchangeRates *result) {

    if (!file) {
        perror("Error al abrir el archivo");
        return -1;
    }

    // Tamaño de un registro
    size_t recordSize = sizeof(ExchangeRates);

    // Calcular el número total de registros
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    long totalRecords = fileSize / recordSize;
    fseek(file, 0, SEEK_SET);

    // Variables para la búsqueda binaria
    long low = 0, high = totalRecords - 1, mid;

    while (low <= high) {
        mid = low + (low + high) / 2;

        // Posicionar el cursor en el registro medio
        fseek(file, mid * recordSize, SEEK_SET);

        // Leer el registro
        ExchangeRates current;
        fread(&current, recordSize, 1, file);

        // Comparar fechas
        int cmp = strcmp(current.Date, targetDate);
        if (cmp == 0) {
            // Fecha encontrada
            *result = current;
            fclose(file);
            return mid;
        } else if (cmp < 0) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    // No se encontró la fecha
    return -1;
}

int main() {
    FILE *fp = fopen("ExchangeRatesTable", "rb+");
    ExchangeRates record;
    fseek(fp, sizeof(ExchangeRates) * 5, SEEK_SET);
    fread(&record, sizeof(ExchangeRates), 1, fp);
    const char *targetDate = record.Date;
    ExchangeRates result;
    int position = binarySearch(fp, targetDate, &result) != 0;
    if(position != -1){
        printf("Fecha encontrada: %s\n", result.Date);
        printf("Moneda: %s\n", result.Currency);
        printf("Tipo de cambio: %.2f\n", result.Exchange);
        printf("\tposition: %i\n", position);
    } else {
        printf("Fecha no encontrada.\n");
    }
    fclose(fp);
    return 0;
}
