0MQ
===

How to install 0MQ on server and ARM board,  and make a simple test.

Find the 0MQ from http://zeromq.org/area:download

Extrct the tar file. Simple using ./configure , make , make install

find h5cc from "hdf5folder"/bin/h5cc

h5cc -c hwserver.c

h5cc -o hwserver hwserver .c -lzmq


h5cc -c hwclient.c

h5cc -o hwclient hwclient.c -lzmq
