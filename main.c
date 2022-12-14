#include <stdio.h> //library for fscanf, fprintf, fopen and fclose
#include <string.h> //library for strlen, strcpy, strcmp abd strcat
#include <stdlib.h> //library for itoa
#include <ctype.h> //library for isalpha and isupper

FILE *file_in, *file_out; //global variables for input and output files
#define ELEM_SIZE 200 //global variable for array's sizes

struct player{ //struct for more comfortable saving information about players
    char name[ELEM_SIZE];
    int teamNumber;
    int power;
    char visibility[ELEM_SIZE];
    char teamName[ELEM_SIZE];
};

void error() { //function to print "Invalid inputs"
    fclose(file_out);
    file_out = fopen("output.txt", "w");
    fprintf(file_out, "Invalid inputs\n");
    fclose(file_out);
    fclose(file_in);
    exit(0);
};

int main() {
    struct player player[ELEM_SIZE]; //initializes array of structs for player

    for (int i = 0; i < ELEM_SIZE; ++i) { //sets to zero all elements of arrays player.teamNumber and player.power
        player[i].teamNumber = 0;
        player[i].power = 0;
    }
    //opens input and output files
    file_in = fopen("input.txt", "r");
    file_out = fopen("output.txt", "w");

    int N; //N - given number of teams
    fscanf(file_in, "%d", &N);
    if (N < 1 || N > 10) { //checks if 1 <= n <= 10
        error();
    }

    for (int i = 0; i < N; ++i) { //cycle to scan team names and check if data is valid
        if (fscanf(file_in, "%s", player[i].teamName) == EOF) {
            error();
        }
        if (strlen(player[i].teamName) < 1 || strlen(player[i].teamName) > 20 || !(isupper(
                player[i].teamName[0]))) {
            error();
        }
        for (int j = 1; j < strlen(player[i].teamName); ++j)
            if (!(isalpha(player[i].teamName[j]))) {
                error();
            }
    }
    int M;
    fscanf(file_in, "%d", &M); //input M - the number of players
    if (!(M >= N && M <= 100)) error(); //check if M is valid

    //cycle to scan players from input file and check if player's data is valid
    for (int i = 0; i < M; ++i) {
        fscanf(file_in, "%s", player[i].name);
        fscanf(file_in, "%d", &player[i].teamNumber);
        if (player[i].teamNumber < 0 || player[i].teamNumber >= N) {
            error();
        }
        fscanf(file_in, "%d", &player[i].power);
        if (player[i].power < 0 || player[i].power > 1000) {
            error();
        }
        fscanf(file_in, "%s", player[i].visibility);
        if (strlen(player[i].name) < 1 || strlen(player[i].name) > 20 || !isupper(player[i].name[0]) ||
            !(strcmp(player[i].visibility, "True") == 0 || strcmp(player[i].visibility, "False") == 0)) {
            error();
        } else
            for (int j = 1; j < strlen(player[i].name); j++)
                if (!(isalpha(player[i].name[j]))) {
                    error();
                }
    }

    char actionType[100]; //array for type of action
    char name1[ELEM_SIZE]; //array for first name in action
    char name2[ELEM_SIZE]; //array for second name in action
    int actionsAmount = 0; //integer for counting actions
    int supersAmount = 0; //integer for counting supers
    while (fscanf(file_in, "%s", actionType) != EOF) { //cycle for scanning and checking actions
        actionsAmount++;
        if (actionsAmount > 1000) error(); //checks if amount of actions is valid

        //logic block for attack action type
        if (strcmp(actionType, "attack") == 0) {
            fscanf(file_in, "%s", name1);
            fscanf(file_in, "%s", name2);
            int fg1 = 0; //fg1 and fg2 - flags to check if players with given name exist
            int fg2 = 0;
            for (int r = 0; r < ELEM_SIZE; ++r) {
                if (strcmp(player[r].name, name1) == 0) fg1 = 1;
                if (strcmp(player[r].name, name2) == 0) fg2 = 1;
            }
            if (fg1 == 0 || fg2 == 0) error();
            int i = 0;
            int j = 0;
            while (strcmp(player[i].name, name1) != 0)i++;
            while (strcmp(player[j].name, name2) != 0)j++;
            if (strcmp(player[i].visibility, "True") == 0) {
                if (player[i].power != 0) {
                    if (strcmp(player[j].visibility, "True") == 0) {
                        if (player[i].power > player[j].power) {
                            player[i].power += player[i].power - player[j].power;
                            if (player[i].power > 1000) player[i].power = 1000;
                            player[j].power = 0;
                        } else if (player[j].power > player[i].power) {
                            player[j].power += player[j].power - player[i].power;
                            if (player[j].power > 1000) player[j].power = 1000;
                            player[i].power = 0;
                        } else {
                            player[i].power = 0;
                            player[j].power = 0;
                        }
                    } else player[i].power = 0;
                } else fprintf(file_out, "This player is frozen\n");
            } else fprintf(file_out, "This player can't play\n");

        //logic block for flip_visibility action type
        } else if (strcmp(actionType, "flip_visibility") == 0) {
            fscanf(file_in, "%s", name1);
            int fg1 = 0; //fg1 - flag to check if player with given name exists
            for (int r = 0; r < ELEM_SIZE; ++r) {
                if (strcmp(player[r].name, name1) == 0) fg1 = 1;
            }
            if (fg1 == 0) error();
            int i = 0;
            while (strcmp(player[i].name, name1) != 0)i++;
            if (player[i].power != 0) {
                if (strcmp(player[i].visibility, "True") == 0) strcpy(player[i].visibility, "False");
                else strcpy(player[i].visibility, "True");
            } else fprintf(file_out, "This player is frozen\n");

        //logic block for heal action type
        } else if (strcmp(actionType, "heal") == 0) {
            fscanf(file_in, "%s", name1);
            fscanf(file_in, "%s", name2);
            int fg1 = 0; //fg1 and fg2 - flags to check if players with given name exist
            int fg2 = 0;
            for (int r = 0; r < ELEM_SIZE; ++r) {
                if (strcmp(player[r].name, name1) == 0) fg1 = 1;
                if (strcmp(player[r].name, name2) == 0) fg2 = 1;
            }
            if (fg1 == 0 || fg2 == 0) error();
            int i = 0;
            int j = 0;
            while (strcmp(player[i].name, name1) != 0)i++;
            while (strcmp(player[j].name, name2) != 0)j++;
            if (!strcmp(player[i].visibility, "True")) {
                if (player[i].power != 0) {
                    if (player[i].teamNumber == player[j].teamNumber) {
                        if (i != j) {
                            player[j].power += (player[i].power + 1) / 2;
                            if (player[j].power > 1000) player[j].power = 1000;
                            player[i].power = (player[i].power + 1) / 2;
                        } else fprintf(file_out, "The player cannot heal itself\n");
                    } else fprintf(file_out, "Both players should be from the same team\n");
                } else fprintf(file_out, "This player is frozen\n");
            } else fprintf(file_out, "This player can't play\n");

        //logic block for super action type
        } else if (strcmp(actionType, "super") == 0) {
            fscanf(file_in, "%s", name1);
            fscanf(file_in, "%s", name2);
            int fg1 = 0; //fg1 and fg2 - flags to check if players with given name exist
            int fg2 = 0;
            for (int r = 0; r < ELEM_SIZE; ++r) {
                if (strcmp(player[r].name, name1) == 0) fg1 = 1;
                if (strcmp(player[r].name, name2) == 0) fg2 = 1;
            }
            if (fg1 == 0 || fg2 == 0) error();
            int i = 0;
            int j = 0;
            while (strcmp(player[i].name, name1) != 0)i++;
            while (strcmp(player[j].name, name2) != 0)j++;
            if (!strcmp(player[i].visibility, "True")) {
                if (player[i].power != 0) {
                    if (player[i].teamNumber == player[j].teamNumber) {
                        if (i != j) {
                            char charK[10];
                            itoa(supersAmount, charK, 10);
                            strcpy(player[ELEM_SIZE - 1 - supersAmount].name, "S_");
                            strcat(player[ELEM_SIZE - 1 - supersAmount].name, charK);
                            player[ELEM_SIZE - 1 - supersAmount].power = player[i].power + player[j].power;
                            if (player[ELEM_SIZE - 1 - supersAmount].power > 1000) player[ELEM_SIZE - 1 - supersAmount].power = 1000;
                            strcpy(player[ELEM_SIZE - 1 - supersAmount].visibility, "True");
                            player[i].power = 0;
                            player[j].power = 0;
                            strcpy(player[i].name, "");
                            strcpy(player[j].name, "");
                            player[ELEM_SIZE - 1 - supersAmount].teamNumber = player[i].teamNumber;
                            supersAmount++;
                        } else fprintf(file_out, "The player cannot do super action with itself\n");
                    } else fprintf(file_out, "Both players should be from the same team\n");
                } else fprintf(file_out, "This player is frozen\n");
            } else fprintf(file_out, "This player can't play\n");
        } else error();
    }

    //counts final power of each team
    int teamPowers[ELEM_SIZE];
    for (int i = 0; i < ELEM_SIZE; ++i) teamPowers[i] = 0;
    for (int i = 0; i < ELEM_SIZE; ++i) {
        teamPowers[player[i].teamNumber] += player[i].power;
    }

    //determines the team with the highest power
    int maxPower = 0;
    for (int i = 0; i < ELEM_SIZE; ++i)
        if (teamPowers[i] > maxPower) {
            maxPower = teamPowers[i];
        }
    for (int i = 0; i < ELEM_SIZE; ++i)
        for (int j = 0; j < ELEM_SIZE; ++j)
            if (teamPowers[i] == teamPowers[j] && teamPowers[i] == maxPower && i != j) {
                fprintf(file_out, "It's a tie\n");
                fclose(file_out);
                fclose(file_in);
                return 0;
            }
    for (int i = 0; i < ELEM_SIZE; ++i)
        if (teamPowers[i] == maxPower) {
            fprintf(file_out, "The chosen wizard is %s\n", player[i].teamName);
            fclose(file_out);
            fclose(file_in);
            return 0;
        }
}
