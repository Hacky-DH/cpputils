#include "utils.hpp"

bool log_to_file = false;
std::string log_dir_prefix;
std::ofstream log_stream;

int main(int /*argc*/, char** /*argv*/){
    random_generator rg;
    timer t;
    for(int i=0; i<10; ++i){
        size_t rand = rg.next<size_t>(1,100);
        log("INFO")<<"rand "<<rand<<std::endl;
    }
    logf("DEBUG","used %lld ms",t.elapsed());
    log("INFO")<<"used "<<t.elapsed()<<" ms"<<std::endl;
}
