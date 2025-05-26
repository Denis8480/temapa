#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    int lin, col;
} Coord;

typedef struct Node
{
    Coord data;
    struct Node *next;
} Node;

typedef struct
{
    Node *head;
} List;

typedef struct StackNode
{
    List *changes;
    struct StackNode *next;
} StackNode;

typedef struct
{
    StackNode *top;
} Stack;

char **aloc_matrice(int n, int m)
{
    char **mat = (char **)malloc(n * sizeof(char *));
    for (int i = 0; i < n; i++)
    {
        mat[i] = (char *)malloc((m + 1) * sizeof(char));
    }
    return mat;
}

void elib_mem(char ***mat, int n)
{
    for (int i = 0; i < n; i++)
        free((*mat)[i]);
    free(*mat);
    *mat = NULL;
}

char **citire_fisier(const char *nume, int *n, int *m, int *t, int *k)
{
    FILE *fisier = fopen(nume, "rt");
    (void)fscanf(fisier, "%d", t);
    (void)fscanf(fisier, "%d %d", n, m);
    (void)fscanf(fisier, "%d", k);
    char **mat = aloc_matrice(*n, *m);
    for (int i = 0; i < *n; i++)
        {(void)fscanf(fisier, "%s", mat[i]);
        mat[i][*m] = '\0'; }
    fclose(fisier);
    return mat;
}

int calc_vecini(char **mat, int n, int m, int x, int y)
{
    int cnt = 0;
    for (int i = -1; i <= 1; i++)
        for (int j = -1; j <= 1; j++)
            if (!(i == 0 && j == 0))
            {
                int nx = x + i, ny = y + j;
                if (nx >= 0 && nx < n && ny >= 0 && ny < m && mat[nx][ny] == 'X')
                    cnt++;
            }
    return cnt;
}

char **generatie_urmatoare(char **mat, int n, int m)
{
    char **temp = aloc_matrice(n, m);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
        {
            int v = calc_vecini(mat, n, m, i, j);
            if (mat[i][j] == 'X')
            {
                if (v < 2 || v > 3)
                    temp[i][j] = '+';
                else
                    temp[i][j] = 'X';
            }
            else
            {
                if (v == 3)
                    temp[i][j] = 'X';
                else
                    temp[i][j] = '+';
            }
        }
    return temp;
}
void afisare(char **mat, int n, const char *nume)
{
    FILE *fisier = fopen(nume, "a");
    for (int i = 0; i < n; i++)
        fprintf(fisier, "%s\n", mat[i]);
    fprintf(fisier, "\n");
    fclose(fisier);
}

void mat_initiala(char **mat, int n, const char *nume)
{
    FILE *fisier = fopen(nume, "w");
    for (int i = 0; i < n; i++)
        fprintf(fisier, "%s\n", mat[i]);
    fprintf(fisier, "\n");
    fclose(fisier);
}

void adauga_ordonat(List *list, int l, int c)
{
    Node *new_node = malloc(sizeof(Node)), *curr = list->head, *prev = NULL;
    new_node->data.lin = l;
    new_node->data.col = c;
    new_node->next = NULL;

    while (curr && (curr->data.lin < l || (curr->data.lin == l && curr->data.col < c)))
    {
        prev = curr;
        curr = curr->next;
    }

    if (!prev)
    {
        new_node->next = list->head;
        list->head = new_node;
    }
    else
    {
        prev->next = new_node;
        new_node->next = curr;
    }
}

List *compara_matrici(char **a, char **b, int n, int m)
{
    List *list = malloc(sizeof(List));
    list->head = NULL;
    int diferenta = 0;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            if (a[i][j] != b[i][j])
            {
                adauga_ordonat(list, i, j);
                diferenta = 1;
            }
    if (!diferenta)
    {
        free(list);
        return NULL;
    }
    return list;
}

void push(Stack *st, List *lst)
{
    if (!lst)
        return;
    StackNode *node = malloc(sizeof(StackNode));
    node->changes = lst;
    node->next = st->top;
    st->top = node;
}

void free_list(List *list)
{
    Node *curr = list->head;
    while (curr)
    {
        Node *next = curr->next;
        free(curr);
        curr = next;
    }
    free(list);
}

void free_stack(Stack *st)
{
    StackNode *curr = st->top;
    while (curr)
    {
        StackNode *next = curr->next;
        if (curr->changes)
            free_list(curr->changes);
        free(curr);
        curr = next;
    }
    st->top = NULL;
}

