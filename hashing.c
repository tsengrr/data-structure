#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void insert(long hash[100][20], int n, int m, long value) {
    int bucket = value % n;
    for (int i = 0; i < n; i++) {
        int currentBucket = (bucket + i) % n;  // Linear probing
        for (int j = 0; j < m; j++) {
            if (hash[currentBucket][j] == -1) {
                hash[currentBucket][j] = value;
                return;
            }
        }
    }
}

void delete(long hash[100][20], int n, int m, long value) {
    int bucket = value % n;
    for (int i = 0; i < n; i++) {
        int currentBucket = (bucket + i) % n;  // Linear probing
        for (int j = 0; j < m; j++) {
            if (hash[currentBucket][j] == value) {
                hash[currentBucket][j] = -1;
                return;
            }
        }
    }
}

void search(long hash[100][20], int n, int m, long value) {
    int bucket = value % n;
    for (int i = 0; i < n; i++) {
        int currentBucket = (bucket + i) % n;  // Linear probing
        for (int j = 0; j < m; j++) {
            if (hash[currentBucket][j] == value) {
                printf("%d %d\n", currentBucket, j);
                return;
            }
        }
    }
}

int main() {
    int n, m;
    char input[100];
    long hash[100][20];

    // Initialize hash table
    for (int i = 0; i < 100; i++) {
        for (int j = 0; j < 20; j++) {
            hash[i][j] = -1;
        }
    }

    // Read bucket and slot values
    scanf("bucket %d\n", &n);
    scanf("slot %d\n", &m);

    // Process commands
    while (fgets(input, sizeof(input), stdin)) {

       // printf("%s\n", input);
        char command[100];
        long value;

        if (sscanf(input, "%s %ld", command, &value) == 1 && strcmp(command, "exit") == 0) {
            break;
        }

        if (strcmp(command, "insert") == 0) {
            insert(hash, n, m, value);
        } else if (strcmp(command, "delete") == 0) {
            delete(hash, n, m, value);
        } else if (strcmp(command, "search") == 0) {
            search(hash, n, m, value);
        }
    }

    return 0;
}