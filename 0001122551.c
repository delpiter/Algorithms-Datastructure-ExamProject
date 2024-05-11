#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>

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

typedef struct Cell
{
    int id;
    int x;
    int y;
    int height;
} Cell;

/*Test Main*/
/*int main()
{
    char* file_name = "test0.in";
    int *memLeakTest = (int *)malloc(10 * sizeof(int));
    int a;
    FILE *pFile;

    printf("ciao");

    memLeakTest[0] = 5;
    a = memLeakTest[0];
    memLeakTest[1] = a;

}*/

int main(int argc, char *argv[])
{
    int src = 0;
    int *memLeakTest = (int *)malloc(10 * sizeof(int));
    int a;
    char *filename;

    filename = argv[1];
    printf("%s\n", filename);

    printf("ciao\n");

    memLeakTest[0] = 5;
    a = memLeakTest[0];
    memLeakTest[1] = a;
    memLeakTest[2] = src;

    return EXIT_SUCCESS;
}