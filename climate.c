/*
Antonio Gutierrez
Project 3
CS221
*/
#include <float.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define NUM_STATES 50

/* TODO: Add elements to the climate_info struct as necessary. */
struct climate_info {
    char code[3];
    long double max_temp;
    long max_timestamp;
    long double min_temp;
    long min_timestamp;
    unsigned long num_records;
    long double sum_temperature;
    long double sum_humidity;
    long double sum_snow;
    long double sum_cloud;
    long double sum_lightning;
};

void analyze_file(FILE *file, struct climate_info *states[], int num_states);
void print_report(struct climate_info *states[], int num_states);

int main(int argc, char *argv[]) {

    /* TODO: fix this conditional. You should be able to read multiple files. */
    if (argc < 2) {
        printf("Usage: %s tdv_file1 tdv_file2 ... tdv_fileN \n", argv[0]);
        return EXIT_FAILURE;
    }

    /* Let's create an array to store our state data in. As we know, there are
     * 50 US states. */
    struct climate_info *states[NUM_STATES] = { NULL };
    FILE* fp;
    int i;
    for (i = 1; i < argc; ++i) {
        /* TODO: Open the file for reading */
        printf("Opening file: %s\n", argv[i]);
        fp = fopen(argv[i], "r"); // opening the file
        /* TODO: If the file doesn't exist, print an error message and move on
         * to the next file. */
        if (fp != NULL){ // checks to make sure the file exists
            analyze_file(fp, states, NUM_STATES);
            fclose(fp); //closing the file
        }
        /* TODO: Analyze the file */
        /* analyze_file(file, states, NUM_STATES); */
    }

    /* Now that we have recorded data for each file, we'll summarize them: */
    print_report(states, NUM_STATES);
    int lenght = 0;
    while (states[lenght] == NULL){ // will make sure to free all allocated memory in the array
        free(states[lenght]);
        i++;
    }

    return 0;
}

void analyze_file(FILE *file, struct climate_info **states, int num_states) {
    const int line_sz = 100;
    char line[line_sz];
    char* temp = NULL;
    int timestamp;
    
    int found = -1;
    char* number;
    int i = 0;
    
    
    while (fgets(line, line_sz, file) != NULL) {
        temp = strtok(line, "\t");
        found = -1;// must reset so it checks for every new line
        i = 0;//must reset so it checks for every new line
        while(states[i] != NULL){
            if((temp[0]) == ((states[i]) -> code[0]) && (temp[1]) == ((states[i]) -> code[1]))
                found = i; // if found, I know where to assign the data to
            i++;
        }
        if(found != -1){
        // we need to check if the state has been added to states yet, if not we add it.

         // its '\t' because in memory thats how tabs are saved.    
                (states[found]) -> num_records += 1;
                temp = strtok(NULL, "\t"); // now its on timestamp
                timestamp = atoi(temp); // storing the address of both stamps
                temp = strtok(NULL, "\t"); // now its on geo-location
                temp = strtok(NULL, "\t"); // now its on humidity
                (states[found]) -> sum_humidity += atoi(temp);
                temp = strtok(NULL, "\t"); // now its on snow cover
                if(atoi(temp) > 0)
                    (states[found]) -> sum_snow += 1;
                temp = strtok(NULL, "\t"); // now its on cloud cover
                (states[found]) -> sum_cloud += atoi(temp);
                temp = strtok(NULL, "\t"); // now its on lightning strikes
                if(atoi(temp) != 0)
                    (states[found]) -> sum_lightning += 1;
                temp = strtok(NULL, "\t"); // now its on pressure
                temp = strtok(NULL, "\t"); // now its on temperature (KELVIN)
                (states[found]) -> sum_temperature += atoi(temp);
                if((states[found]) -> max_temp < atoi(temp)){ //checking if we have new max
                    (states[found]) -> max_temp = atoi(temp);
                    (states[found]) -> max_timestamp = timestamp/1000; //divide by 100 because ctime requires that
                }
                if((states[found]) -> min_temp > atoi(temp)){ // checking if we have new min
                    (states[found]) -> min_temp = atoi(temp);
                    (states[found]) -> min_timestamp = timestamp/1000;
                }
            }
        
        else{
            int starter = 0;
            while(states[starter] != NULL){
                starter++;
            }
            struct climate_info* ptr = malloc(sizeof(struct climate_info)); //temp ptr that initiates the malloc
            states[starter] = ptr;
            strcpy((states[starter]) -> code, temp);
        
            (states[starter]) -> num_records = 1; // Since this is new, this would be considered the 1st record
      
    
            temp = strtok(NULL, "\t"); // now its on timestamp
            (states[starter]) -> max_timestamp = atoi(temp)/1000;
            (states[starter]) -> min_timestamp = atoi(temp)/1000;
          
            temp = strtok(NULL, "\t"); // now its on geo-location
            temp = strtok(NULL, "\t"); // now its on humidity
            (states[starter]) -> sum_humidity = *temp;
            temp = strtok(NULL, "\t"); // now its on snow cover

            if(atoi(temp))
                (states[starter]) -> sum_snow = 1; // snow or no snow
            else
                (states[starter]) -> sum_snow = 0;
            temp = strtok(NULL, "\t"); // now its on cloud cover
            (states[starter]) -> sum_cloud = *temp;
            temp = strtok(NULL, "\t"); // now its on lightning strikes
            if(atoi(temp))
                (states[starter]) -> sum_lightning = 1;
            else
                (states[starter]) -> sum_lightning = 0;
            temp = strtok(NULL, "\t"); // now its on pressure
            temp = strtok(NULL, "\t"); // now its on temperature (KELVIN)
            (states[starter]) -> sum_temperature = atoi(temp);
            (states[starter]) -> max_temp = atoi(temp); //Since this is the only data we have so far, this would be both max and min
            (states[starter]) -> min_temp = atoi(temp);
        }
    }
}


void print_report(struct climate_info *states[], int num_states) {
    printf("States found: ");
    int i = 0;
    while (states[i] != NULL) {
        printf("%s ", states[i]->code);
        i++;
    }
    printf("\n");
    int a;
    for (a = 0; a < i; a++){
        printf("-- State: %s --\n", states[a]->code);
        printf("Number of Records: %lu\n", states[a] -> num_records);
        printf("Average Humidity: %0.1Lf%%\n", states[a] -> sum_humidity / (states[a] -> num_records));
        printf("Average Temperature: %0.1fF\n", (double)states[a] -> sum_temperature /(states[a] -> num_records) * 1.8 - 459.67); // divide by records to get average
        printf("Max Temperature: %0.1LfF on %s\n", states[a] -> max_temp* 1.8 - 459.67, ctime(&(states[a] -> max_timestamp)));//we make the kelvin to F switch here
        printf("Min Temperature: %0.1LfF on %s\n", states[a] -> min_temp* 1.8 - 459.67, ctime(&(states[a] -> min_timestamp)));
        printf("Lightning Strikes: %.0Lf\n", states[a] -> sum_lightning);
        printf("Records with Snow Cover: %.0Lf\n", states[a] -> sum_snow);
        printf("Average Cloud Cover: %.0Lf%%\n", (states[a] -> sum_cloud)/ (states[a] -> num_records)); // divide by records to get average
    }
    /* TODO: Print out the summary for each state. See format above. */
}
