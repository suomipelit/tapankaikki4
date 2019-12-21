Tapan Kaikki 4: Bloodshed
=========================

![Tapan Kaikki 4: Bloodshed GIF](https://github.com/suomipelit/suomipelit.github.io/blob/master/pngs/tk4.png)

Tapan Kaikki 4: Bloodshed is a top view shooter game. The sole purpose of the game is to
kill everything that moves. 1-4 players.

This is a fork of the open-source release of Tapan Kaikki 4 (available [here](
https://sourceforge.net/projects/tapankaikki/)), with some minor bug fixes,
ported to Linux and Mac, and converted to use CMake as the build system.


Requirements
------------

SDL, SDL_image, SDL_mixer, SDL_net, libgtk (for Editor to work on Linux and Mac)


Build instructions for Linux and Mac
------------------------------------

    cmake .
    make
    make install

The `make install` command will install the game data files and executables
under your install prefix (`/usr/local` by default). You can customize the
install prefix with the following CMake invocation:

    cmake -DCMAKE_INSTALL_PREFIX=/path/to/install/to .

After all this you should be ready for the game: run `tk4`.


Build instructions for Windows
------------------------------

Set the environment variable `SDLDIR` to point to the directory where you
installed the SDL/SDL_image/SDL_mixer/SDL_net headers and libraries, for
example:

    setx SDLDIR C:\path\to\SDL-1.2.15

Open `src\tk.sln` in Visual Studio. Build. Copy SDL .dll files to build
directory if needed. You may also want to set `data` as the working
directory for launching the game directly from Visual Studio.
