#include<stdio.h>

union foo {
        int i;
        char c[sizeof(int)];
};

int main() {
        union foo f;
        printf("size of foo %d \n", sizeof (f));
        f.i = 1;
        if(f.c[0] == 1) {
                printf("Little endian\n");
        } else {
                printf("Big Endian \n");
        }
        return 0;
}
