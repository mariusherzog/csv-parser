# csv

simple class used to parse a csv file. 


## Usage:
Instantiate the template class with the data types for the respective columns of the csv file.
The constructor takes 3 arguments, the first being the filepath, the second the column delimiter and the third denotes 
whether a header is used. The delimiter and header are default comma and false respectively.  
Using the template specialization std::string (csv&lt;std::string&gt; c("a.csv");) will return raw rows regardless
the number of columns.

###Example:
csv-file:  
1.3, Q  
2.1, R  

then:
csv&lt;double, char&gt; c("csvf.csv");

also refer to the main.cpp.
