#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"

typedef struct Pair Pair;
typedef struct HashMap HashMap;

struct Pair {
    char *key;
    void *value;
    Pair *next;
};

struct HashMap {
    Pair **buckets;
    long size;     // cantidad de datos/pairs en la tabla
    long capacity; // capacidad de la tabla
    long current;  // indice del ultimo dato accedido
};

Pair *createPair(char *key, void *value) {
    Pair *newPair = (Pair *)malloc(sizeof(Pair));
    newPair->key = strdup(key);
    newPair->value = value;
    newPair->next = NULL;
    return newPair;
}

long hash(char *key, long capacity) {
    unsigned long hash = 0;
    char *ptr;
    for (ptr = key; *ptr != '\0'; ptr++) {
        hash += hash * 32 + tolower(*ptr);
    }
    return hash % capacity;
}

int is_equal(void *key1, void *key2) {
    if (key1 == NULL || key2 == NULL)
        return 0;
    return strcmp((char *)key1, (char *)key2) == 0;
}

void insertMap(HashMap *map, char *key, void *value) {
    // Crear un nuevo par con la clave y el valor dados
    Pair *newPair = createPair(key, value);

    // Calcular el índice del bucket usando la función hash
    long index = hash(key, map->capacity);

    // Si el bucket está vacío, insertar el nuevo par directamente
    if (map->buckets[index] == NULL) {
        map->buckets[index] = newPair;
        map->size++;
    } else {
        // Si el bucket ya está ocupado, agregar el nuevo par al final
        Pair *current = map->buckets[index];
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newPair;
        map->size++;
    }

    // Actualizar el índice current a la posición del nuevo par
    map->current = index;
}

void enlarge(HashMap *map) {
    enlarge_called = 1; // no borrar (testing purposes)
    Pair **old_buckets = map->buckets;
    long old_capacity = map->capacity;

    map->capacity *= 2;
    map->buckets = (Pair **)calloc(map->capacity, sizeof(Pair *));
    map->size = 0;

    for (int i = 0; i < old_capacity; i++) {
        Pair *current = old_buckets[i];
        while (current != NULL) {
            Pair *temp = current;
            current = current->next;
            insertMap(map, temp->key, temp->value);
            free(temp->key);
            free(temp);
        }
    }

    free(old_buckets);
}

HashMap *createMap(long capacity) {
    HashMap *map = (HashMap *)malloc(sizeof(HashMap));
    map->buckets = (Pair **)calloc(capacity, sizeof(Pair *));
    map->size = 0;
    map->capacity = capacity;
    map->current = -1;
    return map;
}

void eraseMap(HashMap *map, char *key) {
    int index = hash(key, map->capacity);
    Pair *current = map->buckets[index];
    Pair *prev = NULL;

    while (current != NULL) {
        if (is_equal(current->key, key)) {
            if (prev == NULL) {
                map->buckets[index] = current->next;
            } else {
                prev->next = current->next;
            }
            free(current->key);
            free(current);
            map->size--;
            return;
        }
        prev = current;
        current = current->next;
    }
}

Pair *searchMap(HashMap *map, char *key) {
    int index = hash(key, map->capacity);
    Pair *current = map->buckets[index];

    while (current != NULL) {
        if (is_equal(current->key, key)) {
            map->current = index;
            return current;
        }
        current = current->next;
    }

    return NULL;
}

Pair *firstMap(HashMap *map) {
    map->current = -1;
    return nextMap(map);
}

Pair *nextMap(HashMap *map) {
    if (map->current >= 0 && map->current < map->capacity) {
        Pair *current = map->buckets[map->current];
        if (current != NULL) {
            map->current = map->current;
            return current;
        }
    }

    for (int i = map->current + 1; i < map->capacity; i++) {
        Pair *current = map->buckets[i];
        if (current != NULL) {
            map->current = i;
            return current;
        }
    }

    return NULL;
}
