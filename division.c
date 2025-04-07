#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include "division.h"

void append(List_t **head, int a, int b) {
    List_t *new_connection = (List_t*)malloc(sizeof(List_t));
    if (new_connection == NULL) {
        printf("Błąd alokacji pamięci!\n");
        exit(EXIT_FAILURE);
    }
    List_t *current = *head;
    new_connection->node1 = a;
    new_connection->node2 = b;
    new_connection->next = NULL;
    
    if(*head == NULL) {
        *head = new_connection;
        return;
    }

    while(current->next != NULL) {
        current = current->next;
    }

    current->next = new_connection;
}


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


int *count_gains(List_t *head, int nodes, int *groups) {
    int *gains = malloc(nodes * sizeof(int));
    for(int i = 0; i < nodes; i++) {
        int internal = 0;
        int external = 0;
        List_t *current = head;
        while(current != NULL) {
            if(current->node1 == i || current->node2 == i) {
                if(groups[current->node1] == groups[current->node2]){
                    internal++;
                }else{
                    external++;
                }
            }
            current=current->next;
        }

        gains[i] = external - internal;
    }

    return gains;
}

void append_changes(Changes_l **head, int a, int b, int gain) {
    Changes_l *new_pair = (Changes_l*)malloc(sizeof(Changes_l));
    if (new_pair == NULL) {
        printf("Błąd alokacji pamięci!\n");
        exit(EXIT_FAILURE);
    }
    Changes_l *current = *head;
    new_pair->node1 = a;
    new_pair->node2 = b;
    new_pair->gain = gain;
    new_pair->next = NULL;
    
    if(*head == NULL) {
        *head = new_pair;
        return;
    }

    while(current->next != NULL) {
        current = current->next;
    }

    current->next = new_pair;
}

void optimize_groups(int **groups, int nodes, int *gains, List_t *head, bool *locked) {
    Changes_l *changes = NULL;
    for(int i = 0; i < nodes; i++) {
        if(locked[i]) {
            continue;
        }
        for(int j = 0; j < nodes; j++){
            if(j == i || locked[j] || groups[i] == groups[j]) {
                continue;
            }

            int *original_groups = malloc(nodes * sizeof(int));
            memcpy(original_groups, *groups, nodes * sizeof(int));
            int *tmp_groups = malloc(nodes * sizeof(int));
            memcpy(tmp_groups, original_groups, nodes * sizeof(int));
            
            

            int *original_gains = count_gains(head, nodes, original_groups);
            
            
            int tmp = tmp_groups[i];
            tmp_groups[i] = tmp_groups[j];
            tmp_groups[j] = tmp;
            
            
            int *new_gains = count_gains(head, nodes, tmp_groups);
            
            int prev_gain = original_gains[i] + original_gains[j];
            int new_gain = new_gains[i] + new_gains[j];         

            if(new_gain < prev_gain) {
                append_changes(&changes, i, j, new_gain);
            }

            free(original_groups);
            free(tmp_groups);
            free(original_gains);
            free(new_gains);
            
        }
    }

    if(changes == NULL) {
        return;
    }else{
        Changes_l *current = changes;
        int best_gain = current->gain;
        int n1 = current->node1;
        int n2 = current->node2;
        while(current != NULL) {
            if(best_gain > current->gain) {
                best_gain = current->gain;
                n1 = current->node1;
                n2 = current->node2;
            }
            current=current->next;
        }

        locked[n1] = true;
        locked[n2] = true;

        int tmp = (*groups)[n1];
        (*groups)[n1] = (*groups)[n2];
        (*groups)[n2] = tmp;
        
        //printf("Dokonano zamiany %d z %d\n", n1, n2);
    }

    if(changes != NULL || all_locked(locked, nodes) == false) {
        optimize_groups(groups, nodes, gains, head, locked);
    }
}

bool all_locked(bool *locked, int nodes) {
    for(int i=0; i < nodes; i++) {
        if(!locked[i]) {
            return false;
        }
    }

    return true;
}


int count_ext(List_t *head, int nodes, int *groups) {
    int sum = 0;
    List_t *current = head;
    while(current != NULL) {
        if(groups[current->node1] != groups[current->node2]){
            sum++;
        }
        current=current->next;
    }

    return sum;
}

int count_int(List_t *head, int nodes, int *groups, int group_id) {
    int sum = 0;
    List_t *current = head;
    while(current != NULL) {
        if(groups[current->node1] == group_id && groups[current->node1] == groups[current->node2]){
            sum++;
        }
        current=current->next;
    }

    return sum;
}

