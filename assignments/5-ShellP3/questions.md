1. Your shell forks multiple child processes when executing piped commands. How does your implementation ensure that all child processes complete before the shell continues accepting user input? What would happen if you forgot to call waitpid() on all child processes?

_My shell uses waitpid() in a loop to wait for all child processes to finish before returning control to the user. Each child process in the pipeline runs its command, and the parent process waits for them in order. If I forgot to call waitpid(), the child processes would become zombie processes, since the parent wouldn't clean up their exit statuses. Over time, this could cause resource leaks and other in efficencies._

2. The dup2() function is used to redirect input and output file descriptors. Explain why it is necessary to close unused pipe ends after calling dup2(). What could go wrong if you leave pipes open?

_After calling dup2(), the original pipe file descriptors are no longer needed because the process now reads from or writes to standard in/out instead. Keeping unused pipe ends open can cause deadlocks, where a process waits indefinitely for input that never comes because the writing end is still open elsewhere. It can also lead to resource exhaustion, as too many open file descriptors can hit system limits and prevent new processes or pipes from being created._

3. Your shell recognizes built-in commands (cd, exit, dragon). Unlike external commands, built-in commands do not require execvp(). Why is cd implemented as a built-in rather than an external command? What challenges would arise if cd were implemented as an external process?

_cd is implemented as a built-in command because changing the working directory is a process-wide operation that needs to affect the shell itself. If cd were an external command, it would run in a separate child process, and any directory change would only apply to that child, not the parent shell._

4. Currently, your shell supports a fixed number of piped commands (CMD_MAX). How would you modify your implementation to allow an arbitrary number of piped commands while still handling memory allocation efficiently? What trade-offs would you need to consider?

_To support an arbitrary number of piped commands, I would replace the fixed-size array in command_list_t with a dynamically allocated array using malloc() and realloc(). As new commands are parsed, the array would expand as needed, similar to how dynamic arrays work in C. However, in this approach there would be a lot more complexity and potential need for overhead._
