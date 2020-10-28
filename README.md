# first_blood
cpptest

So far the only supported way to debug a program like this is to start a headless instance in one terminal:

dlv --headless debug yourprogram.go
This will print something like this:

API server listening at: 127.0.0.1:XYZ
then in another terminal do:

dlv connect :XYZ
Input for delve will go in the second terminal, input for your program will go in the first one.
