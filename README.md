## wdparse - read Weather Display log files files and present summary data.
This console utility reads the log files from the Weather Display program,
and displays summaries for the data.  User can either get one summary for
all existing data, or get separate summaries for each year of log files.

```
Usage: wdparse [filter_by_year] [filter_by_month]
-h = show usage screen
-yYYYY = show max/min stats for given year
-mMM = Show max/min stats for given month over all years

Month and Year values may be combined; thus:
-m8 -y2023 will show stats for August 2023.

If neither year nor month are specified,
data from all years will be collected.
```

Default path is currently hard-coded to c:\WeatherDisplay\logfiles 

Sample output from default scan:
```
> wdparse
show data for all years
filespec: C:\WeatherDisplay\logfiles\*lg.txt, 125 found
  
09/02/2017, 15:24  Max Temp:         110.6
12/27/2015, 07:27  Min Temp:          28.5
01/19/2021, 06:45  Max Wind Average:  19.8
03/14/2023, 13:01  Max Wind Gust:     42.0
12/31/2022, 23:34  Max Daily Rain:     3.90
06/06/2023, 10:49  Yearly Rain:       31.80
```
See wdparse.out in this repository, for an example of breaking out
results by year.

View [summaries by year](wdparse.out) here  
Download [wdparse](https://github.com/DerellLicht/wdparse/releases/tag/v1.00) here

***

This project is licensed under _Creative Commons CC0 1.0 Universal_   

See the [License](LICENSE.txt) file for detailed information about this license
***

See [Changelog](CHANGELOG.md) for the full revision history.  
  
