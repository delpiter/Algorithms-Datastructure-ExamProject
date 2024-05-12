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
    double prio;
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
    int height;
    char visited;
    int shortest_dist_from_origin;
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

/*Grapgh Structure-Adjacency List*/

/* struttura arco */
typedef struct Edge
{
    int src_id;    /* nodo sorgente        */
    int dst_id;    /* nodo destinazione    */
    double weight; /* peso dell'arco       */
    struct Edge *next;
} Edge;

typedef enum
{
    GRAPH_UNDIRECTED,
    GRAPH_DIRECTED
} Graph_type;

/* struttura grafo */
typedef struct
{
    int n;        /* numero di nodi               */
    int m;        /* numero di archi              */
    Graph_type t; /* tipo di grafo (orientato/non orientato) */
    Edge **edges; /* array di liste di adiacenza  */
    int *in_deg;  /* grado entrante dei nodi      */
    int *out_deg; /* grado uscente dei nodi       */
} Graph;

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
    int i, j;

    fscanf(filein, "%d", &M->C_Cell);
    fscanf(filein, "%d", &M->C_height);
    fscanf(filein, "%d", &M->n);
    fscanf(filein, "%d", &M->m);

    M->Matrix = (Cell ***)malloc(M->n * (sizeof(Cell **))); /*Allocate Memory for the lines of the Matrix*/
    for (i = 0; i < M->n; i++)
    {
        M->Matrix[i] = (Cell **)malloc(M->m * (sizeof(Cell *))); /*Allocate Memory for the columns of the Matrix*/
        for (j = 0; j < M->m; j++)
        {
            M->Matrix[i][j] = (Cell *)malloc(sizeof(Cell)); /*Allocate Memory for each cell of the Matrix*/

            fscanf(filein, "%d", &(M->Matrix[i][j]->height));
            M->Matrix[i][j]->shortest_dist_from_origin = INT_MAX;
            M->Matrix[i][j]->visited = 0;
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