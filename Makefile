CC = gcc
CFLAGS = -Wall -pthread

# Target executables
SENSORS = voltage_sensor current_sensor power_sensor frequency_sensor
CONTROLLER = controller

all: $(SENSORS) $(CONTROLLER)

# Compile each sensor
voltage_sensor: voltage_sensor.c sensor.h
	$(CC) $(CFLAGS) voltage_sensor.c -o voltage_sensor

current_sensor: current_sensor.c sensor.h
	$(CC) $(CFLAGS) current_sensor.c -o current_sensor

power_sensor: power_sensor.c sensor.h
	$(CC) $(CFLAGS) power_sensor.c -o power_sensor

frequency_sensor: frequency_sensor.c sensor.h
	$(CC) $(CFLAGS) frequency_sensor.c -o frequency_sensor

# Compile controller
controller: controller.c sensor.h shm_data.h
	$(CC) $(CFLAGS) controller.c -o controller

# Clean build files
clean:
	rm -f $(SENSORS) $(CONTROLLER) *.o log.txt
