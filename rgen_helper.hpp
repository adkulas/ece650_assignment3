#pragma once

#include <iostream>
#include <vector>

template<class T>
T read_urandom();

int random_int(int min, int max);

char random_letter();

std::string random_word(int length);

std::vector< std::pair<int,int> > generate_rand_street_segments(int nint_value, int coord_range);

std::string create_random_a_command(int sint_value, int nint_value, int cint_value);

class Rand_graph {
private:
    std::vector< std::vector< std::pair<int,int> > > street_points;
    std::vector< std::string > street_names;
    int sint_value, nint_value, cint_value;

    std::vector< std::pair<int,int> > generate_rand_street_segments();
    bool check_valid_input(std::vector< std::pair<int,int> > edges);
    void generate_graph();

public:
    Rand_graph( int sint_value, int nint_value, int cint_value);

    /// Accessors
    void print_add_graph();
    void print_remove_graph();
    
    /// Mutators

};