#include "set.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

Set set_empty(void) {
    Set x = 0;
    return x;
}

Set set_universal(void) {
    Set x = 0xFFFFFFFF;
    return x;
}

Set set_insert(Set s, uint8_t x) {
    s = s | (1 << x);
    return s;
}

Set set_remove(Set s, uint8_t x) {
    s = s & ~(1 << x);
    return s;
}

bool set_member(Set s, uint8_t x) {
    if (s & (1 << x)) { //if bit x does not exist, result is 0, and thus this is false
        return true;
    }
    return false;
}

Set set_union(Set s, Set t) {
    return (s | t);
}

Set set_intersect(Set s, Set t) {
    return (s & t);
}

Set set_difference(Set s, Set t) {
    return (s & (~t));
}

Set set_complement(Set s) {
    return (set_universal() & (~s));
}
