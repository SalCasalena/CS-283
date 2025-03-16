1. How does the remote client determine when a command's output is fully received from the server, and what techniques can be used to handle partial reads or ensure complete message transmission?

_The remote client determines when a command’s output is fully received by checking for the special end-of-transmission character (0x04), which the server appends to responses. The client continues reading data in a loop using recv() until it detects this EOF marker._

2. This week's lecture on TCP explains that it is a reliable stream protocol rather than a message-oriented one. Since TCP does not preserve message boundaries, how should a networked shell protocol define and detect the beginning and end of a command sent over a TCP connection? What challenges arise if this is not handled correctly?

_A networked shell protocol should define and detect message boundaries by using explicit delimiters to indicate the start and end of a command. The client reads data in a loop until it detects this delimiter, ensuring that the entire message is received before processing it. If message boundaries are not properly handled in a networked shell, commands may be split across multiple recv() calls, merged incorrectly, or cause blocking reads, leading to incomplete execution and data corruption_

3. Describe the general differences between stateful and stateless protocols.

_Stateful protocols maintain session information between client and server across multiple requests, allowing continuity but requiring more resources. Stateless protocols treat each request independently, reducing memory overhead but requiring clients to resend necessary context._

4. Our lecture this week stated that UDP is "unreliable". If that is the case, why would we ever use it?

_UDP is used when low latency and speed are more critical than reliability, such as in real-time applications like video streaming, online gaming, and VoIP_

5. What interface/abstraction is provided by the operating system to enable applications to use network communications?

_The operating system provides sockets API as an abstraction for network communication. This interface allows applications to create, bind, listen, send, and receive data over network connections using system calls_