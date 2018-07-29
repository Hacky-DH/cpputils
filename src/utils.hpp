/**
* @brief cpp utils
*
* @author Hacky-DH
* @version 1.0.0
* @date 20180728
*/

#include <iostream>
#include <string>
#include <cstdio>
#include <cstdarg>
#include <ctime>
#include <cstring>
#include <fstream>
#include <chrono>
#include <random>

extern bool log_to_file;
extern std::string log_dir_prefix;
extern std::ofstream log_stream;

/**
* @brief logf
*   use C printf like format
*   depends on global log_to_file and log_dir_prefix
* @param level string level to log, DEBUG, INFO ...
* @param fmt ...  format arguments
*
* @example
* @code
*   logf("DEBUG", "debug %s,%d", str,num);
*/
inline void logf(const char *level, const char *fmt, ...){
    std::time_t t = std::time(nullptr);
    char buf[2048];
    std::strftime(buf, 100, "%Y-%m-%d %H:%M:%S", std::localtime(&t));
    std::sprintf(buf+strlen(buf)," [%s] ", level);
    va_list args;
    va_start(args, fmt);
    std::vsnprintf(buf+strlen(buf), 1920, fmt, args);
    va_end(args);
    if (log_to_file){
        std::string path = log_dir_prefix + "system.log";
        std::ofstream log_file(path, std::ios_base::out | std::ios_base::app );
        log_file << buf << std::endl;
    }else{
        std::cout << buf << std::endl;
    }
}

/**
* use by log, get ostreanm
* depends global log_to_file, log_dir_prefix and log_stream
*/
inline std::ostream& get_stream(){
    if (log_to_file){
        if(!log_stream.is_open()){
            std::string path = log_dir_prefix + "system.log";
            log_stream.open(path, std::ios_base::out | std::ios_base::app);
        }
        return log_stream;
    }
    return std::cout;
}

/**
* @brief log
*   use C++ stream
* @param level string level to log, DEBUG, INFO ...
*
* @example
* @code
*   log("DEBUG")<<"debug "<<str<<","<<num<<std::endl;
*/
inline std::ostream& log(const char *level){
    std::time_t t = std::time(nullptr);
    char buf[100];
    std::strftime(buf, 100, "%Y-%m-%d %H:%M:%S", std::localtime(&t));
    return get_stream()<<buf<<" ["<<level<<"] ";
}

/**
* @bref int random generator
* @example
* @code
*   random_generator rg;
*   size_t rand = rg.next<size_t>(1,5);
*/
class random_generator{
    std::default_random_engine generator;
public:
    random_generator(){
        auto seed = std::chrono::system_clock::now().time_since_epoch().count();
        generator.seed(seed);
    }
    template<typename T>
    T next(T lower,T upper){
        std::uniform_int_distribution<T> distribution(lower,upper);
        return distribution(generator);
    }
};

/**
* @bref timer
* @example
* @code
*   timer t;
*   long e = t.elapsed();
* sleep 1 sec
* std::this_thread::sleep_for(std::chrono::seconds(1));
*/
class timer{
    std::chrono::time_point<std::chrono::system_clock> start;
public:
    timer(){
         start = std::chrono::system_clock::now();
         //std::chrono::high_resolution_clock::now()
    }
    void reset(){
        start = std::chrono::system_clock::now();
    }
    /*
    * milliseconds
    */
    long elapsed(){
        std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
        auto elp = std::chrono::duration_cast<std::chrono::milliseconds>(end-start);
        return elp.count();
    }
    bool reach(std::chrono::seconds duration){
        return std::chrono::system_clock::now() - start >= duration;
    }
};

