#include "registry.h"

#define MAX_COMPONENTS 1
#define MAX_ENTITIES 1

#define TRANSFORM_COMPONENT 0

#define T Registry_T
struct T {
    void* componentPool[MAX_COMPONENTS][MAX_ENTITIES];
};

void Registry_init(T me) {}

void Registry_update(T me) {

} 

