#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

int *make_groups(int nodes, int groups_n) {
    int *groups = malloc(nodes * sizeof(int));
    srand(time(NULL));
    int max_nodes = nodes / groups_n + (nodes % groups_n);
    int *nodes_in_group = calloc(groups_n, sizeof(int));

    for(int i=0; i < nodes; i++) {
        while(1) {
            int g = rand() % groups_n;
            if(nodes_in_group[g] < max_nodes) {
                nodes_in_group[g]++;
                groups[i] = g;
                break;
            }
        }
    }
    
    return groups;
}

void print_groups(int *groups, int nodes, int groups_n) {
    for(int i = 0; i < groups_n; i++) {
        printf("Grupa %d: ", i+1);
        for(int j = 0; j < nodes; j++) {
            if(groups[j] == i) {
                printf("%d, ", j);
            }
        }
        printf("\n");
    }
}


