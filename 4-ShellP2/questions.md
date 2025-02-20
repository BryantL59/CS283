1. Can you think of why we use `fork/execvp` instead of just calling `execvp` directly? What value do you think the `fork` provides?

    > **Answer**: fork creates a child process that allows execvp to execute and still alow the parent to retain control over the 
    process. If fork is not used, the main process if replaced and other features and error handling would also break. 

2. What happens if the fork() system call fails? How does your implementation handle this scenario?

    > **Answer**:  if fork() fails, the child process is not created and it returns a error code. My program will terminate the child process and passes along a erorr code. 

3. How does execvp() find the command to execute? What system environment variable plays a role in this process?

    > **Answer**: execvp() searches for the command in the directories listed in the PATH environment variable.

4. What is the purpose of calling wait() in the parent process after forking? What would happen if we didn’t call it?

    > **Answer**:  The purpose of calling wait is to wait for the child process to terminate and collect its exit code.

5. In the referenced demo code we used WEXITSTATUS(). What information does this provide, and why is it important?

    > **Answer**:  WEXISTSTATUS() extracts the exit status to check if the child failed or succeeded. This is important in error handling to figure out how the child terminated. 

6. Describe how your implementation of build_cmd_buff() handles quoted arguments. Why is this necessary?

    > **Answer**: I manually parsed it and checked for "''" and space characters and skipped them in my command handling furthermore once we enter a quoted text, it no longer checks for whitespaces. I used this methood because with other methods such as strtok(), the white spaces within a quoted segment would be trimmed. 

7. What changes did you make to your parsing logic compared to the previous assignment? Were there any unexpected challenges in refactoring your old code?

    > **Answer**:  My previous parsing logic utilized strchr to find the first instance of a pipe however this week it was not required as we were not handling pipes. in addition last week i also used strchr in order to idenify the first space character while this week I utilized a more direct form of parsing. 

8. For this quesiton, you need to do some research on Linux signals. You can use [this google search](https://www.google.com/search?q=Linux+signals+overview+site%3Aman7.org+OR+site%3Alinux.die.net+OR+site%3Atldp.org&oq=Linux+signals+overview+site%3Aman7.org+OR+site%3Alinux.die.net+OR+site%3Atldp.org&gs_lcrp=EgZjaHJvbWUyBggAEEUYOdIBBzc2MGowajeoAgCwAgA&sourceid=chrome&ie=UTF-8) to get started.

- What is the purpose of signals in a Linux system, and how do they differ from other forms of interprocess communication (IPC)?

    > **Answer**:  in Linux signaling is used to communicate between multiple processes. specifcally signaling is used to serve as a mechanism to interrupt a process without direct interaction between the two processes. 

- Find and describe three commonly used signals (e.g., SIGKILL, SIGTERM, SIGINT). What are their typical use cases?

    > **Answer**:  
    
    SIGKILL signal 9: this signal is used to forcefully terminate a process, this signal cannot be blocked or ignored.

    SIGTERM Signal 15: he default signal sent by commands like kill to terminate a process. unlike sigkill the process that is signaled to terminate can still execute nessecary actions before being terminated like freeing up resources.

    SIGINT Signal 2: : this signal is for the user to interrupt a process by pressing Ctrl + C in the terminal. It is intended to signal that the process should terminate

- What happens when a process receives SIGSTOP? Can it be caught or ignored like SIGINT? Why or why not?

    > **Answer**:  when a process recieves sigstop, it is paused by the operating system. the process remains in a stopped state until prompted to execute again. This process cannot be caught or ignored because, this signal acts as a immediate stop to the current process.
