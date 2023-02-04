#include <stdio.h>
#include <stdint.h>
#include "stats.h"
#include "gaps.h"

/*My implementation of shell sort. Follows closely to python psuedo code
 * provided in assignment spec.
 */
void shell_sort(Stats *stats, uint32_t *arr, uint32_t length){
    uint32_t index = 0; //index for iterating through gaps[]
    while (1){
        for (uint32_t i = gaps[index]; i < length; i++) {
            uint32_t j = i;
            uint32_t temp = move(stats, arr[i]); //move() is used here since arr element being moved to temp var
            while (j >= gaps[index] && temp < arr[j-gaps[index]]){ //use cmp later
                arr[j] = move(stats, arr[j - gaps[index]]);
                j = j - gaps[index];
            }
            arr[j] = temp;
        }
        if (gaps[index] == 1){
            break; //this conditional checks if the final value of the gap sequence has been reached
                   //This works since final value is always 1
        }
        index++;
    }
    return;
}

//main for testing puposes
int main(void){
    Stats stat;
    uint32_t arr[] = {3,4,7,2,1,5,6,14,11,13,12,16,20,9,8};
    uint32_t length = 15;
    shell_sort(&stat, arr, length);

    for(uint32_t i = 0; i < length; i++) {
        printf("%u\n", arr[i]);
    }
    return 0;
}
