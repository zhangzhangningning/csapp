#include <stdint.h>

uint64_t sum(uint64_t n) {
    if (n == 0) {
        return 0;
    } else {
        return n + sum(n - 1);
    }
}

int main() {
    uint64_t c = sum(3);
    return 0;
}