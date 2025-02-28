/**************************************************\
 * @file serial_linux.c                         
 *
 * @brief Implementation of functions to interact with serial port
\**************************************************/

#include "serial_linux.h"

int serial_init(const char* name, int baudrate) {
  int fd = serial_open(name);
  if (fd == -1) {
    perror("serial_open");
    exit(EXIT_FAILURE);
  }
  serial_set_interface_attribs(fd, baudrate, 0);
  serial_set_blocking(fd, 0);
  return fd;
}

int serial_set_interface_attribs(int fd, int speed, int parity) {
  struct termios tty;
  memset (&tty, 0, sizeof tty);

  if (tcgetattr (fd, &tty) != 0) 
  {
    printf ("error %d from tcgetattr", errno);
    return -1;
  }

  switch (speed)
  {
    case 19200:
      speed=B19200;
      break;
    case 57600:
      speed=B57600;
      break;
    case 115200:
      speed=B115200;
      break;
    case 230400:
      speed=B230400;
      break;
    case 576000:
      speed=B576000;
      break;
    case 921600:
      speed=B921600;
      break;
    default:
      printf("cannot sed baudrate %d\n", speed);
      return -1;
  }
  
  cfsetospeed (&tty, speed);
  cfsetispeed (&tty, speed);
  cfmakeraw(&tty);

  // enable reading
  tty.c_cflag &= ~(PARENB | PARODD);               // shut off parity
  tty.c_cflag |= parity;
  tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;      // 8-bit chars

  if (tcsetattr (fd, TCSANOW, &tty) != 0) 
  {
    printf ("error %d from tcsetattr", errno);
    return -1;
  }

  return 0;
}

void serial_set_blocking(int fd, int should_block) {
  struct termios tty;
  memset (&tty, 0, sizeof tty);
  if (tcgetattr (fd, &tty) != 0) {
      printf ("error %d from tggetattr", errno);
      return;
  }

  tty.c_cc[VMIN]  = should_block ? 1 : 0;
  tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

  if (tcsetattr (fd, TCSANOW, &tty) != 0)
    printf ("error %d setting term attributes", errno);
}

int serial_open(const char* name) {
  int fd = open (name, O_RDWR | O_NOCTTY | O_SYNC );
  if (fd < 0) {
    printf ("error %d opening serial, fd %d\n", errno, fd);
  }
  reset_arduino(fd);
  return fd;
}

void reset_arduino(int fd) 
{
  // Manually toggle DTR to reset Arduino
  int dtr_flag = TIOCM_DTR;
  ioctl(fd, TIOCMBIC, &dtr_flag); // Clear DTR
  usleep(10000);                  // Wait 10ms
  ioctl(fd, TIOCMBIS, &dtr_flag); // Set DTR
}
