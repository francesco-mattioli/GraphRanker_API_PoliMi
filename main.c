#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct Nodo
{
    int dest;
    unsigned weight;
    struct Nodo* next;
};

struct Listadiacenza
{
    struct Nodo *head;
};
//array di head, perchè ogni nodo ha una lista che contiene i nodi
struct Grafo
{
    int n;
    struct Listadiacenza* array;
};

struct Nodo* Nuovonodo(int dest, unsigned int weight)
{
    struct Nodo* nuovo =(struct Nodo*)malloc(sizeof(struct Nodo));
    nuovo->dest = dest;
    printf("dest: %d\n",nuovo->dest);
    nuovo->weight = weight;
    printf("wigth: %d\n",nuovo->weight);
    nuovo->next = NULL;
    return nuovo;
}

struct Grafo* CreaGrafo(int n)
{
    struct Grafo* grafo = (struct Grafo*)malloc(sizeof(struct Grafo));
    grafo->n = n;
    grafo->array = (struct Listadiacenza*)malloc(n * sizeof(struct Listadiacenza));
    for (int i = 0; i < n; ++i)
        grafo->array[i].head = NULL;

    return grafo;
}

void Aggiugni(struct Grafo* grafo, int src,int dest, unsigned int weight)
{
    struct Nodo* nuovo = Nuovonodo(dest, weight);
    nuovo->next = grafo->array[src].head;
    grafo->array[src].head = nuovo;
}


int main()
{
    int n,k;
    scanf("%d %d",&n,&k);
    struct Grafo* grafo = CreaGrafo(n);
    int l = 0;
    char buffer[n*n*sizeof(int)];
    char *comma;
    unsigned weigth;
    char buf[14];
    getchar();
    while(fgets(buf,14,stdin)!=NULL) {
        getchar();
        if (buf[0]=='A'){
            for (int i = 0; i < n; i++) {
                fgets(buffer, n * n * sizeof(unsigned int), stdin);
                comma = strtok(buffer, ",");
                while (comma) {
                    weigth = strtoul(comma,&comma,10);
                    if (weigth!=0){
                        Aggiugni(grafo, i, l, weigth);
                    }
                    l++;
                    comma = strtok(NULL, ",");
                }
                l = 0;
            }
        }
    }
    return 0;
}
