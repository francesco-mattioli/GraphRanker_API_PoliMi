#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <stdint.h>

struct Nodo{
    int dest;
    int peso;
    struct Nodo* next;
};
struct ListaAdiacenza{
    struct Nodo *head;
};
struct Grafo{
    int n;
    struct ListaAdiacenza* array;
};


struct Grafo* CreaGrafo(int n){
    struct Grafo* grafo = (struct Grafo*)malloc(sizeof(struct Grafo));
    grafo->n = n;
    grafo->array = (struct ListaAdiacenza*)malloc(n * sizeof(struct ListaAdiacenza));
    for (int i = 0; i < n; i++)
        grafo->array[i].head = NULL;
    return grafo;
}


struct MinHeapNodo{
    int vert;
    int dist;
};

struct MaxHeapNodo{ //nodo max heap
    int i;
    int somma; //la somma dei percorsi più brevi del grafo
    int numgrafo;
};

struct MinHeap{
    int HeapSize;
    int HeapLength;
    int *pos;
    struct MinHeapNodo **array;
    struct MinHeapNodo **nodes;
};

struct MaxHeap{
    int MaxHeapSize; //dimensione corrente del min heap
    int MaxHeapLength; // dimensione massima del min heap
    int *maxpos;
    struct MaxHeapNodo **maxarray; // array di nodi min heap
};

struct MinHeapNodo* NuovoMinHeapNodo(int vert,int dist){
    struct MinHeapNodo* minHeapNodo =(struct MinHeapNodo*)malloc(sizeof(struct MinHeapNodo));
    minHeapNodo->vert = vert;
    minHeapNodo->dist = dist;
    return minHeapNodo;
}

struct MaxHeapNodo* NuovoMaxHeapNodo(int i,int sum, int numerografo){ //creazione di un nuovo nodo con i e peso nel max heap
    struct MaxHeapNodo* maxHeapNodo = (struct MaxHeapNodo*)malloc(sizeof(struct MaxHeapNodo));
    maxHeapNodo->i=i;
    maxHeapNodo->somma=sum;
    maxHeapNodo->numgrafo=numerografo;
    return maxHeapNodo;
}

struct MinHeap* CreaMinHeap(int HeapLength){
    struct MinHeap* minHeap =(struct MinHeap*)malloc(sizeof(struct MinHeap));
    minHeap->pos = (int *)malloc(HeapLength * sizeof(int));
    minHeap->HeapSize = 0;
    minHeap->HeapLength = HeapLength;
    minHeap->array = (struct MinHeapNodo**)malloc(HeapLength * sizeof(struct MinHeapNodo*));
    minHeap->nodes = (struct MinHeapNodo**)malloc(HeapLength * sizeof(struct MinHeapNodo*));
    return minHeap;
}

struct MaxHeap* CreaMaxHeap(int MaxHeapLength){ //creo il max heap che sarà la classfica
    struct MaxHeap* maxHeap = (struct MaxHeap*)malloc(sizeof (struct MaxHeap));
    maxHeap->maxpos=(int*) malloc(MaxHeapLength* sizeof(int));
    maxHeap->MaxHeapSize = 0; //la dimensione corrente è 0
    maxHeap->MaxHeapLength = MaxHeapLength; //la dimensione massima è k che è passata come parametro
    maxHeap->maxarray = (struct MaxHeapNodo**)malloc(MaxHeapLength*sizeof (struct MaxHeapNodo*));
    return maxHeap;
}

void swapMinHeapNodo(struct MinHeapNodo** nodo1,struct MinHeapNodo** nodo2){
    struct MinHeapNodo* temp = *nodo1;
    *nodo1 = *nodo2;
    *nodo2 = temp;
}

void swapMaxHeapNodo(struct MaxHeapNodo** nodo3, struct MaxHeapNodo** nodo4){ //mi serve per eseguire lo swap di due nodi
    struct MaxHeapNodo* maxtemp = *nodo3;
    *nodo3 = *nodo4;
    *nodo4 = maxtemp;
}

void MinHeapify(struct MinHeap* minHeap,int index){
    int c1=1;
    while (c1!=0){
        int min, left, right;
        min = index;
        left = 2 * index + 1;
        right = 2 * index + 2;

        if (left < minHeap->HeapSize && minHeap->array[left]->dist < minHeap->array[min]->dist )
            min = left;

        if (right < minHeap->HeapSize && minHeap->array[right]->dist < minHeap->array[min]->dist )
            min = right;

        if (min != index){
            struct MinHeapNodo *minNodo = minHeap->array[min];
            struct MinHeapNodo *indexNodo = minHeap->array[index];
            minHeap->pos[minNodo->vert] = index;
            minHeap->pos[indexNodo->vert] = min;
            swapMinHeapNodo(&minHeap->array[min],&minHeap->array[index]);
            index = min;
        }
        else c1 = 0;
    }

}

