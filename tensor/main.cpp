#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "fr.hpp"

int main() {
    Fr_init();    
    // 21877930911832249470409883743641463626721097223897723233065562074207719225345 8 349918102776089884323611807399201641453505767211409987349695659850434940975
    //              134, 246,  69,  59,   5,  130, 13,  30,  251, 132, 37, 192, 181, 234, 23,  98, 84,  80,  88,  108, 176, 182, 206, 68, 42,  37,  126,  33, 205,  79,  128, 27
    int input1[32] = {0, 0,   0,  0,   0,   0,  0,   0,  0,   0,  0,   0,  0,   0,  0,  0,   0,  0,   0,  0,   0,   0,  0,  0,  0,   0,   0,   0,   0,   0,   0,  8};
    int input2[32] = {0, 198, 11, 239, 202, 96, 203, 64, 215, 83, 148, 28, 221, 75, 93, 137, 34, 185, 50, 106, 152, 62, 29, 82, 185, 138, 122, 248, 116, 225, 16, 47};
    int array1_length = 3;
    int array2_length = 3;

    FrElement a = { 0, Fr_LONGMONTGOMERY, {1,1,1,1}};
    FrElement b = { 0, Fr_LONGMONTGOMERY, {2,2,2,2}};
    FrElement c;
    Fr_mul(&c,&a, &b);

    char *c1 = Fr_element2str(&c);
    printf("%s\n\n", c1);

    // int array1[array1_length-1][32] = {input1, input1, input1};
    // int array2[array2_length-1][32] = {input2, input2, input2};

    // int array1[array1_length-1][32];
    // int array2[array2_length-1][32];
    
    // FrElement b1[array1_length-1];
    // FrElement b2[array2_length-1];
    // FrElement b3[array1_length-1][array2_length-1];

    
    // for (int i=0; i < array1_length; i ++) {
    //     b1[i].type = Fr_LONGMONTGOMERY;
    //     b1[i].shortVal = 0;
    //     for (int j=0; j<Fr_N64; j++) {
    //         b1[i].longVal[j] = input1[j];
    //     }
    // }
    // for (int i=0; i < array2_length; i ++) {
    //     b2[i].type = Fr_LONGMONTGOMERY;
    //     b2[i].shortVal = 0;
    //     for (int j=0; j<Fr_N64; j++) {
    //         b2[i].longVal[j] = input2[j];
    //     }
    // }
    // #pragma omp parallel for
    // for (int k=0; k<array1_length; k++) {
    //     for (int j=0; j<array2_length; j++) {
    //         Fr_mul(&b3[k][j], &b2[j], &b1[k]);
    //     }
    // }
    

    // Fr_mul(&b3, &b2, &b1);
    // printf("b1: %ln \n", b1.longVal);
    // printf("b2: %ln \n", b2.longVal);
    
    // printf("result2: %lu, %lu, %lu, %lu\n", b3.longVal[0], b3.longVal[1], b3.longVal[2], b3.longVal[3]);
    // Fr_copyn(a2, a1, 10);
    
    // for (int i=0; i<10; i++) {
    //     char *c1 = Fr_element2str(&b3);
    //     printf("result1: %lu %lu %lu\n", b1.longVal[i], b2.longVal[i], c1);
    //     free(c1);
    //     // free(c2);
    // }

    // for (int k=0; k<array1_length; k++) {
    //     for (int j=0; j<array2_length; j++) {
    //         char *c1 = Fr_element2str(&b3[k][j]);
    //         printf("result: %s\n\n", c1);
    //         free(c1);
    //     }
    // }


//    FrElement err = { 0, Fr_LONGMONTGOMERY, {1,1,1,1}};
  //  Fr_toInt(&err);

    // printf("result3: %lu, %lu, %lu, %lu\n", c.longVal[0], c.longVal[1], c.longVal[2], c.longVal[3]);
}
