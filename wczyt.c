#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define BASE_SIZE 256
#define MAX_LINE 1000000

int read_line(char* line, int* numbers){
    char* token;
    int number;
    int count = 0;
    int capacity = BASE_SIZE;
    token = strtok(line, ";");
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
                numbers[count++] = number;
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

int main(int argc, char **argv){
    int* tab1 = malloc(sizeof(int));
    int* tab2 = malloc(sizeof(int)*BASE_SIZE);
    int* tab3 = malloc(sizeof(int)*BASE_SIZE);
    int* tab4 = malloc(sizeof(int)*BASE_SIZE);
    int* tab5 = malloc(sizeof(int)*BASE_SIZE);

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
    n2 = read_line(line,tab2);
    printf("Tablica 2: ");
    write_tab(tab2,n2);

    fgets(line, sizeof(line), input);
    n3 = read_line(line,tab3);
    printf("Tablica 3: ");
    write_tab(tab3,n3);

    fgets(line, sizeof(line), input);
    n4 = read_line(line,tab4);
    printf("Tablica 4: ");
    write_tab(tab4,n4);

    fgets(line, sizeof(line), input);
    n5 = read_line(line,tab5);
    printf("Tablica 5: ");
    write_tab(tab5,n5);

    int maxi = max_index_in_row(tab2,n2); //ilosc kolumn

    int n = n3-1; //ilosc wierszy

    bool** graph = malloc(sizeof(int*)*n); 
    int j = 0;
    for(int i=0; i<n; i++){
        bool* row = malloc(sizeof(int)*maxi);
        for(int k = 0; k<maxi;k++){
            row[k] = 0;
        }
        if(tab3[i] != tab3[i+1]){
            while(tab2[j] < tab2[j+1]){
                row[tab2[j]] = 1; 
                j++;
            }
            if(tab2[j] > tab2[j+1]){
                row[tab2[j]] = 1;
                j++;
            }
        }
        graph[i] = row;
    }
    print_graph(graph,n,maxi);

    

}