## wdparse - read Weather Display data files and collect data
This application is copyright (c) 2023  Derell Licht  
This program, and its source code, are distributed as unrestricted freeware.
You can use them for any purpose, personal or commercial, in whole or in part,
for any purpose that you wish, without contacting me further.

This console utility reads the log files from the Weather Display program,
and displays summaries for the data.  User can either get one summary for
all existing data, or get separate summaries for each year of log files.

```
Usage: wdparse year_to_select
-h = show usage screen
-yYYYY = year to select; display data only for selected year
If year_to_select is not specified, data from all years will be collected.
```

Default path is currently hard-coded to c:\WeatherDisplay\logfiles 

Sample output from default scan:
```
> wdparse
show data for all years
filespec: C:\WeatherDisplay\logfiles\*lg.txt, 125 found
[\]
09/02/2017, 15:24  Max Temp:         110.6
12/27/2015, 07:27  Min Temp:          28.5
01/19/2021, 06:45  Max Wind Average:  19.8
03/14/2023, 13:01  Max Wind Gust:     42.0
12/31/2022, 23:34  Max Daily Rain:     3.90
```
See wdparse.out in this repository, for an example of breaking out
results by year.

Download [executable](http://derelllicht.com/files/wdparse.zip) here

