#include <zmq.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "hdf5.h"
#include "hdf5_hl.h"

#define WIDTH         4
#define HEIGHT        2
#define PAL_ENTRIES   9
unsigned char buf [ WIDTH*HEIGHT ];

int main (void)
{
    hid_t         file_id;
    herr_t        status;
    size_t        i, j;
    int          n, space;

	/* create an image of 9 values divided evenly by the array */
	 space = WIDTH*HEIGHT / PAL_ENTRIES;
	 for (i=0; i < WIDTH*HEIGHT; i++)
	 {
	  buf[i] = i*i;
	 }
	 printf("buf : %u\n", buf[1]);
	 printf("buf : %u\n", buf[2]);
	 printf("buf : %u\n", buf[3]);

    printf ("Connecting to server...\n");
    void *context = zmq_ctx_new ();
    void *requester = zmq_socket (context, ZMQ_PUSH);
	//Modify the ip to your server address, not client local address.
    zmq_connect (requester, "tcp://192.168.1.107:5555"); 

	 hid_t fapl;
   fapl = H5Pcreate(H5P_FILE_ACCESS);
   H5Pset_fapl_core(fapl,8,0);
 /* create a new HDF5 file using default properties. */
 file_id = H5Fcreate( "ex_image1.h5", H5F_ACC_TRUNC, H5P_DEFAULT, fapl);

 /* make the image */
 // define the dataset of "image1" it is special dataset only for image. we can use palette too.
 status = H5IMmake_image_8bit( file_id, "image1", (hsize_t)WIDTH, (hsize_t)HEIGHT, buf );

 /* make a palette. palette could be used as normal dataset */
 //status = H5IMmake_palette( file_id, "pallete", pal_dims, pal );

 H5Fflush(file_id, H5F_SCOPE_LOCAL);

 size_t size = H5Fget_file_image(file_id, NULL, 0);
 int * buffer_ptr = malloc(size);
 H5Fget_file_image(file_id, buffer_ptr, size);

  printf("the image size is %zu\n",size);
    int request_nbr;
    for (request_nbr = 1; request_nbr <= 10; request_nbr++) {
	
        printf ("Sending data %d...\n", request_nbr);
	
        zmq_send (requester, buffer_ptr, size, 0);
    }
    H5Pclose(fapl);
    H5Fclose( file_id );
    zmq_close (requester);
    zmq_ctx_destroy (context);
    return 0;
}
