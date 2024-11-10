/*
- Autor: Ing(c) Burbano Rodriguez Angel Gabriel
- Nombre del lenguaje utilizado: C
- Versión del lenguaje utilizado: C11
- Versión del compilador utilizado: GCC 6.3.0
- Versión del S.O: Windows 10 Pro Versión 22H2
- Fecha: 20/10/2024
- Hora: 4:40pm
- Descripcion del programa: Este programa consiste en implementar la idea del algoritmo del MergeSort usando arrays.
*/

#include<stdio.h>



/*
Función merge que sirve para unir los dos sub arrays
- variable left que contiene el valor de la posición izquierda
- variable right que contiene el valor de la posición derecha
- variable medium que contiene el valor de la posición mitad
- variable array que contiene el array a ajustar
*/
void Merge(int left, int right, int medium, int array[]){
    //variable firstArray que tiene el tamaño del array desde la izquierda hasta la mitad + 1
    int firstArray = medium-left+1;
    //variable secondArray que tiene el tamaño del array desde la mitad hasta la derecha
    int secondArray = right-medium;
    //variable temporalLeft que guardará los valores del array desde la izquierda hasta la mitad
    //variable temporalRight que guardará los valores del array desde la mitad hasta la derecha
    int temporalLeft[firstArray], temporalRight[secondArray];
     for(int i=0; i<firstArray; i++){
        temporalLeft[i] = array[left+i];
     }
     for(int j=0; j<secondArray; j++){
        temporalRight[j] = array[medium+j+1];
     }

     int i=0;
     int j=0;
     //variable posición que empezará desde la posición izquierda de la parte del array que ajustaremos
     int posicion = left;

    //Se modifican las posiciones
     while(i<firstArray && j<secondArray){
        if(temporalLeft[i]< temporalRight[j]){
            array[posicion] = temporalLeft[i];
            i++;
        }else{
            array[posicion] = temporalRight[j];
            j++;
        }
        posicion++;
     }
    //Nos aseguramos de que no sobre nada en el subarray desde la izquieda hasta la mitad
     while(i<firstArray){
        array[posicion] = temporalLeft[i];
        i++;
        posicion++;
     }
    //Nos aseguramos de que no sobre nada en el subarray desde la mitad hasta la derecha
     while(j<secondArray){
        array[posicion] = temporalRight[j];
        j++;
        posicion++;
     }
}

/*
Funcion MergeSort que será usado para dividir el array a la mitad
- variable left que guarda el número de la posición desde la izquierda del array a ajustar
- variable right que guarda el número de la posición derecha del array a ajustar
- variable array que contiene el array a ajustar
*/
void MergeSort(int left, int right, int array[]){
    if(left<right){
        //variable medium que guarda el valor de la posición de la mitad del array
        int medium = left + ((right - left) / 2);

        //Volvemos a llamar a la función pero desde la izquierda hasta la mitad
        MergeSort(left, medium, array);
        //Volvemos a llamar a la función pero desde la mitad + 1  hasta la derecha
        MergeSort(medium+1, right, array);
        //Unimos las dos mitades
        Merge(left, right, medium, array);
    }
}

/*
Funcion PrintArray que imprime todos los valores del array
- variale size que guarda el tamaño del array
- variable array que contiene los datos del array a imprimir
*/
void PrintArray(int size, int array[]){
    for(int i = 0; i<size; i++){
        printf("%i ", array[i]);
    }
    printf("\n");
}

int main(){
    //variable array que tendrá el array a ajustar
    int array[] = {3453, 4468, 4267, 132, 1634, 3748, 3263, 2436, 3456, 4566, 4567, 4566};
    //variable size que contiene el tamaño del array
    int size = sizeof(array)/sizeof(array[0]);
    printf("Array desordenado: \n");
    PrintArray(size, array);
    printf("Array ordenado: \n");
    MergeSort(0, size-1, array);
    PrintArray(size, array);
    return 0;
}