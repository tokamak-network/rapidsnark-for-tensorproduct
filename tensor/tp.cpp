#include "tp.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "fr.hpp"

void tensorProduct(
    int arr1[1024][1][32],
    int arr2[1][32][32],
    int arr1_size,
    int arr2_size
) {
    Fr_init();
    
    int array1[arr1_size - 1][32];
    int array2[arr2_size - 1][32];

    FrElement FrArr1[arr1_size - 1];
    FrElement FrArr2[arr2_size - 1];
    FrElement product[arr1_size - 1][arr2_size - 1];

    for (int i = 0; i < arr1_size; i ++) {
        FrArr1[i].type = Fr_LONGMONTGOMERY;
        FrArr1[i].shortVal = 0;
        for (int j=0; j<Fr_N64; j++) {
            FrArr1[i].longVal[j] = arr1[i][0][j];
        }
    }

    for (int i = 0; i < arr2_size; i ++) {
        FrArr2[i].type = Fr_LONGMONTGOMERY;
        FrArr2[i].shortVal = 0;
        for (int j=0; j<Fr_N64; j++) {
            FrArr2[i].longVal[j] = arr2[i][0][j];
        }
    }

    #pragma omp parallel for
    for (int i=0; i < arr1_size; i++) {
        for (int j=0; j < arr2_size; j++) {
            Fr_mul(&product[i][j], &FrArr2[j], &FrArr1[i]);
        }
    }
<<<<<<< HEAD
}
=======
}
>>>>>>> defba619ac119d3c843aeef9d86613e6a1299e10
