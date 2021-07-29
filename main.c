#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include <string.h>

struct Nodo{
    int dest;
    unsigned int peso;
    struct Nodo* next;
};

struct ListaAdiacenza{
    struct Nodo *head;
};

struct Grafo{
    int n;
    struct ListaAdiacenza* array;
};

struct Nodo* NuovoNodo(int dest,unsigned int peso){
    struct Nodo* nuovo =(struct Nodo*)malloc(sizeof(struct Nodo));
    nuovo->dest = dest;
    nuovo->peso = peso;
    nuovo->next = NULL;
    return nuovo;
}

struct Grafo* CreaGrafo(int n){
    struct Grafo* grafo = (struct Grafo*)malloc(sizeof(struct Grafo));
    grafo->n = n;
    grafo->array = (struct ListaAdiacenza*)malloc(n * sizeof(struct ListaAdiacenza));
    for (int i = 0; i < n; i++)
        grafo->array[i].head = NULL;
    return grafo;
}

void AggiungiGrafo(struct Grafo* grafo, int src,int dest,unsigned int peso){
    if (dest!=0){
        struct Nodo* nuovo = NuovoNodo(dest, peso);
        nuovo->next = grafo->array[src].head;
        grafo->array[src].head = nuovo;
    }
}

struct MinHeapNodo{
    int vert;
    unsigned int dist;
};

struct MaxHeapNodo{ //nodo max heap
    int indice;
    unsigned int somma; //la somma dei percorsi più brevi del grafo
    int numgrafo;
};

struct MinHeap{
    int HeapSize;
    int HeapLength;
    int *pos;
    struct MinHeapNodo **array;
};

struct MaxHeap{
    int MaxHeapSize; //dimensione corrente del min heap
    int MaxHeapLength; // dimensione massima del min heap
    int *maxpos;
    struct MaxHeapNodo **maxarray; // array di nodi min heap
};

struct MinHeapNodo* NuovoMinHeapNodo(int vert,unsigned int dist){
    struct MinHeapNodo* minHeapNodo =(struct MinHeapNodo*)malloc(sizeof(struct MinHeapNodo));
    minHeapNodo->vert = vert;
    minHeapNodo->dist = dist;
    return minHeapNodo;
}

struct MaxHeapNodo* NuovoMaxHeapNodo(int indice,unsigned int sum, int numerografo){ //creazione di un nuovo nodo con indice e peso nel max heap
    struct MaxHeapNodo* maxHeapNodo = (struct MaxHeapNodo*)malloc(sizeof(struct MaxHeapNodo));
    maxHeapNodo->indice=indice;
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
        MinHeapify(minHeap, min);
    }
}

