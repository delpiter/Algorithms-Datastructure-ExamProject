/*
 * Progetto di Laboratorio Algoritmi e Strutture Dati
 * Università di Bologna, corso di laurea in Ingegneria e Scienze Informatiche
 *
 * --------------------------
 *
 * Anno Accademico 2023/2024
 *
 * --------------------------
 *
 * Foschi Gioele
 * Matricola: 0001122551
 * Classe B
 * gioele.foschi@studio.unibo.it
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <limits.h>

/*Priority Queue Structure-Min-Heap*/
typedef struct
{
    int key;
    long int prio;
    /*The coordinates of the cell, only used for a faster access to the matrix*/
    int x_axis;
    int y_axis;
} HeapElem;

typedef struct
{
    HeapElem *heap;
    int *pos;
    int n;    /* quante coppie (chiave, prio) sono effettivamente presenti nello heap */
    int size; /* massimo numero di coppie (chiave, prio) che possono essere contenuti nello heap */
} MinHeap;

/*Matrix Structure*/
typedef struct Cell
{
    int id;
    int height;
    char visited;
    long int shortest_dist_from_origin;
    struct Cell *predecessor;
} Cell;

/*Matrix that contains all the necessary data*/
typedef struct Matrix
{
    int C_Cell;
    int C_height;
    int n; /*Number of Lines*/
    int m; /*Number of Columns*/
    Cell ***Matrix;
} Matrix;

/*Min-Heap Structure Functions*/

/*
 * Clears all the Min-Heap Structure
 *
 * --------------------------
 *
 * h -> The pointer to the Min-Heap Structure
 */
void minheap_clear(MinHeap *h)
{
    int i;
    assert(h != NULL);
    for (i = 0; i < h->size; i++)
    {
        h->pos[i] = -1;
    }
    h->n = 0;
}

/*
 * Initializes a Min-Heap Structure
 * This structure can contain, at maximum capacity, a specified number of elements
 *
 * --------------------------
 *
 * size -> the maximum number of elements in the struct at once
 */
MinHeap *minheap_create(int size)
{
    MinHeap *h = (MinHeap *)malloc(sizeof(*h));
    assert(h != NULL);
    assert(size > 0);

    h->size = size;
    h->heap = (HeapElem *)malloc(size * sizeof(*(h->heap)));
    assert(h->heap != NULL);
    h->pos = (int *)malloc(size * sizeof(*(h->pos)));
    assert(h->pos != NULL);
    minheap_clear(h);
    return h;
}

/*
 * Frees all the memory allocated of a given Min-Heap structure
 *
 * --------------------------
 *
 * h -> Pointer to the matrix
 */
void minheap_destroy(MinHeap *h)
{
    assert(h != NULL);

    h->n = h->size = 0;
    free(h->heap);
    free(h->pos);
    free(h);
}

/*
 * Returns true (not zero) if the heap is empty
 *
 * --------------------------
 *
 * h -> The pointer to the heap structure
 */
int minheap_is_empty(const MinHeap *h)
{
    assert(h != NULL);

    return (h->n == 0);
}

/*
 * Returns true (not zero) if the heap is full
 *
 * --------------------------
 *
 * h -> The pointer to the heap structure
 */
int minheap_is_full(const MinHeap *h)
{
    assert(h != NULL);

    return (h->n == h->size);
}

/*
 * Support Function: Returns 1 if and only if the index 'i' belongs to
 * the range of valid indexes in the array that represents the heap
 *
 * --------------------------
 *
 * h -> The pointer to the heap structure
 * i -> The index that will be checked
 */
static int valid(const MinHeap *h, int i)
{
    assert(h != NULL);

    return ((i >= 0) && (i < h->n));
}

/*
 * Support Function: Returns the index (in the array) of the left child
 *
 * --------------------------
 *
 * h -> The pointer to the heap structure
 * i -> The index of the parent
 */
static int lchild(const MinHeap *h, int i)
{
    assert(valid(h, i));

    return 2 * i + 1;
}

/*
 * Support Function: Returns the index (in the array) of the right child
 *
 * --------------------------
 *
 * h -> The pointer to the heap structure
 * i -> The index of the parent
 */
static int rchild(const MinHeap *h, int i)
{
    assert(valid(h, i));

    return 2 * i + 2;
}

/*
 * Support Function: Returns the index (in the array) of the child with the lowest priority
 *
 * --------------------------
 *
 * h -> The pointer to the heap structure
 * i -> The index of the parent
 */
