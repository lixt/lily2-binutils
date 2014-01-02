lily2-binutils
==============

This is the binutils for LILY2.
LILY2 is a DSP core developed by DSP Group, Institute of Microeletronics, Tsinghua University.

To configure the compilation and installation, run the ``configure'' script in the root directory:

    ./configure --prefix=PATH_TO_INSTALL --target=lily2 --disable-werror

To automatically build them, do:

    make

To install them(by default in /usr/local/bin, /usr/local/lib, etc if PATH_TO_INSTALL is not specified), do:

    make install

If you have any question about the binutils, send email to lixiaotian07@gmail.com.