void afisare_stiva(Stack *st, const char *out)
{

    FILE *f = fopen(out, "w");
    if (!st->top)
    {
        fclose(f); // creează fișier gol
        return;
    }
    // numărăm câte elemente are stiva
    int count = 0;
    StackNode *tmp = st->top;
    while (tmp)
    {
        count++;
        tmp = tmp->next;
    }

    // salvăm în vector pointerii către fiecare nod
    StackNode **vect = malloc(count * sizeof(StackNode *));
    tmp = st->top;
    for (int i = 0; i < count; i++)
    {
        vect[i] = tmp;
        tmp = tmp->next;
    }

    // parcurgem invers vectorul și scriem în fișier
    for (int i = count - 1; i >= 0; i--)
    {
        fprintf(f, "%d", count - i);
        if (vect[i] && vect[i]->changes)
        {
            Node *c = vect[i]->changes->head;
            while (c)
            {
                fprintf(f, " %d %d", c->data.lin, c->data.col);
                c = c->next;
            }
        }
        fprintf(f, "\n");
    }

    free(vect);
    fclose(f);
}
char **generatieb(char **mat, int a, int b)
{
    char **q = aloc_matrice(a, b);
    for (int i = 0; i < a; i++) {
        for (int j = 0; j < b; j++) {
            int p = calc_vecini(mat, a, b, i, j);
            if (p == 2)
                q[i][j] = 'X';  // devine sau rămâne vie
            else
                q[i][j] = mat[i][j];  // rămâne neschimbată
        }
        q[i][b] = '\0';
    }
    return q;
}
typedef struct ArboreNode
{
    List *modificari;
    char **matrice;
    struct ArboreNode *st, *dr;
} ArboreNode;

void construieste(ArboreNode *nod, int nivel, int K, int N, int M)
{
    if (nivel == K)
        return;

    char **gen_b = generatieb(nod->matrice, N, M);
    char **gen   = generatie_urmatoare(nod->matrice, N, M);

    List *modB = compara_matrici(nod->matrice, gen_b, N, M);
    List *mod  = compara_matrici(nod->matrice, gen,   N, M);

   

    // RAMURA STÂNGA (regula b) — construiește oricum
    ArboreNode *st = malloc(sizeof(ArboreNode));
    st->matrice = gen_b;
    st->modificari = modB;
    st->st = st->dr = NULL;
    nod->st = st;
    construieste(st, nivel + 1, K, N, M);
     
     // RAMURA DREAPTA (Game of Life) — construiește oricum
    ArboreNode *dr = malloc(sizeof(ArboreNode));
    dr->matrice = gen;
    dr->modificari = mod;
    dr->st = dr->dr = NULL;
    nod->dr = dr;
    construieste(dr, nivel + 1, K, N, M);
}
void afisare_arbore_preordine(ArboreNode *nod, int n, const char *out)
{
    if (!nod)
        return;
    afisare(nod->matrice, n, out);
    afisare_arbore_preordine(nod->st, n, out); // apoi regula b
    afisare_arbore_preordine(nod->dr, n, out); // întâi Game of Life
    
}
void elibereaza(ArboreNode *nod, int N)
{
    if (!nod)
        return;
    elibereaza(nod->st, N);
    elibereaza(nod->dr, N);
    if (nod->matrice)
        elib_mem(&nod->matrice, N);
    if (nod->modificari)
        free_list(nod->modificari);
    free(nod);
}
char** copie_matrice(char** src, int n, int m) {
    char **d = aloc_matrice(n, m);
    for (int i = 0; i < n; i++)
        memcpy(d[i], src[i], m + 1);
    return d;
}
typedef struct {
    Coord *v;
    int **adj;
    int n;
} Graf;

Graf *matrice_in_graf(char **mat, int N, int M) {
    Graf *g = malloc(sizeof(Graf));
    g->v = malloc(N * M * sizeof(Coord));
    g->adj = malloc(N * M * sizeof(int *));
    for (int i = 0; i < N * M; i++)
        g->adj[i] = calloc(N * M, sizeof(int));

    int id = 0;
    int map[25][25];
    memset(map, -1, sizeof(map));

    for (int i = 0; i < N; i++)
        for (int j = 0; j < M; j++)
            if (mat[i][j] == 'X') {
                g->v[id].lin = i;
                g->v[id].col = j;
                map[i][j] = id++;
            }

    g->n = id;

    const int di[] = {-1, -1, -1, 0, 0, 1, 1, 1};
    const int dj[] = {-1, 0, 1, -1, 1, -1, 0, 1};

    for (int k = 0; k < g->n; k++) {
        int i = g->v[k].lin, j = g->v[k].col;
        for (int d = 0; d < 8; d++) {
            int ni = i + di[d], nj = j + dj[d];
            if (ni >= 0 && ni < N && nj >= 0 && nj < M && map[ni][nj] != -1)
                g->adj[k][map[ni][nj]] = 1;
        }
    }

    return g;
}

void dfs_componenta(Graf *g, int *visited, int *component, int *size, int node) {
    visited[node] = 1;
    component[(*size)++] = node;
    for (int i = 0; i < g->n; i++)
        if (!visited[i] && g->adj[node][i])
            dfs_componenta(g, visited, component, size, i);
}

