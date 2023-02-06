#include "gaps.h"
#include "stats.h"

#include <stdint.h>
#include <stdio.h>

/*My implementation of shell sort. Follows closely to python psuedo code
 * provided in assignment spec.
 */
void shell_sort(Stats *stats, uint32_t *arr, uint32_t length) {
    uint32_t index = 0; //index for iterating through gaps[]
    while (1) {
        for (uint32_t i = gaps[index]; i < length; i++) {
            uint32_t j = i;
            uint32_t temp = move(
                stats, arr[i]); //move() is used here since arr element being moved to temp var
            while (j >= gaps[index] && cmp(stats, temp, arr[j - gaps[index]]) < 0) {
                arr[j] = move(stats, arr[j - gaps[index]]);
                j = j - gaps[index];
            }
            arr[j] = move(stats, temp);
        }
        if (gaps[index] == 1) {
            break; //this conditional checks if the final value of the gap sequence has been reached
                //This works since final value is always 1
        }
        index++;
    }
    return;
}
