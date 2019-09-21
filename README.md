C++ Resource Includer
=================================

Description
---------------------------------

This program may be useful when you want to include
files content inside a binary file. All you need is to
specify files that you want to include. As the result,
program will generate C++ source and header file.

How to use it?
---------------------------------

Suppose your files located under your working
directory. Execute this command to include files
`files/foo.txt` and `bar.txt`.
bar.txt
```
cpp-res files/foo.txt bar.txt
```
After that you will see files `resources.cpp` and 
`resources.hpp` where `resources.cpp` contains all
data as byte array (std::array<std::uint8_t, N> class).

In common case your files may be in different folder
that is not under your working directory. If *cpp-res*
can't find specified files there, it will omitthem.
You can specify base directory to deal with such 
situations.
```sh
BASE=/usr/local/src/resources
cpp-res -path $BASE $BASE/files/foo.txt $BASE/bar.txt
```

There are also some other options. You can see them
like so.
```sh
cpp-res --help
```

You can combine this 3 options to get resource tree
without generating source files.
```
cpp-res -header-only -source-only -tree
```

How to build it?
---------------------------------

You need to install [Meson](https://mesonbuild.com)
build system. After that you can compile *cpp-res*
using any compiler you want like so:
```sh
meson build
cd build
ninja
ninja install
```
