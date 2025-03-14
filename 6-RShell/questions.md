1. How does the remote client determine when a command's output is fully received from the server, and what techniques can be used to handle partial reads or ensure complete message transmission?

In a RSH setup, the delimter is used to mark EOF or when a full command is read. In our current implementaiton the delimiter is EOF. To handle partial read, we use the recv() function, in this way we send over 4 bytes at a time until the entire command is read. 

2. This week's lecture on TCP explains that it is a reliable stream protocol rather than a message-oriented one. Since TCP does not preserve message boundaries, how should a networked shell protocol define and detect the beginning and end of a command sent over a TCP connection? What challenges arise if this is not handled correctly?

A networked shell protocol marks the start of the command using a special character or by using the length of the command. Without doing these steps, commands will not be displayed properly because TCP does not properly split strings. 

3. Describe the general differences between stateful and stateless protocols.

Stateful protocol keeps track of previous sessions and uses the information of those previous sessions for future sessions. Stateless protcol makes every command its own independant state. 

4. Our lecture this week stated that UDP is "unreliable". If that is the case, why would we ever use it?

UDP while being unreliable is generally faster because it does not handle errors. UDP is used because of its speed. 

5. What interface/abstraction is provided by the operating system to enable applications to use network communications?

Generally a operating systems provides a API to allow communcation endpoint that allows an application to send or receive data over a network. A socket is generally used to manage these endpoints. Then the operating system a network protocol such as TCP that sets the rules for communcation between networks. 