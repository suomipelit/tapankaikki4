Tapan Kaikki
============

Requirements
------------

SDL, SDL_image, SDL_mixer, SDL_net, libgtk (for Editor to work on Linux)


Build instructions for Linux
----------------------------

    ./waf configure
    ./waf build
    ./waf install

The `./waf install` command will install files under the `bin` directory of
current working directory. 

After all this you should be ready for the game.


Build instructions for Windows
------------------------------

Set the environment variable `SDLDIR` to point to the directory where you
installed the SDL headers and libraries, for example:

    setx SDLDIR C:\path\to\SDL-1.2.15

Open `src\tk.sln` in Visual Studio. Build. Copy SDL .dll files to build
directory if needed. You may also want to set `data` as the working
directory for launching the game directly from Visual Studio.
