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