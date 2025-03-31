#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define BASE_SIZE 256
#define MAX_LINE 1000000

int read_line(char* line, int** numbers){
    char* token;
    int number;
    int count = 0;
    int capacity = BASE_SIZE;
    token = strtok(line, ";");
    *numbers = malloc(capacity * sizeof(int)); // Poprawiona alokacja pamięci
    if (*numbers == NULL) {
        perror("Błąd alokacji pamięci");
        exit(EXIT_FAILURE);
    }
    while (token != NULL) {
        // Przekształć token na liczbę
        if (sscanf(token, "%d", &number) == 1) {
            // Sprawdź czy to liczba naturalna (dodatnia)
            if (number >= 0) {
                // Sprawdź, czy tablica ma wystarczającą pojemność
                if (count >= capacity) {
                    capacity *= 2; // Podwajamy pojemność
                    printf("\n%d\n",capacity);
                    numbers = realloc(numbers, capacity * sizeof(int));
                    if (numbers == NULL) {
                        perror("Błąd alokacji pamięci");
                    }
                }
                // Zapisz liczbę do tablicy
                (*numbers)[count++] = number;
            } else {
                printf("Błąd: oczekiwana liczba naturalna, ale znaleziono %d\n", number);
            }
        } else {
            printf("Błąd: Nie udało się przekształcić '%s' na liczbę\n", token);
        }
        // Pobierz następny token
        token = strtok(NULL, ";");
    }
    return count;
}

void write_tab(int* tab, int roz){
    for(int i = 0; i<roz; i++){
        printf("%d,",tab[i]);
    }
    printf("\n");
}

int max_index_in_row(int *tab, int n){
    int maxi = 0;
    for(int i =0; i < n; i++){
        if(tab[i]>maxi){
            maxi = tab[i];
        }
    }
    return maxi+1;
}

void print_graph(bool **graph, int n, int m) {
    for (int i = 0; i < n; i++) {     
        printf("[");
        for (int j = 0; j < m; j++) {
            if(j+1 == m){
                printf("%d.",graph[i][j]);  
            }
            else{
        	    printf("%d. ",graph[i][j]);  
            }
        }
        printf("]\n");
    }
}

typedef struct List_elem{
    int node1;
    int node2;
    struct List_elem *next;
}List_t;

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

void connection_list(List_t **head, int *groups, int n1, int *first_nodes, int n2) {
    int i;
    for( i = 0; i < n2 - 1; i++) {
        int a = groups[first_nodes[i]];
        for(int j = first_nodes[i] + 1; j < first_nodes[i+1]; j++) {
            int b = groups[j];
            append(head, a, b);
        }
    }

    if(i == n2-1) {
        int a = groups[first_nodes[i]];
        for(int j = first_nodes[i] + 1; j < n1; j++) {
            int b = groups[j];
            append(head, a, b);
        }
    }
    
}

void print_list(List_t *head) {
    List_t *current = head;
    while(current != NULL) {
        printf("%d - %d\n", current->node1, current->node2);
        current = current->next;
    }
}

int main(int argc, char **argv){
    int* tab1 = malloc(sizeof(int));
    int* tab2;
    int* tab3;
    int* tab4;
    int* tab5;
    List_t *head = NULL;
    int n1 = 1;
    int n2 = 0;
    int n3 = 0;
    int n4 = 0;
    int n5 = 0;

    char line[MAX_LINE];

    FILE *input = fopen(argv[1], "r");

    if(input == NULL) {
        printf("Nie udało się otworzyć pliku!\n");
        fclose(input);
        return 1;
    }

    char* token;
    int number = 0;
    fgets(line, sizeof(line), input);
    sscanf(line, "%d", &number);
    tab1[0] = number;
    printf("Tablica 1: ");
    write_tab(tab1,n1);

    fgets(line, sizeof(line), input);
    n2 = read_line(line,&tab2);
    printf("Tablica 2: ");
    write_tab(tab2,n2);

    fgets(line, sizeof(line), input);
    n3 = read_line(line,&tab3);
    printf("Tablica 3: ");
    write_tab(tab3,n3);

    fgets(line, sizeof(line), input);
    n4 = read_line(line,&tab4);
    printf("Tablica 4: ");
    write_tab(tab4,n4);

    fgets(line, sizeof(line), input);
    n5 = read_line(line,&tab5);
    printf("Tablica 5: ");
    write_tab(tab5,n5);

    fclose(input);

    int maxi = max_index_in_row(tab2,n2); //ilosc kolumn

    int n = n3-1; //ilosc wierszy

    bool** graph = malloc(sizeof(bool*) * n);
    for(int i = 0; i < n; i++){
        graph[i] = calloc(maxi, sizeof(bool));
    }

    int j = 0;
    for(int i = 0; i < n; i++){
        while (j < n2 - 1 && tab3[i] != tab3[i + 1] && tab2[j] < tab2[j + 1]) {
            graph[i][tab2[j]] = 1;
            j++;
        }
        if (j < n2) {
            graph[i][tab2[j]] = 1;
            j++;
        }
    }

    print_graph(graph, n, maxi);

    connection_list(&head, tab4, n4, tab5, n5);
    print_list(head);

    free(tab1);
    free(tab2);
    free(tab3);
    free(tab4);
    free(tab5);
    for (int i = 0; i < n; i++) {
        free(graph[i]);
    }
    free(graph);

}