void MaxHeapify(struct MaxHeap* maxHeap, int maxidx){ //come sopra, ma per il max heap

    if (maxHeap->MaxHeapSize<=maxHeap->MaxHeapLength){
        int c1 =1;
        while (c1!=0){
            int parent = (maxidx - 1)/2;
            if (maxHeap->maxarray[parent]->somma < maxHeap->maxarray[maxidx]->somma){
                struct MaxHeapNodo *parentNodo = maxHeap->maxarray[parent];
                struct MaxHeapNodo *maxidxNodo = maxHeap->maxarray[maxidx];
                maxHeap->maxpos[parentNodo->i]=maxidx;
                maxHeap->maxpos[maxidxNodo->i]=parent;
                swapMaxHeapNodo(&maxHeap->maxarray[parent],&maxHeap->maxarray[maxidx]);
                maxidx = parent;
            }
            else c1 = 0;
        }
    }
    int c2 = 1;
    while (c2!=0){
        int max,left,right;
        left = 2 * maxidx + 1;
        right = 2 * maxidx + 2;
        max = maxidx;
        if (left < maxHeap->MaxHeapSize && maxHeap->maxarray[left]->somma > maxHeap->maxarray[max]->somma)
            max = left;
        if (right < maxHeap->MaxHeapSize && maxHeap->maxarray[right]->somma > maxHeap->maxarray[max]->somma)
            max = right;
        if (max!=maxidx){
            struct MaxHeapNodo *maxNodo = maxHeap->maxarray[max];
            struct MaxHeapNodo *maxidxNodo = maxHeap->maxarray[maxidx];
            maxHeap->maxpos[maxNodo->i]=maxidx;
            maxHeap->maxpos[maxidxNodo->i]=max;
            swapMaxHeapNodo(&maxHeap->maxarray[max],&maxHeap->maxarray[maxidx]);
            maxidx = max;
        }
        else c2 = 0;
    }
}

int Vuoto(struct MinHeap* minHeap){
    return minHeap->HeapSize == 0;
}

struct MinHeapNodo* EstraiMin(struct MinHeap* minHeap){
    if (Vuoto(minHeap))
        return NULL;
    struct MinHeapNodo* root = minHeap->array[0];
    struct MinHeapNodo* UltimoNodo = minHeap->array[minHeap->HeapSize - 1];
    minHeap->array[0] = UltimoNodo;
    minHeap->pos[root->vert] = minHeap->HeapSize-1;
    minHeap->pos[UltimoNodo->vert] = 0;
    --minHeap->HeapSize;
    MinHeapify(minHeap, 0);
    return root;
}


