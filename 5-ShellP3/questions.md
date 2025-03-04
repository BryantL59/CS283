1. Your shell forks multiple child processes when executing piped commands. How does your implementation ensure that all child processes complete before the shell continues accepting user input? What would happen if you forgot to call waitpid() on all child processes?

In the current implementation, the waitpid() function is called when attempting to fork and will not allow any new user input until all child input has successfully finished and exited. If I forgot to call waitpid(), a number of things could happen, including zombie child processes which are child processes that are not properly exited and takes up system resources. 

2. The dup2() function is used to redirect input and output file descriptors. Explain why it is necessary to close unused pipe ends after calling dup2(). What could go wrong if you leave pipes open?

Multiple file descriptors can refer to the same pipe and until all file descriptors are closed, all pipes assoicated with it are also not closed which leads to clogging of resources since there are a limited amount of descriptors availble. 

3. Your shell recognizes built-in commands (cd, exit, dragon). Unlike external commands, built-in commands do not require execvp(). Why is cd implemented as a built-in rather than an external command? What challenges would arise if cd were implemented as an external process?

CD is not called using exevcp() because the Cd command modifies the current working directory which is unqiue to each process. If cd were to be exported to an external process, the working directory of that process would be changed then exited however the directory in the main process remains unchanged which defeats the purpose of the cd command. 

4. Currently, your shell supports a fixed number of piped commands (CMD_MAX). How would you modify your implementation to allow an arbitrary number of piped commands while still handling memory allocation efficiently? What trade-offs would you need to consider?

the modification needed to allow an arbitarty number of pipes to be executed would be to use dyanmic memory management and the realloc()
function to give the cmd line enough memory to work with. The trade-off is the resource management will be significantly increased and since were allowing the buffer size to be changed during a cmd executed, the debugging process also becomes more difficult. 
