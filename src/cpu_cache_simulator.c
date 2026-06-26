// Online C compiler to run C program online
// Online C compiler to run C program online
#include <stdio.h>
#include <stdlib.h>

#define RAM_SIZE 1000

int RAM[RAM_SIZE];

typedef struct Node
{
    int address;
    int value;

    int dirty;

    struct Node *prev;
    struct Node *next;
} Node;


Node* createNode(int address, int value)
{
    Node *newNode =
    malloc(sizeof(Node));

    newNode->address = address;
    newNode->value = value;

    newNode->dirty = 0;

    newNode->prev = NULL;
    newNode->next = NULL;

    return newNode;
}

typedef struct HashEntry
{
    int key;

    Node *node;

    struct HashEntry *next;

} HashEntry;

typedef struct
{
    int capacity;
    int size;

    int hits;
    int misses;
    int evictions;

    Node *head;
    Node *tail;

    HashEntry **map;
    int hashSize;

} Cache;

int hashFunction(int key, int hashSize)
{
    return key % hashSize;
}


void addToFront(Cache *cache, Node *node)
{
    if(cache->head == NULL)
    {
        cache->head = node;
        cache->tail = node;
        return;
    }

    node->next = cache->head;
    node->prev = NULL;

    cache->head->prev = node;
    cache->head = node;
}

void displayCache(Cache *cache)
{
    Node *temp = cache->head;

    printf("Cache: ");

    while(temp != NULL)
    {
        printf("[%d V=%d D=%d] ",temp->address,temp->value,temp->dirty);
        temp = temp->next;
    }

    printf("\n");
}

/* Remove any node */
void removeNode(Cache *cache, Node *node)
{
    /* only node */
    if(cache->head == node && cache->tail == node)
    {
        cache->head = NULL;
        cache->tail = NULL;
    }

    /* head node */
    else if(node == cache->head)
    {
        cache->head = node->next;
        cache->head->prev = NULL;
    }

    /* tail node */
    else if(node == cache->tail)
    {
        cache->tail = node->prev;
        cache->tail->next = NULL;
    }

    /* middle node */
    else
    {
        node->prev->next = node->next;
        node->next->prev = node->prev;
    }

    node->prev = NULL;
    node->next = NULL;
}

/* Move node to front */
void moveToFront(Cache *cache, Node *node)
{
    if(node == cache->head)
        return;

    removeNode(cache, node);
    addToFront(cache, node);
}


void hashInsert(Cache *cache,
                int key,
                Node *node)
{
    int index =
    hashFunction(key, cache->hashSize);

    HashEntry *entry =malloc(sizeof(HashEntry));

    entry->key = key;
    entry->node = node;

    entry->next =
    cache->map[index];

    cache->map[index] =
    entry;
}

Node* hashLookup(Cache *cache,
                 int key)
{
    int index =
    hashFunction(key,
                 cache->hashSize);

    HashEntry *temp =
    cache->map[index];

    while(temp)
    {
        if(temp->key == key)
            return temp->node;

        temp = temp->next;
    }

    return NULL;
}


void hashRemove(Cache *cache,
                int key)
{
    int index =
    hashFunction(key,
                 cache->hashSize);

    HashEntry *curr =
    cache->map[index];

    HashEntry *prev = NULL;

    while(curr)
    {
        if(curr->key == key)
        {
            if(prev == NULL)
            {
                cache->map[index] =
                curr->next;
            }
            else
            {
                prev->next =
                curr->next;
            }

            free(curr);
            return;
        }

        prev = curr;
        curr = curr->next;
    }
}


void removeTail(Cache *cache)
{
    if(cache->tail == NULL)
        return;

    Node *lru = cache->tail;

    printf("Evicted: %d\n", lru->address);

    if(lru->dirty == 1)
    {
      RAM[lru->address]=lru->value;
      printf("WRITE BACK: RAM[%d] = %d\n",lru->address,lru->value);
    }

    hashRemove(cache,lru->address);

    removeNode(cache,lru);

    free(lru);

    cache->size--;
    cache->evictions++;
}

void writeMemory(
Cache *cache,
int address,
int newValue
)
{
    Node *node =
    hashLookup(
    cache,
    address
    );

    if(node != NULL)
    {
        printf(
        "\nWRITE HIT %d = %d\n",
        address,
        newValue
        );

        node->value = newValue;

        node->dirty = 1;

        moveToFront(
        cache,
        node
        );
        cache->hits++;
    }
    else
    {
        printf(
        "\nWRITE MISS %d = %d\n",
        address,
        newValue
        );

        cache->misses++;  

        if(cache->size ==
           cache->capacity)
        {
            removeTail(cache);
        }

        Node *newNode =
        createNode(
        address,
        newValue
        );

        newNode->dirty = 1;

        addToFront(
        cache,
        newNode
        );

        hashInsert(
        cache,
        address,
        newNode
        );

        cache->size++;
    }

    displayCache(cache);
}


void accessMemory(Cache *cache, int address)
{
    printf("\nAccessing %d\n", address);

    Node *node = hashLookup(cache, address);

    /* HIT */
    if(node != NULL)
    {
        printf("HIT\n");

        cache->hits++;

        moveToFront(cache, node);
    }

    /* MISS */
    else
    {
        printf("MISS\n");

        cache->misses++;

        if(cache->size == cache->capacity)
        {
            removeTail(cache);
        }

       Node *newNode =createNode(address,RAM[address]);

        addToFront(cache, newNode);

        hashInsert(cache,address,newNode);

        cache->size++;
    }

    displayCache(cache);
}



void printStatistics(Cache *cache)
{
    int total = cache->hits + cache->misses;

    printf("\n========== REPORT ==========\n");

    printf("Hits      : %d\n", cache->hits);
    printf("Misses    : %d\n", cache->misses);
    printf("Evictions : %d\n", cache->evictions);

    printf("Hit Ratio : %.2f%%\n",
           (100.0 * cache->hits) / total);

    printf("Miss Ratio : %.2f%%\n",
           (100.0 * cache->misses) / total);
}


int main()
{
    for(int i = 0; i < RAM_SIZE; i++)
    {
        RAM[i] = i * 10;
    }

    Cache cache;

    printf("Enter Cache Capacity: ");
    scanf("%d", &cache.capacity);

    cache.hashSize = 2 * cache.capacity;

    cache.map =
    (HashEntry **)calloc(
        cache.hashSize,
        sizeof(HashEntry *)
    );

    cache.size = 0;

    cache.hits = 0;
    cache.misses = 0;
    cache.evictions = 0;

    cache.head = NULL;
    cache.tail = NULL;

    int n;

    printf("Enter Number of Operations: ");
    scanf("%d", &n);

    printf("\nEnter Operations:\n");
    printf("R <address>\n");
    printf("W <address> <value>\n\n");

    for(int i = 0; i < n; i++)
    {
        char op;

        scanf(" %c", &op);

        if(op == 'R')
        {
            int address;

            scanf("%d", &address);

            accessMemory(
                &cache,
                address
            );
        }
        else if(op == 'W')
        {
            int address;
            int value;

            scanf(
                "%d %d",
                &address,
                &value
            );

            writeMemory(
                &cache,
                address,
                value
            );
        }
        else
        {
            printf(
            "Invalid Operation!\n"
            );
        }
    }

    printf("\n===== FINAL RAM CHECK =====\n");

    for(int i = 0; i < 20; i++)
    {
        printf(
        "RAM[%d] = %d\n",
        i,
        RAM[i]
        );
    }

    printStatistics(&cache);

    return 0;
}

 