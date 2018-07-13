#include <sys/mman.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <unistd.h>
#include <time.h>


int main(int argc, char **argv) {
    if (argc < 3) {
        printf("syntax error\n");
        return 1;
    }

    size_t length = (size_t) atoi(argv[1]) * 1024 * 1024;
    int enable_hugepages = atoi(argv[2]);

    size_t alignment = 2 * 1024 * 1024;
    uint8_t *data;

    printf("allocating memory...\n");
    
    if ((data = aligned_alloc(alignment, length)) == NULL) {
        perror("aligned_alloc");
        return 1;
    }

    printf("%" PRIu64 " allocated at: %p\n", (uint64_t) length, data);

    if (enable_hugepages) {
        printf("requesting hugepages...\n");

        if (madvise(data, length, MADV_HUGEPAGE) != 0)
        {
            perror("madvise");
            return 1;
        }
    }
    else {
        printf("disabling hugepages...\n");

        if (madvise(data, length, MADV_NOHUGEPAGE) != 0)
        {
            perror("madvise");
            return 1;
        }
    }

    printf("touching pages...\n");

    for (size_t i = 0; i < length; i += 4096)
        data[i] = 1;

    printf("running test...\n");

    struct timespec start, stop;

    if (clock_gettime(CLOCK_THREAD_CPUTIME_ID, &start) != 0) {
        perror("clock_gettime");
        return 1;
    }

    for (uint64_t j = 0; j < 4398046511104ull / length; j++)
        for (size_t i = 0; i < length; i += 4096)
            data[i] = j;
    
    if (clock_gettime(CLOCK_THREAD_CPUTIME_ID, &stop) != 0) {
        perror("clock_gettime");
        return 1;
    }

    uint64_t elapsed = (stop.tv_sec * 1000000000ll + stop.tv_nsec) - (start.tv_sec * 1000000000ll + start.tv_nsec);

    printf("test complete in: %0.03fs\n", elapsed / 1000000000.0);
    
    //printf("press enter to exit...\n");
    //getchar();

    return 0;
}