void DecreaseKey(struct MinHeap* minHeap,int vert,int dist){
    int i = minHeap->pos[vert];
    minHeap->array[i]->dist = dist;
    while (i && minHeap->array[i]->dist < minHeap->array[(i - 1) / 2]->dist){
        minHeap->pos[minHeap->array[i]->vert] = (i - 1) / 2;
        minHeap->pos[minHeap->array[(i - 1) / 2]->vert] = i;
        swapMinHeapNodo(&minHeap->array[i],&minHeap->array[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}


bool IsInMinHeap(struct MinHeap *minHeap, int vert){
    if (minHeap->pos[vert] < minHeap->HeapSize)
        return true;
    return false;
}


int GetMax(struct MaxHeap* maxHeap){
    int massimo = maxHeap->maxarray[0]->somma;
    return massimo;
}

int Somma(int dist[],int n){
    int somm=0;
    for (int i = 0; i < n; i++) {
        if (dist[i]!=INT_MAX)
            somm= somm+dist[i];
    }
    return somm;
}


int dijkstra(struct Grafo* grafo, int src,struct MaxHeap* maxHeap, int numerografo,struct MinHeap *minHeap,int *dist){
    int n = grafo->n;
    int somm=0;
    while (!Vuoto(minHeap)){
        struct MinHeapNodo* minHeapNodo = EstraiMin(minHeap);
        int u = minHeapNodo->vert;
        struct Nodo* pCrawl = grafo->array[u].head;
        while (pCrawl != NULL){
            int vert = pCrawl->dest;
            if ((dist[u] != INT_MAX) && ((pCrawl->peso + dist[u]) < dist[vert]) && IsInMinHeap(minHeap, vert)){
                dist[vert] = dist[u] + pCrawl->peso;
                DecreaseKey(minHeap, vert, dist[vert]);
            }
            pCrawl = pCrawl->next;
        }
        free(pCrawl);
    }
    somm = Somma(dist,n);
    return somm;
}

void StampaHeap(struct MaxHeap* maxHeap){
    int n = maxHeap->MaxHeapSize-1;
    for (int i=0;i<n;i++){
        printf("%d ",maxHeap->maxarray[i]->numgrafo);
    }

    if (maxHeap->MaxHeapSize!=0){
        printf("%d",maxHeap->maxarray[maxHeap->MaxHeapSize-1]->numgrafo);
    }
}

void AllocMinHeap(struct MinHeap* heap, int n) {
    for(int i = 0; i < n; ++i) {
        heap->nodes[i]=NuovoMinHeapNodo(i, INT_MAX);
    }
}

void InitMinHeap(struct MinHeap* heap, int n) {
    for(int i = 0; i < n; ++i) {
        heap->array[i] = heap->nodes[i];
    }
}

typedef struct {
    struct Nodo* nodes;
    int size;
    int current;
} NodePool;

void InitPool(NodePool* pool) {
    pool->current = 0;
}

void AllocNodePool(NodePool* pool, int n) {
    pool->nodes = (struct Nodo*)malloc(sizeof(struct Nodo) * n * n);
    pool->size = n * n;
    InitPool(pool);
}

struct Nodo* GetNodeFromPool(NodePool* pool) {
    return &pool->nodes[pool->current++];
}

void AggiungiGrafo(struct Grafo* grafo, int src, int dest, int peso, struct Nodo* nuovo) {
    nuovo->dest = dest;
    nuovo->peso = peso;
    nuovo->next = grafo->array[src].head;
    grafo->array[src].head = nuovo;
}


int fastatoi(char *str){
    int valore=0;
    while (*str && (*str != '\n')){
        valore=valore*10+(*str++-'0');
    }
    return valore;
}

char* fastsep(char** str) {
    char* ret=*str;
    char* comma=strchr(*str,',');
    if (comma!=NULL) {*comma++=0; *str=comma;}
    return ret;
}

int main(){
    int n=0,k=0;
    if(scanf("%d %d",&n,&k)==0)
        return -1;
    getchar();
    int l = 0;
    int *sum = malloc(k* sizeof(int));
    char buffer[n*8];
    char *comma;
    int peso;
    int somma=0;
    int contatore=0;
    int numerografo=0;
    char buf[14];
    struct Grafo* grafo = CreaGrafo(n);
    int *dist = malloc(n*sizeof(int));
    struct MinHeap* minHeap = CreaMinHeap(n);
    struct MaxHeap* maxHeap = CreaMaxHeap(k);
    for (int i = 0; i < k; i++) {
        sum[i] = 0;
        maxHeap->maxarray[i]= NuovoMaxHeapNodo(i,sum[i],i);
        maxHeap->maxpos[i]= i;
    }
    AllocMinHeap(minHeap, n);

    NodePool pool;
    AllocNodePool(&pool, n);

    while (fgets(buf,14,stdin)!=NULL){
        if (buf[0]=='A'){
            getchar();
            InitMinHeap(minHeap, n);
            for (int i = 0; i < n; i++) {
                dist[i] = INT_MAX;
                minHeap->array[i]->dist = INT_MAX;
                minHeap->array[i]->vert = i;
                minHeap->pos[i] = i;
            }
            dist[0] = 0;
            minHeap->array[0]->vert=0;
            minHeap->array[0]->dist=0;
            minHeap->pos[0] = 0;
            DecreaseKey(minHeap, 0, dist[0]);
            minHeap->HeapSize = n;

            InitPool(&pool);
            for (int i = 0; i < n; i++) {
                grafo->array[i].head = NULL;
                if(fgets(buffer, n*8, stdin)!=NULL){
                    char* tmpbuf = buffer;
                    comma = fastsep(&tmpbuf);
                    // scarta sempre il primo nodo
                    comma = fastsep(&tmpbuf);
                    l=1;
                    while (l<n) {
                        peso = fastatoi(comma);
                        contatore |= (peso != 0 && i==0);
                        if ((peso != 0) && (i!=l)) {
                            AggiungiGrafo(grafo, i, l, peso, GetNodeFromPool(&pool));
                        }
                        ++l;
                        comma=fastsep(&tmpbuf);
                    }
                    if (contatore==0){
                        somma=0;
                        i=n;
                    }
                }
            }
            if (contatore!=0){
                somma=dijkstra(grafo, 0,maxHeap,numerografo,minHeap,dist);
            }
            contatore=0;

            if (numerografo < k){
                maxHeap->maxarray[maxHeap->MaxHeapSize]= NuovoMaxHeapNodo(numerografo,somma,numerografo);
                maxHeap->MaxHeapSize=maxHeap->MaxHeapSize+1;
                MaxHeapify(maxHeap,numerografo);
            }
            else if (somma < GetMax(maxHeap)){
                maxHeap->maxarray[0]->somma=somma;
                maxHeap->maxarray[0]->numgrafo=numerografo;
                MaxHeapify(maxHeap,0);
            }
            minHeap->HeapSize=0;
            numerografo++;

        }
        if (buf[0]=='T'){
            StampaHeap(maxHeap);
            printf("\n");
        }
    }

    return 0;
}


