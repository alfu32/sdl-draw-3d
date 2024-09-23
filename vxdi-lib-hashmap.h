#ifndef __VXDI_LIB_HASHMAP_H__  // Include guard to prevent double inclusion
#define __VXDI_LIB_HASHMAP_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#define HASH_MAP_SIZE 10000

// Structure for each hash map entry with fixed-size keys
typedef struct {
    char key[32];  // Fixed-size key (example: 32 characters)
    void* value;   // Pointer to the original element (array)
} hash_entry_t;

// Structure for the hash map
typedef struct {
    hash_entry_t* entries[HASH_MAP_SIZE];  // Array of pointers to hash entries
    int size;       // Number of elements in the hash map
    int key_size;   // Fixed size of the key
} hash_map_t;

// Function prototypes
void hash_map__add(hash_map_t* self, const char* key, void* value);
void hash_map__remove(hash_map_t* self, const char* key);
int hash_map__index_of_element(hash_map_t* self, const char* key);
void hash_map__clear(hash_map_t* self);
unsigned int hash_function(const char* key, int key_size);

// Hash function (simple string-based, uses key size to limit the hash range)
unsigned int hash_function(const char* key, int key_size) {
    unsigned int hash = 0;
    for (int i = 0; i < key_size && key[i] != '\0'; i++) {
        hash = (hash << 5) + key[i];
    }
    return hash % HASH_MAP_SIZE;
}

// Initialize the hash map with a specified key size
hash_map_t* hash_map__create(int key_size) {
    hash_map_t* map = (hash_map_t*)malloc(sizeof(hash_map_t));
    map->size = 0;
    map->key_size = key_size;
    for (int i = 0; i < HASH_MAP_SIZE; i++) {
        map->entries[i] = NULL;
    }
    return map;
}

// Add or replace a key-value pair in the hash map
void hash_map__add(hash_map_t* self, const char* key, void* value) {
    unsigned int index = hash_function(key, self->key_size);

    // Check if the key already exists and replace its value
    if (self->entries[index] != NULL && strncmp(self->entries[index]->key, key, self->key_size) == 0) {
        self->entries[index]->value = value;  // Replace the value
        printf("Replaced value for key '%s' at index %d.\n", key, index);
        return;
    }

    // Create a new hash entry if the key doesn't exist
    hash_entry_t* entry = (hash_entry_t*)malloc(sizeof(hash_entry_t));
    strncpy(entry->key, key, self->key_size);  // Copy the key with a fixed size
    entry->value = value;                     // Set the value

    // Insert the new entry into the hash map
    self->entries[index] = entry;
    self->size++;
    printf("Added element with key '%s' at index %d.\n", key, index);
}

// Remove a key-value pair from the hash map
void hash_map__remove(hash_map_t* self, const char* key) {
    unsigned int index = hash_function(key, self->key_size);

    if (self->entries[index] != NULL && strncmp(self->entries[index]->key, key, self->key_size) == 0) {
        free(self->entries[index]);
        self->entries[index] = NULL;
        self->size--;
        printf("Removed element with key '%s' from index %d.\n", key, index);
    } else {
        printf("Key '%s' not found.\n", key);
    }
}

// Get the index of a key in the hash map
int hash_map__index_of_element(hash_map_t* self, const char* key) {
    unsigned int index = hash_function(key, self->key_size);

    if (self->entries[index] != NULL && strncmp(self->entries[index]->key, key, self->key_size) == 0) {
        return index;  // Return the index if the key is found
    }
    return -1;  // Return -1 if the key is not found
}

// Free all memory used by the hash map
void hash_map__destroy(hash_map_t* self) {
    for (int i = 0; i < HASH_MAP_SIZE; i++) {
        if (self->entries[i] != NULL) {
            free(self->entries[i]);
        }
    }
    free(self);
}

// Clear all entries in the hash map
void hash_map__clear(hash_map_t* self) {
    for (int i = 0; i < HASH_MAP_SIZE; i++) {
        if (self->entries[i] != NULL) {
            free(self->entries[i]);  // Free each entry
            self->entries[i] = NULL; // Set the entry to NULL
        }
    }
    self->size = 0;  // Reset the size of the hash map
    // printf("Hash map cleared.\n");
}

// Example usage
int hash_map__test() {
    // Create the hash map with a fixed key size of 32 characters
    hash_map_t* map = hash_map__create(32);
    
    // Add elements to the map
    int value1 = 100;
    int value2 = 200;
    int value3 = 300;

    hash_map__add(map, "key1", &value1);
    hash_map__add(map, "key2", &value2);
    hash_map__add(map, "key3", &value3);

    // Replace an existing key
    hash_map__add(map, "key2", &value1);  // Replace "key2" with a new value

    // Get the index of a given element
    int index = hash_map__index_of_element(map, "key2");
    if (index != -1) {
        printf("Index of 'key2': %d\n", index);
        printf("Value at 'key2': %d\n", *(int*)map->entries[index]->value);
    }

    // Remove an element
    hash_map__remove(map, "key2");

    // Try finding the removed element
    index = hash_map__index_of_element(map, "key2");
    if (index == -1) {
        printf("'key2' not found after removal.\n");
    }

    // Destroy the hash map
    hash_map__destroy(map);

    return 0;
}


#endif // HASHMAP_H