#include <unistd.h>     // write(), usleep()
#include <string.h>     // strcpy()
#include <stdlib.h>     // atoi(), rand(), srand()
#include <time.h>       // time()
#include "sensor.h"     // SensorData structure

int main(int argc, char *argv[])
{
    // fd received from controller (write end of pipe)
    int fd = atoi(argv[1]);

    // structure to store sensor data
    SensorData data;

    // initialize random generator using current time
    srand(time(NULL));

    while(1)
    {
        //sensor type
        strcpy(data.sensor_name, "Current");

        // simulate current value of range: 4 to 9A
        data.value = 4 + rand() % 6;

        // store current timestamp
        data.timestamp = time(NULL);

        // send data to controller through pipe
        write(fd, &data, sizeof(data));

        // small delay of 500 samples per second
        usleep(2000);
    }

    return 0;
}
