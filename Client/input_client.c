#include "serial_linux.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
  if (argc<3) {												//Controllo di errore mancanza di parametri <filename> <banda>
    printf("usage: %s <filename> <baudrate>", argv[0]);
    return 0;
  }
  char* filename=argv[1];
  int baudrate=atoi(argv[2]);
  printf( "opening serial device [%s] ... ", filename);
  int fd=serial_open(filename);
  if (fd<=0) {
    printf ("Error\n");
    return 0;
  } else {
    printf ("Success\n");
  }
  printf( "setting baudrate [%d] ... ", baudrate);
  int attribs=serial_set_interface_attribs(fd, baudrate, 0);
  if (attribs) {
    printf("Error\n");
    return 0;
  } 
  else{ printf("\n"); }

  serial_set_blocking(fd, 1);
 
  const int bsize=3;
  char buf[bsize];
  buf[0]='H';
  buf[1]='\n';
  buf[2]='\0'; 
  ssize_t n_write=write(fd, buf, 3);
  n_write=write(fd, buf, 3);
  n_write=write(fd, buf, 3);
  n_write=write(fd, buf, 3);
  n_write=write(fd, buf, 3);
  close(fd);
  return 0;
}
