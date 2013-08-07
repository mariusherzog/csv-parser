# csv

simple class used to parse a csv file. 


## Usage:
Instantiate the template class with the data types for the respective columns of the csv file.
The constructor takes 3 arguments, the first being the filepath, the second the column delimiter and the third denotes 
whether a header is used. The delimiter and header are default comma and false respectively.  

readline() will return a tuple with the respective data types, while raw_readline will return a raw string.  
move_to(unsigned int l) moves the "linepointer" to the line l.  
has_dataset() will return true if there is at least one available line to read, otherwise false.


###Example:
csv-file:  
```
1.3, Q  
2.1, R  
```

then:
```cpp
csv<double, char> c("csvf.csv");
```

also refer to the main.cpp for examples.
