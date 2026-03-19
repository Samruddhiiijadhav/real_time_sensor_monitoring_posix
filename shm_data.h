#ifndef SHM_DATA_H
#define SHM_DATA_H

typedef struct {
    float voltage;
    float current;
    float power;
    float frequency;
} SharedData;

#endif
