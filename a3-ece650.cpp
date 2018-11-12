#include<vector>

#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <iostream>


/// Entry point of process C
int a3_input(void) {
    // Process C reading from both A and B
    while (!std::cin.eof()) {
        // read a line of input until EOL and store in a string
        std::string line;
        std::getline(std::cin, line);
        if (line.size () > 0)
            std::cout << line << std::endl;
    }
    return 0;
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
    std::vector<pid_t> kids;
    pid_t child_pid;    
    // create pipes
    int pipe_rgen_to_a1[2];
    pipe(pipe_rgen_to_a1);

    int pipe_a1_to_a2[2];
    pipe(pipe_a1_to_a2);
    
    //-----------------------------------
    // RUN RGEN PROGARM CONCURRENTLY    
    // ----------------------------------
    child_pid = fork();
    if (child_pid == 0) {
        // redirect stdout to the pipe
        dup2(pipe_rgen_to_a1[1], STDOUT_FILENO);
        close(pipe_rgen_to_a1[0]);
        close(pipe_rgen_to_a1[1]);
        execv("rgen", argv);
        perror ("Error: Could not execute rgen");
        return 1;
    } 
    else if (child_pid < 0) {
        std::cerr << "Error: could not fork Rgen" << std::endl;
        return 1;  
    }
    kids.push_back(child_pid);

    //-----------------------------------
    // RUN a1-ece650 PROGARM CONCURRENTLY   
    // ----------------------------------  
    child_pid = fork();
    if (child_pid == 0) {
        // redirect stdin from the pipe
        dup2(pipe_rgen_to_a1[0], STDIN_FILENO);
        close(pipe_rgen_to_a1[1]);
        close(pipe_rgen_to_a1[0]);
        // redirect stdout from the pipe
        dup2(pipe_a1_to_a2[1], STDOUT_FILENO);
        close(pipe_a1_to_a2[0]);
        close(pipe_a1_to_a2[1]);
        execv("a1-ece650.py", argv);
        perror ("Error: Could not execute a1-ece650.py");
        return 1;
    } 
    else if (child_pid < 0) {
        std::cerr << "Error: could not fork a1-ece.py" << std::endl;
        return 1;  
    }
    kids.push_back(child_pid);

    //-----------------------------------
    // RUN a2-ece650 PROGARM CONCURRENTLY   
    // ----------------------------------  
    child_pid = fork();
    if (child_pid == 0) {
        // redirect stdin from the pipe
        dup2(pipe_a1_to_a2[0], STDIN_FILENO);
        close(pipe_a1_to_a2[1]);
        close(pipe_a1_to_a2[0]);
        execv("a2-ece650", argv);
        perror ("Error: Could not execute a2-ece650");
        return 1;
    } 
    else if (child_pid < 0) {
        std::cerr << "Error: could not fork a2-ece" << std::endl;
        return 1;  
    }
    kids.push_back(child_pid);

    //-----------------------------------
    // RUN a3-ece650 input loop PROGARM CONCURRENTLY   
    // ----------------------------------  
    child_pid = fork();
    if (child_pid == 0) {
        // redirect stdout to the pipe
        dup2(pipe_a1_to_a2[1], STDOUT_FILENO);
        close(pipe_a1_to_a2[0]);
        close(pipe_a1_to_a2[1]);
        
        return a3_input();
    } 
    else if (child_pid < 0) {
        std::cerr << "Error: could not fork a3 loop" << std::endl;
        return 1;  
    }
    kids.push_back(child_pid);

    int monitor;
    wait(&monitor);

    // send kill signal to all children
    for (pid_t k : kids) {
        int status;
        kill (k, SIGTERM);
        waitpid(k, &status, 0);
        // std::cout << "process status: " << status << std::endl;
    }

    return 0;
}
