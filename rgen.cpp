#include <unistd.h>
#include <iostream>

int main (int argc, char **argv) {
    
    for (int i=0; i < 1; i++) {
        sleep(5);
        std::cout << "a \"Weber Street\" (2,-1) (2,2) (5,5) (5,6) (3,8)\n"
                << "a \"King Street S\" (4,2) (4,8)\n"
                << "a \"Davenport Road\" (1,4) (5,8)\n"
                << "g\n";
    }
    return 0;
}