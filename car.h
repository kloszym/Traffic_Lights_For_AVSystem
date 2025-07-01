#ifndef CAR_H
#define CAR_H

typedef enum{
	NORTH = 0,
	EAST = 1,
	SOUTH = 2,
	WEST = 3,
	NOTHING = 4,
} direction;

typedef struct{
	char vehicleId[50];
	direction startRoad;
	direction endRoad;
} Car;

Car defaultCar();

#endif //CAR_H
