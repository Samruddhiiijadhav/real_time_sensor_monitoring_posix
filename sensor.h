#ifndef SENSOR_H
#define SENSOR_H

#include <time.h>

#define MAX_NAME_LEN 20

// Sensor types (helps identify source easily)
typedef enum {
    VOLTAGE,
    CURRENT,
    POWER,
    FREQUENCY
} SensorType;

// Main data structure
typedef struct {
    SensorType type;             // which sensor
    char sensor_name[MAX_NAME_LEN];
    float value;
    time_t timestamp;
} SensorData;

// Buffer size for shared memory
#define BUFFER_SIZE 100

#endif
