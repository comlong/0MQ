#include <zmq.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include "hdf5.h"
#include "hdf5_hl.h"

#define WIDTH         4
#define HEIGHT        2

int main (void)
{

hid_t         file_id;
 herr_t        status;
 size_t        i, j;
 int           n, space;
  hid_t fid;
   hid_t fapl_id;


    //  Socket to talk to clients
    void *context = zmq_ctx_new ();
    void *receiver = zmq_socket (context, ZMQ_PULL);
    int rc =  zmq_bind (receiver, "tcp://192.168.1.104:5555"); //modify the ip to your server address
   sleep (1);
    assert (rc == 0);

    while (1) {
	zmq_msg_t msg;
	rc = zmq_msg_init (&msg);
	assert (rc == 0);
	int nbytes = zmq_recvmsg(receiver, &msg, 0); 
	assert (nbytes != -1);
    
        printf ("Received %d\n", nbytes);
        sleep (1);          //  Do some 'work'

unsigned char buf1 [ WIDTH*HEIGHT ];
   
   fapl_id = H5Pcreate(H5P_FILE_ACCESS);
   H5Pset_fapl_core(fapl_id,8,0);

  // size_t file_id = H5Pset_file_image(fapl_id, buffer, 2152);

//open the HDF5 image from the buffer
 fid = H5LTopen_file_image(zmq_msg_data(&msg), nbytes, H5LT_FILE_IMAGE_DONT_COPY);
 //read the data
 status = H5IMread_image(fid,"image1",buf1);
 printf("buf1 : %u\n", buf1[1]);
 printf("buf1 : %u\n", buf1[2]);
 printf("buf1 : %u\n", buf1[3]);
zmq_msg_close (&msg);
    }
H5Fclose(fid);
H5Pclose(fapl_id);
zmq_close (receiver);
    return 0;
}