void MaxHeapify(struct MaxHeap* maxHeap, int maxidx){ //come sopra, ma per il max heap
    int max,left,right;
    left = 2 * maxidx + 1;
    right = 2 * maxidx + 2;
    max = maxidx;
    if (maxHeap->MaxHeapSize<=maxHeap->MaxHeapLength){
        int parent = (maxidx - 1)/2;
        if (maxHeap->maxarray[parent]->somma < maxHeap->maxarray[maxidx]->somma){
            struct MaxHeapNodo *parentNodo = maxHeap->maxarray[parent];
            struct MaxHeapNodo *maxidxNodo = maxHeap->maxarray[maxidx];
            maxHeap->maxpos[parentNodo->indice]=maxidx;
            maxHeap->maxpos[maxidxNodo->indice]=parent;
            swapMaxHeapNodo(&maxHeap->maxarray[parent],&maxHeap->maxarray[maxidx]);
            MaxHeapify(maxHeap,parent);
        }
    }

    if (left < maxHeap->MaxHeapSize && maxHeap->maxarray[left]->somma > maxHeap->maxarray[max]->somma)
        max = left;
    if (right < maxHeap->MaxHeapSize && maxHeap->maxarray[right]->somma > maxHeap->maxarray[max]->somma)
        max = right;
    if (max!=maxidx){
        struct MaxHeapNodo *maxNodo = maxHeap->maxarray[max];
        struct MaxHeapNodo *maxidxNodo = maxHeap->maxarray[maxidx];
        maxHeap->maxpos[maxNodo->indice]=maxidx;
        maxHeap->maxpos[maxidxNodo->indice]=max;
        swapMaxHeapNodo(&maxHeap->maxarray[max],&maxHeap->maxarray[maxidx]);
        MaxHeapify(maxHeap,max);
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


void DecreaseKey(struct MinHeap* minHeap,int vert,unsigned int dist){
    int i = minHeap->pos[vert];
    minHeap->array[i]->dist = dist;
    while (i && minHeap->array[i]->dist < minHeap->array[(i - 1) / 2]->dist){
        minHeap->pos[minHeap->array[i]->vert] =(i-1)/2;
        minHeap->pos[minHeap->array[(i-1)/2]->vert] = i;
        swapMinHeapNodo(&minHeap->array[i],&minHeap->array[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}


bool IsInMinHeap(struct MinHeap *minHeap, int vert){
    if (minHeap->pos[vert] < minHeap->HeapSize)
        return true;
    return false;
}


unsigned int GetMax(struct MaxHeap* maxHeap){
    unsigned int massimo = maxHeap->maxarray[0]->somma;
    return massimo;
}

unsigned int Somma(unsigned int dist[],int n){
    unsigned int somm=0;
    for (int i = 0; i < n; i++) {
        if (dist[i]!=UINT_MAX)
            somm= somm+dist[i];
    }
    return somm;
}

void StampaHeap(struct MaxHeap* maxHeap){
    for (int i=0;i<maxHeap->MaxHeapSize-1;i++){
        printf("%d ",maxHeap->maxarray[i]->numgrafo);
    }
    if (maxHeap->MaxHeapSize!=0)
        printf("%d",maxHeap->maxarray[maxHeap->MaxHeapSize-1]->numgrafo);
}

void dijkstra(struct Grafo* grafo, int src,struct MaxHeap* maxHeap, int numerografo){
    int n = grafo->n;
    int k = maxHeap->MaxHeapLength;
    unsigned int dist[n];
    unsigned int somm=0;
    struct MinHeap* minHeap = CreaMinHeap(n);
    for (int vert = 0; vert < n; vert++) {
        dist[vert] = UINT_MAX;
        minHeap->array[vert] = NuovoMinHeapNodo(vert, dist[vert]);
        minHeap->pos[vert] = vert;
    }
    minHeap->array[src] = NuovoMinHeapNodo(src, dist[src]);
    minHeap->pos[src] = src;
    dist[src] = 0;
    DecreaseKey(minHeap, src, dist[src]);
    minHeap->HeapSize = n;
    while (!Vuoto(minHeap)){
        struct MinHeapNodo* minHeapNodo = EstraiMin(minHeap);
        int u = minHeapNodo->vert;
        struct Nodo* pCrawl = grafo->array[u].head;
        while (pCrawl != NULL){
            int vert = pCrawl->dest;
            if (IsInMinHeap(minHeap, vert) && dist[u] != UINT_MAX && pCrawl->peso + dist[u] < dist[vert]){
                dist[vert] = dist[u] + pCrawl->peso;
                DecreaseKey(minHeap, vert, dist[vert]);
            }
            pCrawl = pCrawl->next;
        }
    }
    somm = Somma(dist,n);
    if (numerografo < k){
        maxHeap->maxarray[maxHeap->MaxHeapSize]= NuovoMaxHeapNodo(numerografo,somm,numerografo);
        maxHeap->MaxHeapSize=maxHeap->MaxHeapSize+1;
        MaxHeapify(maxHeap,numerografo);
    }
    else if (somm < GetMax(maxHeap)){
        maxHeap->maxarray[0]->somma=somm;
        maxHeap->maxarray[0]->numgrafo=numerografo;
        MaxHeapify(maxHeap,0);
    }
    free(minHeap);
}


int main(){
    int n=0,k=0;
    if(scanf("%d %d",&n,&k)==0)
        return -1;
    getchar();
    int l = 0;
    unsigned int sum[k];
    char buffer[n*n*sizeof(unsigned int)];
    char *comma;
    unsigned int peso;
    int numerografo=0;
    char buf[14];
    struct MaxHeap* maxHeap = CreaMaxHeap(k);
    for (int indice = 0; indice < k; indice++) {
        sum[k] = 0;
        maxHeap->maxarray[indice]= NuovoMaxHeapNodo(indice,sum[indice],indice);
        maxHeap->maxpos[indice]= indice;
    }
    while (fgets(buf,14,stdin)!=NULL){

        if (buf[0]=='A'){
            getchar();
            struct Grafo* grafo = CreaGrafo(n);
            for (int i = 0; i < n; i++) {
                if(fgets(buffer, n * n * sizeof(unsigned int), stdin)!=NULL){
                    comma = strtok(buffer, ",");
                    while (comma) {
                        peso = strtoul(comma,&comma,10);
                        if (peso!=0){
                            AggiungiGrafo(grafo, i, l, peso);
                        }
                        l++;
                        comma = strtok(NULL, ",");
                    }
                    l = 0;
                }
            }
            dijkstra(grafo, 0,maxHeap,numerografo);
            numerografo++;
            free(grafo);
        }
        if (buf[0]=='T'){
            StampaHeap(maxHeap);
            printf("\n");
        }
    }
    return 0;
}


