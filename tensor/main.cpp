#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "fr.hpp"

int main() {
    Fr_init();    
    // 21877930911832249470409883743641463626721097223897723233065562074207719225345 8 349918102776089884323611807399201641453505767211409987349695659850434940975
    //              134, 246,  69,  59,   5,  130, 13,  30,  251, 132, 37, 192, 181, 234, 23,  98, 84,  80,  88,  108, 176, 182, 206, 68, 42,  37,  126,  33, 205,  79,  128, 27
    // int input1[32] = {0, 0,   0,  0,   0,   0,  0,   0,  0,   0,  0,   0,  0,   0,  0,  0,   0,  0,   0,  0,   0,   0,  0,  0,  0,   0,   0,   0,   0,   0,   0,  8};
    // int input2[32] = {0, 198, 11, 239, 202, 96, 203, 64, 215, 83, 148, 28, 221, 75, 93, 137, 34, 185, 50, 106, 152, 62, 29, 82, 185, 138, 122, 248, 116, 225, 16, 47};
    // int array1_length = 3;
    // int array2_length = 3;

    FrElement a ;
    FrElement b ;
    a.type = Fr_LONGMONTGOMERY;
    b.type = Fr_LONGMONTGOMERY;
    FrElement c;
    FrElement d;
    for (int i=0; i<4 ; i++) {
        a.longVal[i] = 0x1;
        b.longVal[i] = 0x2;
    }
    Fr_rawMMul(c.longVal, a.longVal, b.longVal);
    Fr_mul(&d, &a, &b);
    // char *c1 = Fr_element2str(&c);
    // printf("%s\n\n", c1);
    printf("res d: %lu \n", d.longVal);
    printf("res c: %lu \n", c.longVal);

    FrElement x;
    Fr_toLongNormal(&x, &a);
    int g = Fr_toInt(&x);
    printf("res: %d\n",  g);

    // int tests[7] = { 0, 1, 2, -1, -2, 0x7FFFFFFF, (int)0x80000000};
    // for (int i=0; i<7;i++) {
    //     FrElement h = { tests[i], Fr_SHORT, {0,0,0,0}};
    //     FrElement x;
    //     Fr_toLongNormal(&x, &h);
    //     int k = Fr_toInt(&x);
    //     int j = Fr_isTrue(&x);
    //     printf("%d, %d, %d\n", tests[i], k, j);
    // }
}
