#include <stdio.h>      // for printf()
#include <unistd.h>     // for fork(), pipe(), read(), write()
#include <stdlib.h>     // for exit(), malloc(), atoi()
#include <sys/types.h>  // for pid_t and system data types
#include <string.h>     // for strcpy(), strcmp()
#include "sensor.h"     // structure for SensorData

#include <pthread.h>    // for multithreading (threads)
#include <signal.h>     // for handling signals (kill, SIGINT, etc.)
#include <fcntl.h>      // for file control options (open flags)

#include <sys/ipc.h>    // for IPC (Inter-Process Communication)
#include <sys/shm.h>    // for shared memory functions
#include "shm_data.h"   // structure for shared memory data

	SharedData *shm;   // pointer to shared memory
	int running = 1;   // flag to control program execution

	float voltage = 0, current = 0, power = 0, frequency = 0;

	pthread_mutex_t lock; // mutex for synchronization

// -------------------------- THREADS-----------------------------------


   //VOLTAGE
	void* voltage_thread(void* arg)
	{
	    int fd = *(int*)arg;
	    SensorData data;

	    while(running)
	    {
		if(read(fd, &data, sizeof(data)) > 0)
		{
		    pthread_mutex_lock(&lock);
		    voltage = data.value;
		    shm->voltage = data.value;
		    pthread_mutex_unlock(&lock);
		}
	    }
	    return NULL;
	}
    //CURRENT
    	void* current_thread(void* arg)
	{
	    int fd = *(int*)arg;
	    SensorData data;

	    while(running)
	    {
		if(read(fd, &data, sizeof(data)) > 0)
		{
		    pthread_mutex_lock(&lock);
		    current = data.value;
		    shm->current = data.value;
		    pthread_mutex_unlock(&lock);
		}
	    }
	    return NULL;
	}
	
    //POWER
    
 	 void* power_thread(void* arg)
	{
	    int fd = *(int*)arg;
	    SensorData data;

	    while(running)
	    {
		if(read(fd, &data, sizeof(data)) > 0)
		{
		    pthread_mutex_lock(&lock);
		    power = data.value;
		    shm->power = data.value;
		    pthread_mutex_unlock(&lock);
		}
	    }
	    return NULL;
	}
	
    //FREQUENCY
    
    	void* frequency_thread(void* arg)
	{
	    int fd = *(int*)arg;
	    SensorData data;

	    while(running)
	    {
		if(read(fd, &data, sizeof(data)) > 0)
		{
		    pthread_mutex_lock(&lock);
		    frequency = data.value;
		    shm->frequency = data.value;
		    pthread_mutex_unlock(&lock);
		}
	    }
	    return NULL;
	}
	
//----------------------------SIGNAL HANDLING-------------------------------
	void handle_sigint(int sig)
	{
	    printf("\nShutting down safely...\n");
	    running = 0; // stop all the loops
	}
	
	
int main()
{
        signal(SIGINT, handle_sigint); 
	pthread_mutex_init(&lock, NULL);
	
        // pipes for each sensor
	int fd_voltage[2];
	int fd_current[2];
	int fd_power[2];
	int fd_frequency[2];

        // create pipes
	pipe(fd_voltage);
	pipe(fd_current);
	pipe(fd_power);
	pipe(fd_frequency);


 // -----------------------PROCESSES ----------------------------
     
     
      //VOLTAGE
	pid_t pid1 = fork();

	if(pid1 == 0)
	{
	    close(fd_voltage[0]); // close read end

	    char fd_str[10];
	    sprintf(fd_str, "%d", fd_voltage[1]);

	    execl("./voltage_sensor", "voltage_sensor", fd_str, NULL);

	    perror("Exec failed");
	    exit(1);
	}
	
     
      //CURRENT    
	pid_t pid2 = fork();

	if(pid2 == 0)
	{
	    close(fd_current[0]); // close read end

	    char fd_str[10];
	    sprintf(fd_str, "%d", fd_current[1]);

	    execl("./current_sensor", "current_sensor", fd_str, NULL);

	    perror("Exec failed");
	    exit(1);
	}
	
	
      //POWER
	pid_t pid3 = fork();

	if(pid3 == 0)
	{
	    close(fd_power[0]);
	    
	    char fd_str[10];
	    sprintf(fd_str, "%d", fd_power[1]);
	    
	    execl("./power_sensor", "power_sensor", fd_str, NULL);
	    
	    perror("Exec failed");
	    exit(1);
	}
	
	
      //FREQENCY
	pid_t pid4 = fork();
	
	if(pid4 == 0)
	{
	    close(fd_frequency[0]);
	    
	    char fd_str[10];
	    sprintf(fd_str, "%d", fd_frequency[1]);
	    
	    execl("./frequency_sensor", "frequency_sensor", fd_str, NULL);
	    
	    perror("Exec failed");
	    exit(1);
	}
    else
        {
	// Close write ends
	close(fd_voltage[1]);
	close(fd_current[1]);
	close(fd_power[1]);
	close(fd_frequency[1]);

   	}
   	
//-------------------------SHARED MEMORY---------------------------
    
   	key_t key = 1234;

	int shmid = shmget(key, sizeof(SharedData), 0666 | IPC_CREAT);
	if(shmid < 0)
	{
	    perror("shmget failed");
	    return 1;
	}

	shm = (SharedData*) shmat(shmid, NULL, 0);
	if(shm == (void*)-1)
	{
	    perror("shmat failed");
	    return 1;
	}
	
	
//--------------------------THREADS------------------------------------
	pthread_t t1, t2, t3, t4;

	pthread_create(&t1, NULL, voltage_thread, &fd_voltage[0]);
	pthread_create(&t2, NULL, current_thread, &fd_current[0]);
	pthread_create(&t3, NULL, power_thread, &fd_power[0]);
	pthread_create(&t4, NULL, frequency_thread, &fd_frequency[0]);
	
 //-------------------------- LOG FILE ----------------------------------	
	int fd_log = open("log.txt", O_CREAT | O_WRONLY | O_APPEND, 0644);

	if(fd_log < 0)
	{
	    perror("File open failed");
	    return 1;
	}

	char buffer[100];


	while(running)
	{
	    pthread_mutex_lock(&lock);

	    int len = sprintf(buffer,
		"V: %.2f | I: %.2f | P: %.2f | F: %.2f\n",
		shm->voltage,
		shm->current,
		shm->power,
		shm->frequency);

	    printf("%s", buffer);

	    lseek(fd_log, 0, SEEK_END);

	    //write to log file
	    int w = write(fd_log, buffer, len);
	    if(w < 0)
	    {
		perror("Write failed");
	    }

	    pthread_mutex_unlock(&lock);

	    usleep(500000);// 0.5 sec delay
	}

 //---------------------cleanup------------------------------
 
	pthread_join(t1, NULL);
	pthread_join(t2, NULL);
	pthread_join(t3, NULL);
	pthread_join(t4, NULL);

	close(fd_log);
	shmdt(shm);           
	shmctl(shmid, IPC_RMID, NULL); 
	pthread_mutex_destroy(&lock); 
	printf("Program exited cleanly.\n");

	return 0;
}
