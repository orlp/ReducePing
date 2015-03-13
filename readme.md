What is ReducePing?
-------------------
ReducePing is a small utility to tune the "TcpAckFrequency" setting of Windows,
which can drastically improve latency for some applications (mainly games) at
the cost of slightly less throughput.

What is TcpAckFrequency exactly?
--------------------------------
Before I can explain this I must explain a bit about TCP. A lot of the internet
works with TCP, which is a protocol which describes a reliable data transfer
method. On the internet data gets sent in "packets", little chunks of data. In
TCP every packet gets "acknowledged", which means that if person A sends person
B a packet person B will send a packet back that they recieved the packet.

By default Windows doesn't acknowledge immediately when it recieves a packet,
instead it "saves up" a few acknowledgements and sends them all at once. This
saves bandwidth because less data has to be sent, but it increases latency.

ReducePing modifies this setting, called TcpAckFrequency such that packets
immediately get acknowledged. This means that a bit more bandwidth is used, but
that latency can be greatly improved.

How do I use this?
------------------
It's really simple, you start up ReducePing.exe and you click "Enable" to
enable ReducePing. If you want to remove ReducePing for any reason you start up
ReducePing.exe again and click "Disable". All ReducePing does is change a
Windows setting, it doesn't install anything on your computer (not even a
deinstaller, if you want to remove ReducePing and you have lost ReducePing.exe
you just have to redownload it). However, ReducePing has to write this setting
in the Windows registry so it may require Administrator rights to do it's job.

For the real geeks out there ReducePing.exe also supports command line
arguments. There are two accepted arguments, "enable" and "disable" doing the
same thing as the UI buttons. No GUI is started if the command line arguments
are invoked.

Does this support versions of Windows older than XP?
----------------------------------------------------
No, because the TcpAcknowledgeFrequency settings was added to Windows in
Windows XP.

Is a version for <insert non-Windows OS here> available?
--------------------------------------------------------
No, because this optimization only applies to Windows, so a version for an other
OS would be silly.

Is the source code available?
-----------------------------
Yes it is, it can be found on Github: http://github.com/orlp/ReducePing
