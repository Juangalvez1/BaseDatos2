/*
- Autor: Ing(c) Burbano Rodriguez Angel Gabriel
- Nombre del lenguaje utilizado: C
- Versión del lenguaje utilizado: C11
- Versión del compilador utilizado: GCC 6.3.0
- Versión del S.O: Windows 10 Pro Versión 22H2
- Fecha: 21/10/2024
- Hora: 1:30pm
- Descripcion del programa: Este programa consiste en implementar la idea del algoritmo del MergeSort usando archivo.
*/

#include<stdio.h>
#include<string.h>


typedef struct {
    int id;
    char nom[20];
    char lastName[35];
} registro;

/*
Función merge que sirve para unir los dos sub arrays en un archivo
- variable left que contiene el valor de la posición izquierda
- variable right que contiene el valor de la posición derecha
- variable medium que contiene el valor de la posición mitad
- variable fp que es el puntero al archivo binario
*/
void Merge(int left, int right, int medium, FILE *fp){
    // Calcular el tamaño de los sub-arrays
    int firstArray = medium - left + 1;
    int secondArray = right - medium;

    // Crear arrays temporales para almacenar los registros
    registro temporalLeft[firstArray], temporalRight[secondArray];

    // Mover el puntero al inicio del archivo
    fseek(fp, left * sizeof(registro), SEEK_SET);

    // Leer registros para el primer sub-array (izquierda)
    for(int i = 0; i < firstArray; i++) {
        fseek(fp, left * sizeof(registro) + i * sizeof(registro), SEEK_SET);
        fread(&temporalLeft[i], sizeof(registro), 1, fp);
    }

    // Leer registros para el segundo sub-array (derecha)
    fseek(fp, (medium + 1) * sizeof(registro), SEEK_SET);
    for(int j = 0; j < secondArray; j++) {
        fseek(fp, left * sizeof(registro) + j * sizeof(registro), SEEK_SET);
        fread(&temporalRight[j], sizeof(registro), 1, fp);
    }

    // Índices de los sub-arrays temporales
    int i = 0, j = 0;
    // Posición de escritura en el archivo
    int posicion = left;

    // Mover el puntero al inicio del archivo donde se escribirá
    fseek(fp, posicion * sizeof(registro), SEEK_SET);

    // Fusionar los registros ordenados
    while (i < firstArray && j < secondArray) {
        fseek(fp, posicion * sizeof(registro), SEEK_SET);
        if (temporalLeft[i].id < temporalRight[j].id) {
            fwrite(&temporalLeft[i], sizeof(registro), 1, fp);
            i++;
        } else {
            fwrite(&temporalRight[j], sizeof(registro), 1, fp);
            j++;
        }
        posicion++;
    }

    // Escribir los registros restantes del primer sub-array
    while (i < firstArray) {
        fseek(fp, posicion * sizeof(registro), SEEK_SET);
        fwrite(&temporalLeft[i], sizeof(registro), 1, fp);
        i++;
        posicion++;
    }

    // Escribir los registros restantes del segundo sub-array
    while (j < secondArray) {
        fseek(fp, posicion * sizeof(registro), SEEK_SET);
        fwrite(&temporalRight[j], sizeof(registro), 1, fp);
        j++;
        posicion++;
    }
}


/*
Funcion MergeSort que será usado para dividir el archivo a la mitad
- variable left que guarda el número de la posición desde la izquierda del archivo a ajustar
- variable right que guarda el número de la posición derecha del archivo a ajustar
- variable fp que contiene el puntero del archivo a ajustar
*/
void MergeSort(int left, int right, FILE *fp){
    if(left<right){
        //variable medium que guarda el valor de la posición de la mitad del archivo
        int medium = left + (right - left) / 2;

        //Volvemos a llamar a la función pero desde la izquierda hasta la mitad
        MergeSort(left, medium, fp);
        //Volvemos a llamar a la función pero desde la mitad + 1  hasta la derecha
        MergeSort(medium+1, right, fp);
        //Unimos las dos mitades
        Merge(left, right, medium, fp);
    }
}

/*
Función MainMergeSort que permite la apertura del archivo para poder leer y escribir durante el proceso de la función MergeSort
*/
void MainMergeSort(int left, int right, char nameFile[]){
    
    FILE *fp = fopen(nameFile, "rb+");
    
    if (fp == NULL) {
        perror("Error al abrir el archivo");
        return;
    }

    MergeSort(left, right, fp);

    fclose(fp);

}

void PrintFile(char nameFile[], int numberOfRegisters) {
    registro reg;
    FILE *fp = fopen(nameFile, "rb");
    
    if (fp == NULL) {
        perror("Error al abrir el archivo");
        return;
    }
    
    printf("Record \tid\t\tName\t\tlastName\n");
    printf("-------------------------------------------------------------\n");
    for (int i = 0; i < numberOfRegisters; i++) {
        fread(&reg, sizeof(reg), 1, fp);
        printf("%i\t%i\t%s\t\t%s\n", i + 1, reg.id, reg.nom, reg.lastName);
    }
    printf("-----------------------LAST LINE-----------------------------\n");
    
    fclose(fp); // Close the file after printing
}

void CreateFile(char nameFile[]) {
    FILE *fp = fopen(nameFile, "wb");
    registro reg;

    if (fp == NULL) {
        perror("Error al abrir el archivo");
        return;
    }
    
    // first record of five 
    reg.id = 1104941634;
    strcpy(reg.nom, "Angel");
    strcpy(reg.lastName, "Burbano");
    fwrite(&reg, sizeof(reg), 1, fp);
    
    // second record of five
    reg.id = 1105464435;
    strcpy(reg.nom, "Nohora");
    strcpy(reg.lastName, "Cardozo");
    fwrite(&reg, sizeof(reg), 1, fp);
    
    // third record of five
    reg.id = 1055458239;
    strcpy(reg.nom, "Juan");
    strcpy(reg.lastName, "Cano Duque");
    fwrite(&reg, sizeof(reg), 1, fp);
    
    // fourth record of five
    reg.id = 1114151996;
    strcpy(reg.nom, "Brahian");
    strcpy(reg.lastName, "Castro Devia");
    fwrite(&reg, sizeof(reg), 1, fp);
    
    // fifth record of five
    reg.id = 1013650658;
    strcpy(reg.nom, "Ana");
    strcpy(reg.lastName, "Betancour");
    fwrite(&reg, sizeof(reg), 1, fp);
    
    fclose(fp); // Close the file after creating
}

int main(){
    CreateFile("Prueba.dat");
    int numberOfRegisters = 5;
    printf("Archivo original desordenado \n");
    PrintFile("Prueba.dat", numberOfRegisters);
    MainMergeSort(0, numberOfRegisters-1, "Prueba.dat");
    printf("\nArreglo ordenado en orden Ascendente por el campo id \n");
    PrintFile("Prueba.dat", numberOfRegisters);
    return 0;
}