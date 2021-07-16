#include <stdio.h>
#include <stdlib.h>

typedef struct candidate { /* Defining the sturct and defining new variable named elect*/
    char letter; // 'A', 'B', 'C', 'D'
    int votes;
    float percent;
} elect;

int reader(FILE* file)
{ /* Reading the file and calculating how much memory to allocate */
    int calc = 0;
    int number;

    while (!feof(file)) {/* Looping until the end of file */

        fscanf(file, "%d", &number); /* Allocating to some number just for the loop to continue */
        calc++; 
    }
    int district_num = (calc / 4); /* Number of districts */

    return district_num; /* Returning the district number */
}
void calculator(FILE* file, int array[], int district, elect* point_struct, char argu[])
{ /* Calculating each votes and percentages */

    int total_votes[4]; /* Holding the votes of candidates here */
    int counted_votes = 0; /* Total votes will be held here */
    int index = 0;

    for (int c = 0; c < 4; c++) { /* Loop for all candidates */
        file = fopen(argu, "r+"); /* Opening file */
        int times = 0, i = 0, number, total = 0, position = 0;
        position += c; /* Position means if the loop is for A,B,C or D */

        while (!feof(file)) { /* Loop until the end of file */

            if (times == position) {
                fscanf(file, "%d", &array[i]); /* Placing the number into adress */
                i++;
                position += 4; /* Since it adding +4 makes the columns same we take those into calculation for one candidate */

            } else {
                fscanf(file, "%d", &number); /* This is just for the reading to continue */
            }

            times++;
        }

        for (int x = 0; x < district; x++) {
            total += array[x]; /* Adding all votes for one candidate to each other */
        }
        (point_struct + index)->votes = total; /* Using my pointer to allocate value for vote */
        total_votes[index] = total;

        index++;

        fclose(file); /* Closing file for loop to start from the beginning of the file */
    }
    for (int x = 0; x < 4; x++) {
        counted_votes += total_votes[x]; /* Finding the sum of all votes in text file */
    }
    for (int j = 0; j < 4; j++) {
        (point_struct + j)->percent = ((float)total_votes[j] / (float)counted_votes) * 100; /* Using my pointer to allocate percentage values of candidates */
    }
}

void decider(elect* point_struct)
{ /* Finding biggest percentages by for loops */
    float temp1 = (point_struct)->percent;
    int index1 = 0, index2 = 0, winner = 0;

    for (int i = 0; i < 4; i++) {
        if ((point_struct + i)->percent > 50) { /* If there is a winner changing the value 'winner' to 1 */
            printf("Winner: %c\n", (point_struct + i)->letter); /* Printing out the winner */
            winner = 1;
            break;
        }

        if ((point_struct + i)->percent > temp1) { /* Comparing values on my pointed adresses */

            temp1 = (point_struct + i)->percent;

            index1 = i;
        }
    }
    float temp2 = 0; /* Calculating second most voted candidate */
    for (int i = 0; i < 4; i++) {

        if (((point_struct + i)->percent > temp2 && i != index1)) {
            temp2 = (point_struct + i)->percent;
            index2 = i;
        }
    }

    if (!winner) { /* If value of winner is not equal to 1 this if will be activated*/
        printf("Second round: %c and %c\n", (point_struct + index1)->letter, (point_struct + index2)->letter); /* Printing out second round candidates */
    }
}
int main(int argc, char* argv[])
{
    if (argc != 2) { /* Checking if the input is valid */
        printf("Error occured, put one and only one txt file\n");
        return 0;
    } else {

        FILE* file; /* Opening file */
        file = fopen(argv[1], "r+"); /* Since the file name is inside the first index of argv we put argv[1] */
        if (file != NULL) { /* Checking if the file can not be opened or empty */

            int district = reader(file); /* Finding district by reading the file and calling the function to read the votes from data */

            int* votes = (int*)malloc(district * sizeof(int)); /* Dynamic allocation of memory */
            
            elect* point_struct = NULL; /* Dynamic allocation for my pointer to struct to make my job easier */
            
            point_struct = (elect*)malloc(4 * sizeof(elect)); /* Creating 4 structs for candidates */
            
            for (int i = 0; i < 4; i++) { /* Adding letters by ascii numbers */

                (point_struct + i)->letter = 65 + i;
            }

            calculator(file, votes, district, point_struct, argv[1]); /* Calculating total votes and percentages */
            
            decider(point_struct); /*Deciding who is the winner */
            
            free(point_struct); /* Freeing pointers to get no issues */
            free(votes);
            
            votes=NULL; /* Declaring value as NULL */
            point_struct=NULL;
            
            fclose(file);
            
        } else { 
            printf("Error occured,can not open the file\n");
        }
    }
}
