1. Can you think of why we use `fork/execvp` instead of just calling `execvp` directly? What value do you think the `fork` provides?

    > **Answer**:  `fork` lets us create a separate process before running `execvp`, which is important because `execvp` replaces the current process with the new program. Calling `execvp` directly in the main shell process would result in the shell itself being replaced.
    
2. What happens if the fork() system call fails? How does your implementation handle this scenario?

    > **Answer**:  If `fork()` fails, it means the system couldn't create a new process. In my implementation, I check if `fork()` returns `-1`, and if it does, I print an error message using `perror("fork")` and return an error code to indicate failure.

3. How does execvp() find the command to execute? What system environment variable plays a role in this process?

    > **Answer**: `execvp()` searches for the command in the directories listed in the `PATH` environment variable. It checks each directory in `PATH` for an executable file with the given name.

4. What is the purpose of calling wait() in the parent process after forking? What would happen if we didn’t call it?

    > **Answer**: The `wait()` function makes the parent process wait for the child process to finish execution, preventing it from becoming a zombie process. If we don’t call `wait()`, the child process could finish execution, but its exit status wouldn’t be collected, leading to zombie processes piling, up wasting resources.

5. In the referenced demo code we used WEXITSTATUS(). What information does this provide, and why is it important?

    > **Answer**: `WEXITSTATUS()` extracts the exit status of a terminated child process from the status value returned by `wait()`. This is important because it allows the parent process to check if the child process executed successfully or encountered an error.

6. Describe how your implementation of build_cmd_buff() handles quoted arguments. Why is this necessary?

    > **Answer**: My `build_cmd_buff()` implementation detects quoted arguments by checking for double quotes (`"`). If an argument starts with a quote, it keeps reading until it finds the closing quote, treating everything inside as a single argument. This is necessary because spaces inside quotes should not be treated as argument separators, allowing commands like `echo "hello world"` to be parsed correctly.


7. What changes did you make to your parsing logic compared to the previous assignment? Were there any unexpected challenges in refactoring your old code?

    > **Answer**: I modified the parsing logic to properly handle quoted arguments and eliminate unnecessary spaces while preserving those inside quotes. I also ensured that `$?` was handled correctly for tracking exit statuses (extra but i thought it was fun). One unexpected challenge was making sure edge cases, like an empty quoted string (`""`), were correctly recognized and not discarded as empty input.


8. For this quesiton, you need to do some research on Linux signals. You can use [this google search](https://www.google.com/search?q=Linux+signals+overview+site%3Aman7.org+OR+site%3Alinux.die.net+OR+site%3Atldp.org&oq=Linux+signals+overview+site%3Aman7.org+OR+site%3Alinux.die.net+OR+site%3Atldp.org&gs_lcrp=EgZjaHJvbWUyBggAEEUYOdIBBzc2MGowajeoAgCwAgA&sourceid=chrome&ie=UTF-8) to get started.

- What is the purpose of signals in a Linux system, and how do they differ from other forms of interprocess communication (IPC)?

    > **Answer**: Signals in Linux are a way for the OS and processes to communicate asynchronously. Unlike other IPC methods like pipes or shared memory, signals are typically used for simple notifications rather than complex data exchange.

- Find and describe three commonly used signals (e.g., SIGKILL, SIGTERM, SIGINT). What are their typical use cases?

    > **Answer**:
    > - **SIGKILL (9)**: Immediately terminates a process and cannot be caught or ignored. It's used when a process is unresponsive or needs to be forcefully stopped.
    > - **SIGTERM (15)**: Politely asks a process to terminate, allowing it to clean up resources before exiting. Many daemons and applications handle this for graceful shutdowns.
    > - **SIGINT (2)**: Sent when a user presses `Ctrl+C` in the terminal, requesting the process to terminate. It can be caught by a program to allow cleanup before exiting.


- What happens when a process receives SIGSTOP? Can it be caught or ignored like SIGINT? Why or why not?

    > **Answer**: When a process receives **SIGSTOP (19)**, it is paused and cannot continue execution until it receives **SIGCONT (18)**. Unlike SIGINT, it **cannot** be caught or ignored because it's a non-maskable signal handled directly by the kernel. This makes it useful for debugging and job control in the shell.
