1. In this assignment I suggested you use `fgets()` to get user input in the main while loop. Why is `fgets()` a good choice for this application?

    > **Answer**:  fgets allows for a specific maximum buffer size which can prevent buffer overflows. In our situation since we need to be able to read the | character as command seperation, fgets captures the entire line including the | character that other methods of reading the commands simply won't. Lastly fgets detects EOF which is cruical for regonizing when the program needs to exit. 

2. You needed to use `malloc()` to allocte memory for `cmd_buff` in `dsh_cli.c`. Can you explain why you needed to do that, instead of allocating a fixed-size array?

    > **Answer**:  Using malloc() for dynamic memory allocation is more flexible than declaring a fix sized arrray. The shell in this implementation can handle buffers more efficiently with dynamic memory allocation and this becomes specially more relevant when dealing with large buffer sizes. 


3. In `dshlib.c`, the function `build_cmd_list(`)` must trim leading and trailing spaces from each command before storing it. Why is this necessary? If we didn't trim spaces, what kind of issues might arise when executing commands in our shell?

    > **Answer**: To prevent incorrect commands from executing; for instance if the users types " command_1", the shell should still process command_1 even though the user accidently added a whitespace before the command. 

4. For this question you need to do some research on STDIN, STDOUT, and STDERR in Linux. We've learned this week that shells are "robust brokers of input and output". Google _"linux shell stdin stdout stderr explained"_ to get started.

- One topic you should have found information on is "redirection". Please provide at least 3 redirection examples that we should implement in our custom shell, and explain what challenges we might have implementing them.

    > **Answer**:  1 >: ls > output.txt
                   2 <: sort < data.txt
                   3 >>: echo "Hello" >> log.txt 

- You should have also learned about "pipes". Redirection and piping both involve controlling input and output in the shell, but they serve different purposes. Explain the key differences between redirection and piping.

    > **Answer**:  
                    1. Redirection is used to send input or output to or from files.
                    2. Piping connects the output of one command directly as input to another command.

- STDERR is often used for error messages, while STDOUT is for regular output. Why is it important to keep these separate in a shell?

    > **Answer**:  STDOUT can be used not only for error debugging but is a general output while STDERR spefically handles errors so it is important to keep these seperate. 

- How should our custom shell handle errors from commands that fail? Consider cases where a command outputs both STDOUT and STDERR. Should we provide a way to merge them, and if so, how?

    > **Answer**:  STDOUT should be the default output while error handling is strictly delegated to STDERR