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

Pair * firstMap(HashMap * map) {

    return NULL;
}

Pair * nextMap(HashMap * map) {

    return NULL;
}
