<h1 style="text-align: center;">x-mouse</h1>

This a very simple CLI program to control the mouse behaviour on Windows.

I was searching for a method to make the mouse on Windows to mimic the
focus follow mouse behaviour in X11. And found this program
[X-Mouse Controls](https://github.com/joelpurra/xmouse-controls), but when
I tried to run it, it requires to bring in the .NET dependency.

For such a simple task, I would rather not to bring in a whole .NET runtime.
So I serched around and found that it is very simple by calling a
single win32 function `SystemParametersInfoW` with various parameters to
get the job done.

Hence born this simple program. It is developed under
[MSYS2](https://www.msys2.org/)
enviornment and compiled with make and gcc. It is a single/simple executable
that does not need to be installed. It could be run in the `CMD` shell or
`PowerShell`.

The program by default prints out the current settings when run without
argument:
```
   ./x-mouse.exe
   Window tracking: on
   Window raising : off
   Tracking Delay : 50ms
```
Run it with "x-mouse -h" will print out the usage:
```
   ./x-mouse.exe -h
   Usage: x-mouse [arguments]
      --win-tracking on/off   Set Windows tracking on/off.
      --win-raising  on/off   Set Windows raising on/off.
      --delay        NUM      Set Windows tracking delay (0~3000ms).

   -h/?,--help                This help.
```
