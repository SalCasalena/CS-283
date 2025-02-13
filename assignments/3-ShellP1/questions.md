1. In this assignment I suggested you use `fgets()` to get user input in the main while loop. Why is `fgets()` a good choice for this application?

    > **Answer**:  fgets() is a good choice for this shell because it stops buffer overflows by limiting how much input gets read, unlike gets() which is risky. It also keeps spaces in commands intact, making sure args don’t get messed up, handling EOF properly so the shell can exit cleanly when needed.

2. You needed to use `malloc()` to allocte memory for `cmd_buff` in `dsh_cli.c`. Can you explain why you needed to do that, instead of allocating a fixed-size array?

    > **Answer**:  You need to use malloc() for cmd_buff instead of a fixed-size array because command input length can vary, and dynamically allocating memory makes the shell more flexible. If you use a fixed-size array, you either waste memory if it's too big or risk cutting off input if it's too small


3. In `dshlib.c`, the function `build_cmd_list(`)` must trim leading and trailing spaces from each command before storing it. Why is this necessary? If we didn't trim spaces, what kind of issues might arise when executing commands in our shell?

    > **Answer**:  If we didn’t trim, the shell might try to run a command with a space in front of it, which could make it fail to execute. Also, arguments could have unwanted spaces, causing mismatches or empty args.

4. For this question you need to do some research on STDIN, STDOUT, and STDERR in Linux. We've learned this week that shells are "robust brokers of input and output". Google _"linux shell stdin stdout stderr explained"_ to get started.

- One topic you should have found information on is "redirection". Please provide at least 3 redirection examples that we should implement in our custom shell, and explain what challenges we might have implementing them.

    > **Answer**:
    > (1) Redirect Standard Output to a File (>)
    > (2) Redirect Standard Input from a File (<)
    > (3) Redirect Standard Error to Standard Output (2>&1)
    > Challenges: Parsing the command correctly to identify redirection operator and handling file descriptors properly to ensure data is redirected correctly.

- You should have also learned about "pipes". Redirection and piping both involve controlling input and output in the shell, but they serve different purposes. Explain the key differences between redirection and piping.

    > **Answer**: Redirection (>, <, >>) is used to send output to a file or read input from a file, while piping (|) connects the output of one command directly to the input of another

- STDERR is often used for error messages, while STDOUT is for regular output. Why is it important to keep these separate in a shell?

    > **Answer**:  It allows error messages to be handled differently from regular output. This separation makes it easier to debug issues since errors won’t get mixed with normal command results. For example, when running ls non_existent_file > output.txt, the error message still appears in the terminal instead of being saved to the file.

- How should our custom shell handle errors from commands that fail? Consider cases where a command outputs both STDOUT and STDERR. Should we provide a way to merge them, and if so, how?

    > **Answer**:  Our custom shell should detect when a command fails and display an appropriate error message to the user. If a command produces both STDOUT and STDERR, we should allow users to handle them separately or merge them using redirection