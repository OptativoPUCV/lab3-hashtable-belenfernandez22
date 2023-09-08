#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"
typedef struct HashMap HashMap;
int enlarge_called=0;

struct HashMap {
    Pair ** buckets;
    long size; //cantidad de datos/pairs en la tabla
    long capacity; //capacidad de la tabla
    long current; //indice del ultimo dato accedido
};

Pair * createPair( char * key,  void * value) {
    Pair * new = (Pair *)malloc(sizeof(Pair));
    new->key = key;
    new->value = value;
    return new;
}

long hash( char * key, long capacity) {
    unsigned long hash = 0;
     char * ptr;
    for (ptr = key; *ptr != '\0'; ptr++) {
        hash += hash*32 + tolower(*ptr);
    }
    return hash%capacity;
}

int is_equal(void* key1, void* key2){
    if(key1==NULL || key2==NULL) return 0;
    if(strcmp((char*)key1,(char*)key2) == 0) return 1;
    return 0;
}

void insertMap(HashMap * map, char * key, void * value) {
    // Crear un nuevo par con la clave y el valor dados
    Pair * new_pair = createPair(key, value);

    // Calcular el índice del bucket usando la función hash
    long index = hash(key, map->capacity);

    // Si el bucket está vacío, insertar el nuevo par directamente
    if (map->buckets[index] == NULL) {
        map->buckets[index] = new_pair;
        map->size++;
    } else {
        // Si el bucket ya está ocupado, buscar una posición vacía usando sondaje lineal
        while (map->buckets[index] != NULL) {
            // Si la clave ya existe en el mapa, no insertar el nuevo par
            if (is_equal(map->buckets[index]->key, key)) {
                return;
            }
            index = (index + 1) % map->capacity;
        }
        map->buckets[index] = new_pair;
        map->size++;
    }

    // Actualizar el índice current a la posición del nuevo par
    map->current = index;
}



void enlarge(HashMap * map) {
  enlarge_called = 1; //no borrar (testing purposes)
  Pair**old_buckets = map-> buckets;
  map->capacity *= 2;
  map->buckets = (Pair **)calloc(map->capacity, sizeof(Pair *));
  map->size = 0; 
  for (int i = 0; i < map->capacity / 2; i++) {
    Pair *current = old_buckets[i];
    while (current != NULL) {
      insertMap(map, current->key, current->value);
      Pair *temp = current;
      current = current->next;
      free(temp);
}
} 
  free(old_buckets);
}
HashMap * createMap(long capacity) {
  
    // Reservar memoria para una variable de tipo HashMap
  HashMap * map = (HashMap *)malloc(sizeof(HashMap));

    // Reservar memoria para un arreglo de punteros a Pair
  map->buckets = (Pair **)calloc(capacity, sizeof(Pair *));

    // Inicializar el tamaño, la capacidad y el índice current
  map->size = 0;
  map->capacity = capacity;
  map->current = -1;

    // Retornar la variable de tipo HashMap
  return map;
}

void eraseMap(HashMap * map,  char * key) {
    // Buscar el par con la clave key
    Pair * pair = searchMap(map, key);

    // Si el par se encuentra, asignar NULL a la clave del par y actualizar el tamaño del mapa
    if (pair != NULL) {
        pair->key = NULL;
        map->size--;
    }
}


Pair * searchMap(HashMap * map,  char * key) {
    // Usar la función hash para obtener el índice del bucket
    long index = hash(key, map->capacity);

    // Si el bucket está vacío o tiene una clave distinta, avanzar hasta encontrar el par con la clave o llegar a un bucket vacío
    while (map->buckets[index] != NULL && !is_equal(map->buckets[index]->key, key)) {
        index = (index + 1) % map->capacity;
    }

    // Si el par con la clave se encuentra, retornarlo y actualizar el índice current
    if (map->buckets[index] != NULL && is_equal(map->buckets[index]->key, key)) {
        map->current = index;
        return map->buckets[index];
    }

    // Si el par con la clave no se encuentra, retornar NULL y no modificar el índice current
    return NULL;
} 
Pair * firstMap(HashMap * map) {
    // Inicializar el índice current a -1
    map->current = -1;

    // Recorrer el arreglo desde el inicio hasta encontrar un par válido o llegar al final
    for (int i = 0; i < map->capacity; i++) {
        // Si el bucket tiene un par válido, retornarlo y actualizar el índice current
        if (map->buckets[i] != NULL && map->buckets[i]->key != NULL) {
            map->current = i;
            return map->buckets[i];
        }
    }

    // Si no se encuentra ningún par válido, retornar NULL y no modificar el índice current
    return NULL;
}

Pair * nextMap(HashMap * map) {
    // Si el índice current es válido, recorrer el arreglo desde el índice current + 1 hasta encontrar un par válido o llegar al final
    if (map->current >= 0 && map->current < map->capacity) {
        for (int i = map->current + 1; i < map->capacity; i++) {
            // Si el bucket tiene un par válido, retornarlo y actualizar el índice current
            if (map->buckets[i] != NULL && map->buckets[i]->key != NULL) {
                map->current = i;
                return map->buckets[i];
            }
        }
    }

    // Si no se encuentra ningún par válido, retornar NULL y no modificar el índice current
    return NULL;
}
