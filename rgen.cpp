// g++ -o rgen ../../../rgen.cpp ../../../rgen_helper.cpp -std=c++11

#include <unistd.h>
#include <iostream>
#include <fstream>
#include <vector>
#include "rgen_helper.hpp"

int main (int argc, char **argv) {
    std::string svalue;
    int sint_value = 10; // num of streets randint [2,k] k >=2
    std::string nvalue; 
    int nint_value = 5; //num of line segs per street rand int [1,k] k >= 1
    std::string lvalue;
    int lint_value = 5; //wait rand time rand int [5,k] k>=5
    std::string cvalue;
    int cint_value = 20; //xy coord range rand int [-k,k] k>=1
    int c;

    opterr = 0;

    // expected options are '-s value', '-n value', '-l value', and '-c value'
    while ((c = getopt (argc, argv, "s:n:l:c:?")) != -1)
        switch (c)
        {
        case 's':
            svalue = optarg;
            sint_value = atoi(svalue.c_str());
            if(sint_value < 2) {
                std::cerr << "Error: Option -s must have value >= 2" << std::endl;
                return 1;
            }
            break;

        case 'n':
            nvalue = optarg;
            nint_value = atoi(nvalue.c_str());
            if(nint_value < 1) {
                std::cerr << "Error: Option -n must have value >= 1" << std::endl;
                return 1;
            }
            break;

        case 'l':
            lvalue = optarg;
            lint_value = atoi(lvalue.c_str());
            if(lint_value < 5) {
                std::cerr << "Error: Option -l must have value >= 5" << std::endl;
                return 1;
            }
            break;

        case 'c':
            cvalue = optarg;
            cint_value = atoi(cvalue.c_str());
            if(cint_value < 1) {
                std::cerr << "Error: Option -c must have value >= 1" << std::endl;
                return 1;
            }
            break;
        
        case '?':
            std::cerr << "Error: unknown option: " << optopt << std::endl;
            return 1;
        default:
            return 0;
        }
    std::cerr << "[rgen_args]" << "s=" << sint_value << "n=" << nint_value << "l=" << lint_value << "c=" << cint_value << std::endl;

    for(int i=0; i<40; i++) {
        // std::cout << "generate letter: " << generate_random_char() << std::endl;
        // random_letter();
        // char tmp = 'A' + i;
        // std::cout << tmp << std::endl;
        // std::cout << random_word() << std::endl;
        // std::cout << random_int(50,60) << std::endl;
        // std::cout << random_int(-20,20) << std::endl;
        // generate_rand_street_segments(nint_value, cint_value);
        std::cout << create_random_a_command(sint_value, nint_value, cint_value) << std::endl;
    } 

    // sleep(5);
    // std::cout << "a \"Weber Street\" (2,-1) (2,2) (5,5) (5,6) (3,8)\n"
    //         << "a \"King Street S\" (4,2) (4,8)\n"
    //         << "a \"Davenport Road\" (1,4) (5,8)\n"
    //         << "g\n";

    return 0;
}