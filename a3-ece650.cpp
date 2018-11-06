#include<vector>

#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include<iostream>

int main (int argc, char **argv) {
    std::cout << "Replace with your solution for the main "
              << "driver program of Assignment 3\n";
    
    std::vector<pid_t> kids;
    
    // create a pipe
    int connection[2];
    pipe(connection);
    
    pid_t child_pid;
    //-----------------------------------
    // RUN RGEN PROGARM CONCURRENTLY    
    // ----------------------------------
    std::cout << "INFO: Starting RGEN Program" << std::endl;
    child_pid = fork();
    if (child_pid == 0) {
        sleep(3);
        // redirect stdout to the pipe
        dup2(connection[1], STDOUT_FILENO);
        close(connection[0]);
        close(connection[1]);
        execv("rgen", argv);
        return 0;
    } 
    else if (child_pid < 0) {
        std::cerr << "Error: could not fork Rgen" << std::endl;
        return 1;  
    }
    kids.push_back(child_pid);

    //-----------------------------------
    // RUN a1-ece650 PROGARM CONCURRENTLY   
    // ---------------------------------- 
    std::cout << "INFO: Starting python Program" << std::endl;
    child_pid = fork();
    if (child_pid == 0) {
        // redirect stdin from the pipe
        dup2(connection[0], STDIN_FILENO);
        close(connection[1]);
        close(connection[0]);
        execv("a1-ece650.py", argv);
        return 0;
    } 
    else if (child_pid < 0) {
        std::cerr << "Error: could not fork A1" << std::endl;
        return 1;  
    }
    kids.push_back(child_pid);

    // send kill signal to all children
    for (pid_t k : kids) {
        int status;
        kill (k, SIGTERM);
        waitpid(k, &status, 0);
    }

    return 0;
}
