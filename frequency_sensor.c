#include <unistd.h>     // for write(), usleep()
#include <string.h>     // for strcpy()
#include <stdlib.h>     // for atoi(), rand(), srand()
#include <time.h>       // for time()
#include "sensor.h"     // structure for SensorData

int main(int argc, char *argv[])
{
    // Convert command line argument to integer
    // argv[1] = file descriptor to (write end of pipe) passed by the controller
    
    int fd = atoi(argv[1]);

    SensorData data; // structure for sensor information

    // to have random number generated using current time
    srand(time(NULL));

    while(1)
    {
        // sensor name
        strcpy(data.sensor_name, "Frequency");

        // Generate random frequency value between 49–51
        data.value = 49 + rand() % 3;

        // Store current timestamp
        data.timestamp = time(NULL);

        // Write structured data to pipe
        // This sends data to the controller process
        write(fd, &data, sizeof(data));

        // Delay to simulate 500 Hz sampling rate
        usleep(2000); 
    }

    return 0;
}
