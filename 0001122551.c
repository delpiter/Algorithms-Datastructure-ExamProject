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
    int prio;
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
    int shortest_dist_from_origin;
    Cell *predecessor;
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

/*Data structure for the solution*/
typedef struct Solution
{
    int *best_path[2];
    long int path_cost;
} Solution;

/*Min-Heap Structure Functions*/

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
void minheap_insert(MinHeap *h, int key, int prio)
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

Solution *Dijkstra(Matrix *M)
{
    Solution *sol;

    return sol;
}

int main(int argc, char *argv[])
{
    FILE *filein;

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

    print_matrix(M);

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