static int min_child(const MinHeap *h, int i)
{
    int l, r, result = -1;

    assert(valid(h, i));

    l = lchild(h, i);
    r = rchild(h, i);
    if (valid(h, l))
    {
        result = l;
        if (valid(h, r) && (h->heap[r].prio < h->heap[l].prio))
        {
            result = r;
        }
    }
    return result;
}

/*
 * Support Function: Switches heap[i] with heap[j]
 *
 * --------------------------
 *
 * h -> The pointer to the heap structure
 * i -> The index of the first element
 * j -> The index of the second element
 */
static void swap(MinHeap *h, int i, int j)
{
    HeapElem tmp;

    assert(h != NULL);
    assert(valid(h, i));
    assert(valid(h, j));
    assert(h->pos[h->heap[i].key] == i);
    assert(h->pos[h->heap[j].key] == j);

    tmp = h->heap[i];
    h->heap[i] = h->heap[j];
    h->heap[j] = tmp;

    h->pos[h->heap[i].key] = i;
    h->pos[h->heap[j].key] = j;
}

/*
 * Support Function: Returns the index of the parent of the i-th element
 *
 * --------------------------
 *
 * h -> The pointer to the heap structure
 * i -> The index of the element
 */
static int parent(const MinHeap *h, int i)
{
    assert(valid(h, i));

    return (i + 1) / 2 - 1;
}

/*
 * Support Function: Swaps the element in the i-th position with his parent
 * until the element reaches the right position
 *
 * --------------------------
 *
 * h -> The pointer to the heap structure
 * i -> The index of the element
 */
static void move_up(MinHeap *h, int i)
{
    int p;

    assert(valid(h, i));

    p = parent(h, i);
    while (valid(h, p) && (h->heap[i].prio < h->heap[p].prio))
    {
        swap(h, i, p);
        i = p;
        p = parent(h, i);
    }
}

/*
 * Support Function: Swaps the element in the i-th position with his child
 * until the element reaches the right position
 *
 * --------------------------
 *
 * h -> The pointer to the heap structure
 * i -> The index of the element
 */
static void move_down(MinHeap *h, int i)
{
    int done = 0;

    assert(valid(h, i));

    /* L'operazione viene implementata iterativamente, sebbene sia
       possibile una implementazione ricorsiva probabilmente più
       leggibile. */
    do
    {
        const int dst = min_child(h, i);
        if (valid(h, dst) && (h->heap[dst].prio < h->heap[i].prio))
        {
            swap(h, i, dst);
            i = dst;
        }
        else
        {
            done = 1;
        }
    } while (!done);
}

/*
 * Dynamically increases the max size of the heap
 *
 * --------------------------
 *
 * h -> The pointer to the minheap Structure
 */
static void minheap_increase_space(MinHeap *h)
{
    assert(h != NULL);

    h->heap = (HeapElem *)realloc(h->heap, (h->size + 15) * sizeof(*(h->heap)));
    h->size = h->size + 15;

    h->pos = (int *)realloc(h->pos, (h->size + 15) * sizeof(*(h->pos)));

    assert(h != NULL);
}

/*
 * Inserts a new element (key, prio) in the heap structure
 *
 * --------------------------
 *
 * h -> The pointer to the heap structure
 * key -> The key of the element
 * prio -> The priority of the element
 */
void minheap_insert(MinHeap *h, int key, int prio, int x, int y)
{
    int i;

    if (minheap_is_full(h))
        minheap_increase_space(h);

    assert((key >= 0) && (key < h->size));
    assert(h->pos[key] == -1);

    i = h->n++;
    h->pos[key] = i;
    h->heap[i].key = key;
    h->heap[i].prio = prio;
    h->heap[i].x_axis = x;
    h->heap[i].y_axis = y;
    move_up(h, i);
}

/*
 * Returns the key of the element with the lowest priority
 *
 * --------------------------
 *
 * h -> The pointer to the Heap Structure
 */
HeapElem minheap_min(const MinHeap *h)
{
    assert(!minheap_is_empty(h));

    return h->heap[0];
}

/*
 * Deletes the element (key, prio) with the lowest priority
 *
 * Returns the Element removed
 *
 * --------------------------
 *
 * h -> The pointer to the Heap Structure
 */
HeapElem minheap_delete_min(MinHeap *h)
{
    HeapElem result;

    assert(!minheap_is_empty(h));

    result = minheap_min(h);
    swap(h, 0, h->n - 1);
    assert(h->heap[h->n - 1].key == result.key);
    h->pos[result.key] = -1;
    h->n--;
    if (!minheap_is_empty(h))
    {
        move_down(h, 0);
    }
    return result;
}

/*Matrix Structure Functions*/

/*
 * Initializes a Matrix structure
 */
