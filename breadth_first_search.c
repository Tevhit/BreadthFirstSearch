#include <stdio.h>
#include <stdlib.h>

#define MAX 100

#define VERTICIES 10
#define EDGES_COUNT 14


#define TRUE    1
#define FALSE   0
typedef int BOOL;

int * BFS(int ** graphTable, int verticesCount);

typedef struct tagQueue {
    int * data;
    int capacity;
    int size;
    int front;
    int back;
} Queue;

void * allocateMemory(int size)
{
    void * pvMem;

    pvMem = malloc(size);
    if (pvMem == NULL)
    {
        printf("Cannot allocate memory \n");
        exit(1);
    }
    return pvMem;
}

Queue * createQueue(int capacity)
{
    Queue* queue;

    queue = allocateMemory(sizeof(Queue));

    queue->data = allocateMemory(sizeof(int) * capacity);
    queue->capacity = capacity;
    queue->size = 0;
    queue->back = 0;
    queue->front = 0;

    return queue;
}

void destroyQueue(Queue* queue)
{
    free(queue->data);
    free(queue);
}

BOOL enqueue(Queue * queue, int toAdd)
{
    if(isFull(queue) == FALSE)
    {
        queue->data[queue->back] = toAdd;
        queue->back++;
        queue->back = (queue->back % queue->capacity);
        queue->size++;

        return TRUE;
    }
    return FALSE;
}

BOOL dequeue(Queue* queue, int * pReturn)
{
    if(isEmpty(queue) == FALSE)
    {
        *pReturn = queue->data[queue->front];
        ++queue->front;
        queue->front = (queue->front % queue->capacity);
        queue->size--;

        return TRUE;
    }
    return FALSE;
}

BOOL isEmpty(Queue* queue)
{
    return (queue->size == 0);
}

BOOL isFull(Queue* queue)
{
    return (queue->size == queue->capacity);
}

void printGraphTable(int ** graphTable)
{
    int i, j;
    for(i = 0; i < VERTICIES; i++)
    {
        for(j = 0; j < VERTICIES; j++)
        {
            printf("%d ", graphTable[i][j]);
        }
        printf("\n");
    }
}

void fillZerosToGraphTable(int ** graphTable)
{
    int i, j;
    for(i = 0; i < VERTICIES; i++)
    {
        for(j = 0; j < VERTICIES; j++)
        {
            graphTable[i][j] = 0;
        }
    }
}

int ** createGraphTable(int sizeToVertices)
{
    int i;
    int ** graphTable = (int**) malloc(sizeToVertices * sizeof(int));
    for(i = 0; i < sizeToVertices; i++)
    {
        graphTable[i] = (int *) malloc(sizeToVertices * sizeof(int));
    }
    if(graphTable == NULL)
    {
        printf("Cannot allocate memory \n");
        exit(1);
    }

    return graphTable;
}

void fillEdgesToGraphTable(int ** graphTable, int edgesCount, int edgesArray[edgesCount][2])
{
    int i;
    int row, col;
    for(i = 0; i < edgesCount; i++)
    {
        row = edgesArray[i][0];
        col = edgesArray[i][1];
        graphTable[row][col] = 1;
    }
}

int main()
{
    int ** myGraphTable = createGraphTable(VERTICIES);

    fillZerosToGraphTable(myGraphTable);

    int edges[EDGES_COUNT][2] = {{0,1}, {0,5}, {1,7}, {6,0}, {6,1}, {6,2}, {3,2}, {3,7}, {3,8}, {3,4}, {8,2}, {8,7}, {4,8}, {9,4}};

    fillEdgesToGraphTable(myGraphTable, EDGES_COUNT, edges);

    printf("The graph table is created according to the edges : \n");
    printGraphTable(myGraphTable);
    printf("--------------------\n");


    int * BFS_Array = BFS(myGraphTable, VERTICIES);

    printf("The result of breadth-first sorting : \n");
    int i;
    for(i = 0; i < VERTICIES; i++)
    {
        printf("%d ", BFS_Array[i]);
    }
    printf("\n--------------------\n");

	return 0;
}

int * findFirstIndegree(int ** graphTable, int verticesCount)
{
    int * indegreeArray = (int*) malloc(verticesCount * sizeof(int));
    int i, j;
    int count;
    for(i = 0; i < verticesCount; i++)
    {
        count = 0;
        for(j = 0; j < verticesCount; j++)
        {
            if(graphTable[j][i] == 1)
                count++;
        }
        indegreeArray[i] = count;
    }

    return indegreeArray;
}

int * BFS(int ** graphTable, int verticesCount)
{
    int * bfsArray = (int*) malloc(verticesCount * sizeof(int));

    Queue * queue = createQueue(MAX);

    int * indegrees = findFirstIndegree(graphTable, verticesCount);

    int i, j;

    int state[verticesCount];
    for(i = 0; i < verticesCount; i++) {
        state[i] = 0;
    }

    int vertex;
    for(i = 0; i < verticesCount; i++)
    {
        for(j = 0; j < verticesCount; j++)
        {
            if(indegrees[j] == 0 && state[j] == 0)
            {
                enqueue(queue, j);
                state[j] = 1;
            }
        }
        dequeue(queue, &vertex);
        bfsArray[i] = vertex;
		for(j = 0; j < verticesCount; j++)
        {
            if(graphTable[vertex][j] == 1)
                indegrees[j]--;
        }
    }
    destroyQueue(queue);

    return bfsArray;
}
