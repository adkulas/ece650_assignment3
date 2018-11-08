// g++ ../../../rgen.cpp -o rgen -std=c++11

#include <unistd.h>
#include <iostream>
#include <fstream>
#include <vector>

template<class T>
T read_urandom() {
    T random_value; //Declare value to store data into
    size_t size = sizeof(random_value); //Declare size of data
    std::ifstream urandom("/dev/urandom", std::ios::in|std::ios::binary); //Open stream
    if(urandom) //Check if stream is open
    {
        urandom.read(reinterpret_cast<char*>(&random_value), size); //Read from urandom
        if(!urandom) //Read Failed
        {
            std::cerr << "Failed to read from /dev/urandom" << std::endl;
        }
        urandom.close(); //close stream
    }
    else //Open failed
    {
        std::cerr << "Failed to open /dev/urandom" << std::endl;
    }
    return random_value;
}

// Return a random integer N such that min <= N <= max.
int random_int(int min, int max) {
    int range = max-min;
    int i = read_urandom<int>();
    int result = (i % (range+1) + (range+1) ) % (range+1) + min;

    return result;
}

char random_letter() {
    char rand_char;
    int i;
    i = (read_urandom<int>() % 26 + 26) % 26;   
    if (random_int(0,1)) {
        rand_char = 'A' + i;    
    } else {
        rand_char = 'a' + i;
    }
    
    return rand_char;
}

std::string random_word(int length=10) {
    std::string result;
    for(int i=0; i < length; i++) {
        result += random_letter();
    }
    return result;
}

std::vector< std::pair<int,int> > generate_rand_street_segments(int nint_value, int coord_range) {
    std::vector< std::pair<int,int> > street;
    int num_segments = random_int(1, nint_value);

    for(int i = 0; i < num_segments + 1; i++ ) {
        std::pair<int, int> point;
        point.first = random_int(-coord_range, coord_range);
        point.second = random_int(-coord_range, coord_range);
        street.push_back(point);
        std::cout << "(" << point.first << "," << point.second << ")";
    }
    std::cout << std::endl;
    //call to check street for error
        //add here
    return street;
}

std::string create_random_a_command(int sint_value, int nint_value, int cint_value) {
    std::string result;
    int num_streets = random_int(2, sint_value);
    std::string street_name;
    std::vector< std::pair<int,int> > street_points;
    
    street_name = random_word();
    street_points = generate_rand_street_segments(nint_value, cint_value);

    result = "a \"" + street_name + "\" ";
    for( auto& p : street_points) {
        result = result + "(" + std::to_string(p.first) + "," + std::to_string(p.second) + ")";
    }
    result += "\n";
    return result;

}


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
    std::cerr << "[rgen]" << "s=" << sint_value << "n=" << nint_value << "l=" << lint_value << "c=" << cint_value << std::endl;

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