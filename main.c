#include "simulation_logic.h"
#include <stdio.h>


void save_state() {
    FILE* f = fopen(STATE_FILE, "wb");
    if (!f) return;
    fwrite(twoLanesCrossroad, sizeof(twoLanesCrossroad), 1, f);
    fwrite(currentOccupationOfStates, sizeof(currentOccupationOfStates), 1, f);
    fwrite(&currentState, sizeof(currentState), 1, f);
    fwrite(&proportionOfTwoMostOccupiedStates, sizeof(proportionOfTwoMostOccupiedStates), 1, f);
    fwrite(&northStraightLight, sizeof(uint8_t), 1, f); fwrite(&southStraightLight, sizeof(uint8_t), 1, f);
    fwrite(&eastStraightLight, sizeof(uint8_t), 1, f); fwrite(&westStraightLight, sizeof(uint8_t), 1, f);
    fwrite(&northLeftLight, sizeof(uint8_t), 1, f); fwrite(&southLeftLight, sizeof(uint8_t), 1, f);
    fwrite(&eastLeftLight, sizeof(uint8_t), 1, f); fwrite(&westLeftLight, sizeof(uint8_t), 1, f);
    fwrite(&northRightLight, sizeof(uint8_t), 1, f); fwrite(&southRightLight, sizeof(uint8_t), 1, f);
    fwrite(&eastRightLight, sizeof(uint8_t), 1, f); fwrite(&westRightLight, sizeof(uint8_t), 1, f);
    fclose(f);
}

void load_state() {
    FILE* f = fopen(STATE_FILE, "rb");
    if (!f) {
        for (int i = 0; i < 8; i++) initializeLaneQueue(&twoLanesCrossroad[i]);
        turnAllRedLamps(); currentState = state4;
        proportionOfTwoMostOccupiedStates = UINT16_MAX;
        memset(currentOccupationOfStates, 0, sizeof(currentOccupationOfStates));
        return;
    }
    fread(twoLanesCrossroad, sizeof(twoLanesCrossroad), 1, f);
    fread(currentOccupationOfStates, sizeof(currentOccupationOfStates), 1, f);
    fread(&currentState, sizeof(currentState), 1, f);
    fread(&proportionOfTwoMostOccupiedStates, sizeof(proportionOfTwoMostOccupiedStates), 1, f);
    fread(&northStraightLight, sizeof(uint8_t), 1, f); fread(&southStraightLight, sizeof(uint8_t), 1, f);
    fread(&eastStraightLight, sizeof(uint8_t), 1, f); fread(&westStraightLight, sizeof(uint8_t), 1, f);
    fread(&northLeftLight, sizeof(uint8_t), 1, f); fread(&southLeftLight, sizeof(uint8_t), 1, f);
    fread(&eastLeftLight, sizeof(uint8_t), 1, f); fread(&westLeftLight, sizeof(uint8_t), 1, f);
    fread(&northRightLight, sizeof(uint8_t), 1, f); fread(&southRightLight, sizeof(uint8_t), 1, f);
    fread(&eastRightLight, sizeof(uint8_t), 1, f); fread(&westRightLight, sizeof(uint8_t), 1, f);
    fclose(f);
}

const char* direction_to_string(direction d) {
	switch (d) {
		case NORTH: return "NORTH";
		case SOUTH: return "SOUTH";
		case EAST: return "EAST";
		case WEST: return "WEST";
		default: return "NOTHING";
	}
}

direction string_to_direction(const char* s) {
	if (strcmp(s, "NORTH") == 0) return NORTH;
	if (strcmp(s, "SOUTH") == 0) return SOUTH;
	if (strcmp(s, "EAST") == 0) return EAST;
	if (strcmp(s, "WEST") == 0) return WEST;
	return NOTHING;
}

int main(int argc, char* argv[]) {
	load_state();

	if (argc < 2) {
		fprintf(stderr, "Error: No command provided. Usage: %s [addVehicle|step|getState]\n", argv[0]);
		return 1;
	}

	if (strcmp(argv[1], "addVehicle") == 0) {
		if (argc != 5) {
			fprintf(stderr, "Usage: %s addVehicle <id> <start> <end>\n", argv[0]);
			return 1;
		}
		direction start = string_to_direction(argv[3]);
		direction end = string_to_direction(argv[4]);
		if (start == NOTHING || end == NOTHING) {
			fprintf(stderr, "Error: Invalid direction provided.\n");
			return 1;
		}
		addVehicle(argv[2], start, end);
	} else if (strcmp(argv[1], "step") == 0) {
		Car leftCars[MAX_TRAFFIC_SIZE] = {0};
		for (int i = 0; i < MAX_TRAFFIC_SIZE; i++) leftCars[i] = defaultCar();
		step(leftCars);
		for (int i = 0; i < MAX_TRAFFIC_SIZE && leftCars[i].startRoad != NOTHING; i++) {
			printf("%s\n", leftCars[i].vehicleId);
		}
	} else if (strcmp(argv[1], "getState") == 0) {
		printf("LIGHTS:%d %d %d %d %d %d %d %d %d %d %d %d\n",
		   northStraightLight, southStraightLight, eastStraightLight, westStraightLight,
		   northLeftLight, southLeftLight, eastLeftLight, westLeftLight,
		   northRightLight, southRightLight, eastRightLight, westRightLight);
		for (int i = 0; i < 8; i++) {
			printf("LANE:%d:%d\n", i, size(&twoLanesCrossroad[i]));
		}
	} else {
		fprintf(stderr, "Error: Unknown command '%s'.\n", argv[1]);
		return 1;
	}

	save_state();
	return 0;
}