void greedy_lant(Graf *g, int real_start_id, int const *component, int comp_size, int *lant, int *lungime) {
    int start_idx = -1;
    for (int i = 0; i < comp_size; i++) {
        if (component[i] == real_start_id) {
            start_idx = i;
            break;
        }
    }
    if (start_idx == -1) return;

    int *viz = calloc(comp_size, sizeof(int));
    int poz = 0;
    lant[poz++] = start_idx;
    viz[start_idx] = 1;

    while (1) {
        int last = lant[poz - 1];
        int next = -1;
        Coord best_coord = (Coord){1000, 1000};

        for (int i = 0; i < comp_size; i++) {
            if (viz[i]) continue;
            if (g->adj[component[last]][component[i]]) {
                Coord c = g->v[component[i]];
                if (c.lin < best_coord.lin || (c.lin == best_coord.lin && c.col < best_coord.col)) {
                    best_coord = c;
                    next = i;
                }
            }
        }

        if (next == -1) break;
        lant[poz++] = next;
        viz[next] = 1;
    }

    *lungime = poz;
    free(viz);
}
int compare_coord(Graf *g, int const *component, int const *lant1, int const *lant2, int len) {
    for (int i = 0; i < len; i++) {
        Coord a = g->v[component[lant1[i]]];
        Coord b = g->v[component[lant2[i]]];
        if (a.lin != b.lin) return a.lin - b.lin;
        if (a.col != b.col) return a.col - b.col;
    }
    return 0;
}
void cel_mai_lung_lant(Graf *g, FILE *f) {
    if (g->n == 0) return;

    int *visited = calloc(g->n, sizeof(int));

    for (int i = 0; i < g->n; i++) {
        if (!visited[i]) {
            int *component = malloc(g->n * sizeof(int));
            int size = 0;
            dfs_componenta(g, visited, component, &size, i);

            int *lant = malloc(size * sizeof(int));
            int *best_lant_local = malloc(size * sizeof(int));
            int best_len = 0;

            for (int j = 0; j < size; j++) {
                int l = 0;
                greedy_lant(g, component[j], component, size, lant, &l);

                if (l > best_len ||
                   (l == best_len &&
                    compare_coord(g, component, lant, best_lant_local, l)<0)) {
                    best_len = l;
                    memcpy(best_lant_local, lant, l * sizeof(int));
                }
            }

            if (best_len >= 2) {
                fprintf(f, "%d\n", best_len - 1);
                for (int k = 0; k < best_len; k++) {
                    int idx = component[best_lant_local[k]];
                    fprintf(f, "(%d,%d)", g->v[idx].lin, g->v[idx].col);
                    if (k < best_len - 1) fprintf(f, " ");
                }
                fprintf(f, "\n");
            }

            free(component);
            free(lant);
            free(best_lant_local);
        }
    }

    free(visited);
}
void parcurgeretask4(ArboreNode *nod, int N, int M, FILE *f) {
    if (!nod) return;
    Graf *g = matrice_in_graf(nod->matrice, N, M);
    cel_mai_lung_lant(g, f);
    for (int i = 0; i < N * M; i++) free(g->adj[i]);
    free(g->adj); free(g->v); free(g);
    parcurgeretask4(nod->st, N, M, f);
    parcurgeretask4(nod->dr, N, M, f);
}
int main(int argc __attribute__((unused)), char **argv)
{
    int N, M, T, K;
    char **mat = citire_fisier(argv[1], &N, &M, &T, &K);

    if (T == 1)
    {
        mat_initiala(mat, N, argv[2]);
        for (int i = 0; i < K; i++)
        {
            char **next = generatie_urmatoare(mat, N, M);
            elib_mem(&mat, N);
            mat = next;
            afisare(mat, N, argv[2]);
        }
        elib_mem(&mat, N);
    }
    else if (T == 2)
    {
        Stack stiva = {NULL};
        for (int i = 0; i < K; i++)
        {
            char **next = generatie_urmatoare(mat, N, M);
            List *changes = compara_matrici(mat, next, N, M);
            if (!changes)
            {
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
        elib_mem(&mat, N);
    }
    else if (T == 3)
    {
        FILE *f = fopen(argv[2], "w");
        fclose(f);
        ArboreNode *radacina = malloc(sizeof(ArboreNode));
        radacina->matrice = copie_matrice(mat, N, M);
        elib_mem(&mat, N);
        radacina->modificari = NULL;
        radacina->st = radacina->dr = NULL;
        construieste(radacina, 0, K, N, M);
        afisare_arbore_preordine(radacina, N, argv[2]);
        elibereaza(radacina, N);
    }
    else if (T == 4)
{
    FILE *f = fopen(argv[2], "w");
    fclose(f);
    ArboreNode *radacina = malloc(sizeof(ArboreNode));
    radacina->matrice = copie_matrice(mat, N, M);
    elib_mem(&mat, N);
    radacina->modificari = NULL;
    radacina->st = radacina->dr = NULL;
    construieste(radacina, 0, K, N, M);
    f = fopen(argv[2], "a");
    parcurgeretask4(radacina, N, M, f);
    fclose(f);
    elibereaza(radacina, N);
}
    return 0;
}