Matrix *matrix_init()
{
    Matrix *M;

    M = (Matrix *)malloc(sizeof(Matrix));

    M->Matrix = NULL;

    return M;
}

/*
 * Frees all the memory allocated of a given matrix structure
 *
 * --------------------------
 *
 * M -> Pointer to the matrix
 */
void matrix_free(Matrix *M)
{
    int i, j;

    for (i = 0; i < M->n; i++)
    {
        for (j = 0; j < M->m; j++)
        {
            free(M->Matrix[i][j]);
        }
        free(M->Matrix[i]);
    }
    free(M->Matrix);
    free(M);
}

/*
 * Reads data from a file stream and populates a Matrix structure.
 * The file in input has to be in the right format
 *
 * --------------------------
 *
 * filein -> Pointer to the input file stream.
 * M      -> Pointer to the Matrix structure to be populated.
 */
void read_input_file(FILE *filein, Matrix *M)
{
    int i, j, _id;

    fscanf(filein, "%d", &M->C_Cell);
    fscanf(filein, "%d", &M->C_height);
    fscanf(filein, "%d", &M->n);
    fscanf(filein, "%d", &M->m);

    _id = 0;
    M->Matrix = (Cell ***)malloc(M->n * (sizeof(Cell **))); /*Allocate Memory for the lines of the Matrix*/
    assert(M->Matrix == NULL);
    for (i = 0; i < M->n; i++)
    {
        M->Matrix[i] = (Cell **)malloc(M->m * (sizeof(Cell *))); /*Allocate Memory for the columns of the Matrix*/
        assert(M->Matrix[i] == NULL);
        for (j = 0; j < M->m; j++)
        {
            M->Matrix[i][j] = (Cell *)malloc(sizeof(Cell)); /*Allocate Memory for each cell of the Matrix*/
            assert(M->Matrix[i][j] == NULL);

            /*Read from the file each value*/
            fscanf(filein, "%d", &(M->Matrix[i][j]->height));

            /*Set an Id to each cell, like it's an array*/
            M->Matrix[i][j]->id = _id;
            _id++;

            /*Set up the values for Dijkstra algorithm*/
            M->Matrix[i][j]->shortest_dist_from_origin = INT_MAX;
            M->Matrix[i][j]->visited = 0;
            M->Matrix[i][j]->predecessor = NULL;
        }
    }
}

/*
 * Prints the contents of the matrix structure
 */
void print_matrix(Matrix *M)
{
    int i, j;

    fprintf(stdout, "C_cell: %d\n", M->C_Cell);
    fprintf(stdout, "C_height: %d\n", M->C_height);
    fprintf(stdout, "n: %d\n", M->n);
    fprintf(stdout, "m: %d\n", M->m);

    for (i = 0; i < M->n; i++)
    {
        for (j = 0; j < M->m; j++)
        {
            fprintf(stdout, "%d|\t", M->Matrix[i][j]->height);
        }
        fprintf(stdout, "\n");
    }
}

/*Problem Functions*/

/*
 * Returns true if the given value is between 0 and a specified value (0 <= i < max)
 *
 * --------------------------
 *
 * i -> The number to check
 * max -> The upper bound of the value
 */
static int is_in_range(int i, int max)
{
    if (i < 0)
        return 0;
    if (i > max)
        return 0;
    return 1;
}

/*
 * Relax Function
 * Verifies if it is possible to improve the minimum path for the node v passing through u
 *
 * --------------------------
 *
 * u -> The pointer to the cell that could improve the path
 * v -> The pointer to the cell of witch you want to "relax" the path
 * M -> The pointer to the Matrix structure containing all the constant costs
 */
static void Relax(Cell *u, Cell *v, Matrix *M)
{
    int relax_value;
    assert(v == NULL);

    /*The cost of going from the cell u to the cell v*/
    relax_value = M->C_Cell + (M->C_height * ((u->height - v->height) * (u->height - v->height)));

    if (v->shortest_dist_from_origin > u->shortest_dist_from_origin + relax_value)
    {
        v->shortest_dist_from_origin = u->shortest_dist_from_origin + relax_value;
        v->predecessor = u;
    }
}

/*
 * Dijkstra Algorithm
 * Used to calculate the lowest cost to create a path in a matrix
 *
 * Uses a Matrix structure that contains all the constants and values needed to process the data
 *
 * --------------------------
 *
 * M -> The pointer to the matrix structure
 */
