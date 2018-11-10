// g++ ../../../rgen.cpp -o rgen -std=c++11

#include <unistd.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include "rgen_helper.hpp"

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

// Constructor
Rand_graph::Rand_graph(int sint_value, int nint_value, int cint_value):
    sint_value(sint_value), nint_value(nint_value), cint_value(cint_value) {
        generate_graph();
    }

//Private Functions
std::vector< std::pair<int,int> > Rand_graph::generate_rand_street_segments() {
    std::vector< std::pair<int,int> > street;
    int num_segments = random_int(1, nint_value);
    int coord_range = cint_value;

    for(int i = 0; i < num_segments + 1; i++ ) {
        std::pair<int, int> point;
        point.first = random_int(-coord_range, coord_range);
        point.second = random_int(-coord_range, coord_range);
        street.push_back(point);
        // std::cout << "(" << point.first << "," << point.second << ")";
    }
    // std::cout << std::endl;
    //call to check street for error
        //add here
    return street;
}


void Rand_graph::generate_graph() {
    int num_streets = random_int(2, sint_value);    

    for(int i=0; i <  num_streets+1; i++) {
        std::string word = random_word();
        //check if random word already exists and regenerate until unique
        while( std::find(street_names.begin(), street_names.end(), word) != street_names.end() ) {
            word = random_word();
        }
        street_names.push_back(word);
        street_points.push_back(generate_rand_street_segments());
    }
}

//Accessors
void Rand_graph::print_add_graph() {
    std::string tmp;
    for (int i=0; i < street_names.size(); i++) {
        
        tmp = tmp + "a \"" + street_names[i] + "\" ";
        for (auto& p : street_points[i]) {
            tmp = tmp + "(" + std::to_string(p.first) + "," + std::to_string(p.second) + ")";
        }
        tmp += "\n";
    }
    tmp += "g";
    std::cout << tmp << std::endl;
    // std::cerr << tmp << std::endl;
}

void Rand_graph::print_remove_graph() {
    std::string tmp;
    for (auto& s_name : street_names) {
        tmp = tmp + "r \"" + s_name + "\" ";
        tmp += "\n";
        std::cout << "r \"" << s_name << "\"" << std::endl;
    }
}