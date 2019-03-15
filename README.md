# Atmospheric-Data-Gatherer
Analyzing data from the National Oceanic and Atmospheric Administration (NOAA) North American Mesoscale Forecast System. Coded in C due to the speed department: I process millions of lines from text files and use that data to learn more about the climate in a few different states.

Climate.c takes a file takes the info on states from the file and makes a structure named climate_info that holds all the information provided for that state from the file.  At the end it will print the requested information which is: the number of records, average humidity in a state, average temperature, max temperature, min temperature, lightning strikes, Records with snow cover, and average cloud cover.
