# cpputils
utils for cpp(c++11)

## simple log
example
```
logf("DEBUG", "debug %s,%d", str,num);
log("DEBUG")<<"debug "<<str<<","<<num<<std::endl;
```
## timer
```
timer t;
long e = t.elapsed();
```

## random generator
```
random_generator rg;
size_t rand = rg.next<size_t>(1,5);
```
