/**************************************************\
 * @file serial_linux.h                         
 *
 * @brief Header for functions to interact with serial port
\**************************************************/

#pragma once
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <termios.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>

#ifdef __cplusplus
extern "C" {
#endif
  
  //! returns the descriptor of a serial port
  int serial_open(const char* name);

  //! sets the attributes
  int serial_set_interface_attribs(int fd, int speed, int parity);
  
  //! puts the port in blocking/nonblocking mode
  void serial_set_blocking(int fd, int should_block);

  //! standard init for serial
  int serial_init(const char* name, int baudrate);

  //! resets the arduino
  void reset_arduino(int fd);

#ifdef __cplusplus
}
#endif
