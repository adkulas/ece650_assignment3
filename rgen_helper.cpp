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
            exit(2);
        }
        urandom.close(); //close stream
    }
    else //Open failed
    {
        std::cerr << "Failed to open /dev/urandom" << std::endl;
        exit(2);
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

bool intersects(std::pair<int, int> point1, std::pair<int, int> point2, std::pair<int, int> point3, std::pair<int, int> point4) {
    int x1 = point1.first;
    int y1 = point1.second;
    int x2 = point2.first;
    int y2 = point2.second;
    int x3 = point3.first;
    int y3 = point3.second;
    int x4 = point4.first;
    int y4 = point4.second;
    float m1;
    float b1;
    float m2;
    float b2;

    int seg1_xmin = std::min(x1, x2);
    int seg1_xmax = std::max(x1, x2);
    int seg2_xmin = std::min(x3, x4);
    int seg2_xmax = std::max(x3, x4);
    int seg1_ymin = std::min(y1, y2);
    int seg1_ymax = std::max(y1, y2);
    int seg2_ymin = std::min(y3, y4);
    int seg2_ymax = std::max(y3, y4);
    int x_interval_max = std::max(seg1_xmin, seg2_xmin);
    int x_interval_min = std::min(seg1_xmax, seg2_xmax);
    int y_interval_max = std::max(seg1_ymin, seg2_ymin);
    int y_interval_min = std::min(seg1_ymax, seg2_ymax);

    // check for vertical overlapping lines
    if ( x1 == x2 && x2 == x3 && x3 == x4 ) {
        if ( (y_interval_min <= y1 && y1 <= y_interval_max) ||
             (y_interval_min <= y2 && y2 <= y_interval_max) ||
             (y_interval_min <= y3 && y3 <= y_interval_max) ||
             (y_interval_min <= y4 && y4 <= y_interval_max) )  {
            return true;
        }
    } else if ( x1 != x2 && x3 != x4) {    
        m1 = (y2-y1)/(x2-x1);
        b1 = y1-m1*x1;
        m2 = (y4-y3)/(x4-x3);
        b2 = y3-m2*x3;
        // check if line equations are equal
        if (m1 == m2 && b1 == b2) {
            if ( 
             ( (x_interval_min <= x1 && x1 <= x_interval_max) ||
             (x_interval_min <= x2 && x2 <= x_interval_max) ||
             (x_interval_min <= x3 && x3 <= x_interval_max) ||
             (x_interval_min <= x4 && x4 <= x_interval_max) ) 
             &&
             ( (y_interval_min <= y1 && y1 <= y_interval_max) ||
             (y_interval_min <= y2 && y2 <= y_interval_max) ||
             (y_interval_min <= y3 && y3 <= y_interval_max) ||
             (y_interval_min <= y4 && y4 <= y_interval_max) )
                ) 
            {
            return true;
            }
        }
    }

    float xnum = ((x1*y2-y1*x2)*(x3-x4) - (x1-x2)*(x3*y4-y3*x4));
    float xden = ((x1-x2)*(y3-y4) - (y1-y2)*(x3-x4));

    float ynum = (x1*y2 - y1*x2)*(y3-y4) - (y1-y2)*(x3*y4-y3*x4);
    float yden = (x1-x2)*(y3-y4) - (y1-y2)*(x3-x4);
    if (xden == 0 || yden == 0) {
        return false;
    }

    float xcoor = xnum / xden;
    float ycoor = ynum / yden;
    if ( x_interval_min <= xcoor && xcoor <= x_interval_max &&
        y_interval_min <= ycoor && ycoor <= y_interval_max ) {
            return true;
        }
    return false;
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

    int attempts = 0;
    do {
        street.clear();
        for(int i = 0; i < num_segments + 1; i++ ) {
            std::pair<int, int> point;
            point.first = random_int(-coord_range, coord_range);
            point.second = random_int(-coord_range, coord_range);
            street.push_back(point);
        } 
        attempts++;
        if (attempts > 25) {
            std::cerr << "Error: failed to generate valid input for " << attempts << " simultaneous attempts" 
                      << std::endl;
        }     
    } while (not check_valid_input(street));
    return street;
}

void Rand_graph::generate_graph() {
    int num_streets = random_int(2, sint_value);    

    for(int i=0; i <  num_streets; i++) {
        std::string word = random_word();
        //check if random word already exists and regenerate until unique
        while( std::find(street_names.begin(), street_names.end(), word) != street_names.end() ) {
            word = random_word();
        }
        street_names.push_back(word);
        street_points.push_back(generate_rand_street_segments());
    }
}

bool Rand_graph::check_valid_input(std::vector< std::pair<int,int> > street) {
    for (int i=0; i < street.size(); i++) {
        for (int j=i+1; j < street.size(); j++) {
            // Check for duplicate points
            if( (street[i].first == street[j].first) && 
                (street[i].second == street[j].second) )
            {
                return false;
            }
            // check if any two segments overlap or intersect
            if (i+1 < street.size() && j+1 < street.size()) {
                if(intersects(street[i], street[i+1], street[j], street[j+1])) {
                    return false;
                }   
            }
        }
    }
    return true;
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