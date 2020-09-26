# phash
C++ clone of phash function from https://github.com/JohannesBuchner/imagehash/blob/master/imagehash.py#L193

This project currently contains an ImageHash class (same as original one except it can't restore itself back from a string yet), a phash() function producing the same result as the original function and a small demo application printing a hash of a specified image.

## Compiling and running the project

The project is based on ImageMagick 7. Here are the downloads and instructions on its installation: https://imagemagick.org/script/download.php

The project is made in Xcode, one can use the project files provided in this repository.

Also the project can be compiled using CMake. Open a terminal / command prompt in the project's phash directory and run two commands:
```
cmake .
make
```

After all, the project consists only from two hpp and three cpp files, thus it's not a problem to compile and run it in your favourite IDE or anything.
