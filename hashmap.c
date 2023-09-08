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
    
    HashMap * map = (HashMap *)malloc(sizeof(HashMap));
    map->buckets = (Pair **)calloc(capacity, sizeof(Pair *));
    map->size = 0;
    map->capacity = capacity;
    map->current = -1;
    return map;
}



void eraseMap(HashMap * map,  char * key) {    
  Pair* pair = searchMap(map,key);
if (pair != NULL){ 
  pair->key = NULL;
  map -> size --;
}
}

Pair * searchMap(HashMap * map,  char * key) {   
  int pos = hash(key, map->capacity);
  int i = pos; 
  do{
    if (map->buckets[i] == NULL)return NULL;
    if(strcmp(map->buckets[i]->key, key) == 0) {
      map-> current= i;
      return map-> buckets[i];
    }
    i=( i+1)% map -> capacity;
  }
   while( i!= pos);
  return  NULL;

}

Pair *firstMap(HashMap *map) {
    if (map == NULL || map->buckets == NULL) {
        return NULL;
    }

    for (int i = 0; i < map->capacity; i++) {
        if (map->buckets[i] != NULL) {
            map->current_index = i; // Establecer el índice actual
            return map->buckets[i];
        }
    }

    return NULL;
}

Pair *nextMap(HashMap *map) {
    if (map == NULL || map->buckets == NULL) {
        return NULL;
    }

    int current_index = map->current_index;

    if (current_index >= 0 && current_index < map->capacity) {
        // Intentar encontrar el siguiente Pair en el mismo bucket
        if (map->buckets[current_index]->next != NULL) {
            return map->buckets[current_index]->next;
        }
        // Si no hay un siguiente en el mismo bucket, buscar el siguiente bucket no vacío
        for (int i = current_index + 1; i < map->capacity; i++) {
            if (map->buckets[i] != NULL) {
                map->current_index = i; // Actualizar el índice actual
                return map->buckets[i];
            }
        }
    }

    return NULL; // No se encontró un siguiente Pair
}



Pair * nextMap(HashMap * map) {
    int i;
    for (i = map->current + 1; i < map->size; i++) {
        if (map->buckets[i].key != NULL) {
            map->current = i;
           return &(map->buckets[i]);
        }
    }
    return NULL;
}
