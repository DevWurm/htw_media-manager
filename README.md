# Media Manager
Media Manager is a simple C tool for keeping track of borrowed media.

It stores the title, the artist and the person to which the medium is borrowed to
and provides an simple graphical user interface for creating, showing and deleting
records.

The tool stores the information to the file `.media-manager.csv` in the users home
directory.

## Dependencies
Media Manager uses [xforms](http://xforms-toolkit.org/) (a.k.a. libforms) as GUI
Toolkit. Therefore xforms has to be installed on your system.

#### Note
Becasuse of special requirements the build system of Media Manager also works if
your installation of xforms is located at `/opt/xforms` instead of the common
include and library paths. In this case you have to modify your environment via
```sh
# 32-bit system
export LD_LIBRARY_PATH=/opt/xforms/lib

# 64-bit system
export LD_LIBRARY_PATH=/opt/xforms/lib64
```
to run the executable correctly.

## Build
Media Manager uses GNU Make as Build system

To build the programm run
```sh
make clean && make
```

To build a debug version run
```sh
make clean && make debug
```

This generates the executable `media-manager`.

## Command Line Iterface
Media Manager also brings a very basic command line interface for your media store.
The CLI is **NOT FEATURE COMPLETE** to the graphical user interface, because it doesn't
support deleting and sorting of the available information.

The CLI can be started via
```sh
media-manager -c
```

## License
`htw_media-manager` is offered under MIT License (Read LICENSE).<br>
Copyright 2017 Leo Lindhorst

## Contact
If you have any questions, ideas, etc. feel free to contact me:<br>
Leo Lindhorst<br>
Email: <a href='mailto:devwurm@devwurm.net'>devwurm@devwurm.net</a><br>
Jabber: devwurm@jabber.ccc.de<br>
Twitter: @DevWurm<br>
