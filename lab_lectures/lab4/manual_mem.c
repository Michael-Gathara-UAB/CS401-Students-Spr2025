#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define OBJECT_COUNT 5
#define DATA_SIZE 1000000

typedef struct {
    char *name;
    int *data;
} MemoryHungryObject;

MemoryHungryObject* create_object(const char *name, int value) {
    MemoryHungryObject *obj = malloc(sizeof(MemoryHungryObject));
    obj->name = strdup(name);
    obj->data = malloc(DATA_SIZE * sizeof(int));
    for (int i = 0; i < DATA_SIZE; i++) {
        obj->data[i] = value;
    }
    return obj;
}

void free_object(MemoryHungryObject *obj) {
    free(obj->name);
    free(obj->data);
    free(obj);
}

void create_objects(MemoryHungryObject **objects) {
    for (int i = 0; i < OBJECT_COUNT; i++) {
        char name[20];
        sprintf(name, "object_%d", i);
        objects[i] = create_object(name, i);
        printf("Created %s\n", objects[i]->name);
    }
}

void print_memory_usage(MemoryHungryObject **objects) {
    printf("\nObjects memory usage\n");
    for (int i = 0; i < OBJECT_COUNT; i++) {
        if (objects[i] != NULL) {
            printf("%s: %zu Bytes\n", objects[i]->name, sizeof(int) * DATA_SIZE);
        }
    }
}

int main() {
    MemoryHungryObject *objects[OBJECT_COUNT] = {NULL};

    printf("Creating objects...\n");
    create_objects(objects);
    print_memory_usage(objects);

    printf("\nBefore manual cleanup:\n");
    printf("Current objects: ");
    for (int i = 0; i < OBJECT_COUNT; i++) {
        if (objects[i] != NULL) {
            printf("%s ", objects[i]->name);
        }
    }
    printf("\n");

    printf("\nPerforming manual cleanup...\n");
    free_object(objects[1]); 
    objects[1] = NULL;
    free_object(objects[3]);
    objects[3] = NULL;

    printf("\nRemaining objects memory usage:\n");
    for (int i = 0; i < OBJECT_COUNT; i++) {
        if (objects[i] != NULL) {
            printf("%s: %zu Bytes\n", objects[i]->name, sizeof(int) * DATA_SIZE);
        }
    }

    printf("\nAfter manual cleanup:\n");
    printf("Remaining objects: ");
    for (int i = 0; i < OBJECT_COUNT; i++) {
        if (objects[i] != NULL) {
            printf("%s ", objects[i]->name);
        }
    }
    printf("\n");

    for (int i = 0; i < OBJECT_COUNT; i++) {
        if (objects[i] != NULL) {
            free_object(objects[i]);
            objects[i] = NULL;
        }
    }

    printf("\nAfter manual cleanup two:\n");
    printf("Remaining objects: None\n");

    return 0;
}