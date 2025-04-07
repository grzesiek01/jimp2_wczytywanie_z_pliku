#ifndef DIVISION_H
#define DIVISION_H

typedef struct List_elem{
    int node1;
    int node2;
    struct List_elem *next;
}List_t;

typedef struct Changes_list{
    int node1;
    int node2;
    int gain;
    struct Changes_list *next;
}Changes_l;

int *make_groups(int nodes, int groups_n);
void print_groups(int *groups, int nodes, int groups_n);
void append(List_t **head, int a, int b);
int* count_gains(List_t *head, int nodes, int *groups);
void append_changes(Changes_l **head, int a, int b, int gain);
void optimize_groups(int **groups, int nodes, int *gains, List_t *head, bool *locked);
bool all_locked(bool *locked, int nodes);
int count_ext(List_t *head, int nodes, int *groups);
int count_int(List_t *head, int nodes, int *groups, int group_id);

#endif