static void Dijkstra(Matrix *M)
{
    MinHeap *Q = minheap_create(M->m + M->n);
    HeapElem u;

    assert(M == NULL);

    /*Set the distance from the surce = 0*/
    M->Matrix[0][0]->shortest_dist_from_origin = 0;

    minheap_insert(Q, M->Matrix[0][0]->id, 0, 0, 0);

    while (!minheap_is_empty(Q))
    {
        u = minheap_delete_min(Q);
        M->Matrix[u.x_axis][u.y_axis]->visited = 1;

        /*Upper Neighbour Cell*/
        if (is_in_range(u.y_axis - 1, M->n) && !M->Matrix[u.y_axis - 1][u.x_axis]->visited)
        {
            Relax(M->Matrix[u.y_axis][u.x_axis], M->Matrix[u.y_axis - 1][u.x_axis], M);
            minheap_insert(Q,
                           M->Matrix[u.y_axis - 1][u.x_axis]->id,
                           M->Matrix[u.y_axis - 1][u.x_axis]->shortest_dist_from_origin,
                           u.y_axis - 1,
                           u.x_axis);
        }

        /*Right Neighbour Cell*/
        if (is_in_range(u.x_axis + 1, M->n) && !M->Matrix[u.y_axis][u.x_axis + 1]->visited)
        {
            Relax(M->Matrix[u.y_axis][u.x_axis], M->Matrix[u.y_axis][u.x_axis + 1], M);
            minheap_insert(Q,
                           M->Matrix[u.y_axis][u.x_axis + 1]->id,
                           M->Matrix[u.y_axis][u.x_axis + 1]->shortest_dist_from_origin,
                           u.y_axis,
                           u.x_axis + 1);
        }

        /*Lower Neighbour Cell*/
        if (is_in_range(u.y_axis + 1, M->n) && !M->Matrix[u.y_axis + 1][u.x_axis]->visited)
        {
            Relax(M->Matrix[u.y_axis][u.x_axis], M->Matrix[u.y_axis + 1][u.x_axis], M);
            minheap_insert(Q,
                           M->Matrix[u.y_axis + 1][u.x_axis]->id,
                           M->Matrix[u.y_axis + 1][u.x_axis]->shortest_dist_from_origin,
                           u.y_axis + 1,
                           u.x_axis);
        }

        /*Left Neighbour Cell*/
        if (is_in_range(u.x_axis - 1, M->n) && !M->Matrix[u.y_axis][u.x_axis - 1]->visited)
        {
            Relax(M->Matrix[u.y_axis][u.x_axis], M->Matrix[u.y_axis][u.x_axis - 1], M);
            minheap_insert(Q,
                           M->Matrix[u.y_axis][u.x_axis - 1]->id,
                           M->Matrix[u.y_axis][u.x_axis - 1]->shortest_dist_from_origin,
                           u.y_axis,
                           u.x_axis - 1);
        }
    }

    minheap_destroy(Q);
}

static void print_solution_rec(Cell *cell, FILE *output_file, int *n, int *m)
{
    assert(cell == NULL);
    if (cell->id != 0)
        print_solution_rec(cell->predecessor, output_file, n, m);

    fprintf(output_file, "%d %d\n", (int)(cell->id / *(n)), cell->id % *(m));
}
static void print_solution(Matrix *M, FILE *output_file)
{
    assert(M == NULL);

    print_solution_rec(M->Matrix[M->n - 1][M->m - 1], output_file, &(M->n), &(M->m));
    fprintf(output_file, "%d %d\n", -1, -1);
    fprintf(output_file, "%ld", M->Matrix[M->n - 1][M->m - 1]->shortest_dist_from_origin);
}

int main(int argc, char *argv[])
{
    FILE *filein;
    FILE *fileout = stdout;

    /*The Matrix used to calculate the best path*/
    Matrix *M = matrix_init();

    /*Input File Check*/
    if (argc != 2)
    {
        fprintf(stderr, "Input Syntax Error");
        return EXIT_FAILURE;
    }

    /*Open input File*/
    if (strcmp(argv[1], "-") != 0)
    {
        filein = fopen(argv[1], "r");
        if (filein == NULL)
        {
            fprintf(stderr, "Can not open %s\n", argv[1]);
            return EXIT_FAILURE;
        }
    }

    read_input_file(filein, M);

    fclose(filein);

    Dijkstra(M);

    print_solution(M, fileout);

    /*print_matrix(M);*/

    matrix_free(M);

    return EXIT_SUCCESS;
}

/*
 * Function used to test the program in debug mode
 *
 * --------------------------
 *
 * There are no parameters in input, the input file is forced inside the code
 */
/*int main()
{
    char* file_name = "test0.in";
    int *memLeakTest = (int *)malloc(10 * sizeof(int));
    int a;
    FILE *pFile;

    printf("Hello, World!");

    memLeakTest[0] = 5;
    a = memLeakTest[0];
    memLeakTest[1] = a;

}*/