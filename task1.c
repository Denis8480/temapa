#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int lin, col;
} Coord;

typedef struct Node {
    Coord data;
    struct Node* next;
} Node;

typedef struct {
    Node* head;
} List;

typedef struct StackNode {
    List* changes;
    struct StackNode* next;
} StackNode;

typedef struct {
    StackNode* top;
} Stack;

char** aloc_matrice(int n, int m) {
    char **mat = (char **)malloc(n*sizeof(char*));
    for (int i = 0; i < n; i++) {
        mat[i] = (char *)malloc((m+1)*sizeof(char));
    }
    return mat;
}

void elib_mem(char ***mat, int n) {
    for (int i = 0; i < n; i++)
        free((*mat)[i]);
    free(*mat);
    *mat = NULL;
}

char** citire_fisier(const char *nume, int *n, int *m, int *t, int *k) {
    FILE *fisier = fopen(nume, "rt");
    (void)fscanf(fisier , "%d" , t);
    (void)fscanf(fisier,"%d %d" , n, m);
    (void)fscanf(fisier,"%d", k);
    char **mat = aloc_matrice(*n,*m);
    for (int i = 0; i < *n; i++)
        (void)fscanf(fisier,"%100s", mat[i]);
    fclose(fisier);
    return mat;
}

int calc_vecini(char **mat, int n, int m, int x, int y) {
    int cnt = 0;
    for (int i=-1; i<=1; i++)
        for (int j=-1; j<=1; j++)
            if (!(i == 0 && j == 0)) {
                int nx = x + i, ny = y + j;
                if (nx >= 0 && nx < n && ny >= 0 && ny < m && mat[nx][ny] == 'X')
                    cnt++;
            }
    return cnt;
}

char** generatie_urmatoare(char **mat, int n, int m) {
    char **temp = aloc_matrice(n, m);
    for (int i=0; i<n; i++)
        for (int j=0; j<m; j++) {
            int v = calc_vecini(mat, n, m, i, j);
            if (mat[i][j] == 'X')
                temp[i][j] = (v < 2 || v > 3) ? '+' : 'X';
            else
                temp[i][j] = (v == 3) ? 'X' : '+';
        }
    return temp;
}

void afisare(char **mat, int n, const char *nume) {
    FILE *fisier = fopen(nume, "a");
    for (int i = 0; i < n; i++)
        fprintf(fisier,"%s\n",mat[i]);
    fprintf(fisier,"\n");
    fclose(fisier);
}

void mat_initiala(char **mat, int n, const char *nume) {
    FILE *fisier = fopen(nume,"w");
    for (int i = 0; i < n; i++)
        fprintf(fisier,"%s\n",mat[i]);
    fprintf(fisier,"\n");
    fclose(fisier);
}

void adauga_ordonat(List* list, int l, int c) {
    Node *new_node = malloc(sizeof(Node)), *curr = list->head, *prev = NULL;
    new_node->data.lin = l; new_node->data.col = c; new_node->next = NULL;

    while (curr && (curr->data.lin < l || (curr->data.lin == l && curr->data.col < c))) {
        prev = curr;
        curr = curr->next;
    }

    if (!prev) {
        new_node->next = list->head;
        list->head = new_node;
    } else {
        prev->next = new_node;
        new_node->next = curr;
    }
}

List* compara_matrici(char **a, char **b, int n, int m) {
    List* list = malloc(sizeof(List));
    list->head = NULL;
    int diferenta=0;
    for (int i=0; i<n; i++)
        for (int j=0; j<m; j++)
            if (a[i][j] != b[i][j])
                {adauga_ordonat(list, i, j);
                    diferenta=1;}
    if (!diferenta) {
        free(list);
        return NULL;
    }
    return list;
}

void push(Stack *st, List *lst) {
    if (!lst) return;
    StackNode *node = malloc(sizeof(StackNode));
    node->changes = lst;
    node->next = st->top;
    st->top = node;
  
}

void free_list(List *list) {
    Node *curr = list->head;
    while (curr) {
        Node *next = curr->next;
        free(curr);
        curr = next;
    }
    free(list);
}

void free_stack(Stack *st) {
    StackNode *curr = st->top;
    while (curr) {
        StackNode *next = curr->next;
        if (curr->changes)
            free_list(curr->changes);
        free(curr);
        curr = next;
    }
    st->top = NULL;
}

void afisare_stiva(Stack *st, const char* out) {
  
    FILE *f = fopen(out, "w");
    if (!st->top) {
        fclose(f);  // creează fișier gol
        return;
    }
    // numărăm câte elemente are stiva
    int count = 0;
    StackNode *tmp = st->top;
    while (tmp) {
        count++;
        tmp = tmp->next;
    }

    // salvăm în vector pointerii către fiecare nod
    StackNode **vect = malloc(count * sizeof(StackNode*));
    tmp = st->top;
    for (int i = 0; i < count; i++) {
        vect[i] = tmp;
        tmp = tmp->next;
    }

    // parcurgem invers vectorul și scriem în fișier
    for (int i = count - 1; i >= 0; i--) {
        fprintf(f, "%d", count - i);
        if (vect[i] && vect[i]->changes) {
            Node *c = vect[i]->changes->head;
            while (c) {
                fprintf(f, " %d %d", c->data.lin, c->data.col);
                c = c->next;
            }
        }
        fprintf(f, "\n");
    }

    free(vect);
    fclose(f);
}

int main(int argc __attribute__((unused)), char** argv) {
    int N, M, T, K;
    char **mat = citire_fisier(argv[1], &N, &M, &T, &K);

    if (T == 1) {
        mat_initiala(mat, N, argv[2]);
        for (int i = 0; i < K; i++) {
            char **next = generatie_urmatoare(mat, N, M);
            elib_mem(&mat, N);
            mat = next;
            afisare(mat, N, argv[2]);
        }
    } else if (T == 2) {
        Stack stiva = {NULL};
        for (int i = 0; i < K; i++) {
            char **next = generatie_urmatoare(mat, N, M);
            List *changes = compara_matrici(mat, next, N, M);
            if (!changes) {
                elib_mem(&mat, N);
                mat = next;
                continue;
            }
            push(&stiva, changes);
            elib_mem(&mat, N);
            mat = next;
        }
        afisare_stiva(&stiva, argv[2]);
        free_stack(&stiva);
    }
    elib_mem(&mat, N);
    return 0;
}