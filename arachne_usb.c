/*============================----beg-of-source---============================*/

#include  "arachne.h"

#include  <termios.h>             /* clibc standard terminal control          */

typedef struct termios TERMIOS;
TERMIOS   oldtio;
TERMIOS   newtio;

int       x_port    = 0;       /* port file descriptor                     */


int        /* ---- : open the usb/serial port for use ------------------------*/
usb_open           (char *a_dev)
{
   /*---(notes)--------------------------*/
   /*
    *   O_RDRW    : both read and write
    *   O_NOCTTY  : don't be the controlling terminal for the port (no sigs)
    *   O_NODELAY : don't wait for the other end of the port to be ready
    *
    */
   /*---(locals)-------------------------*/
   int       fd        = 0;       /* file descriptor                          */
   /*---(open)---------------------------*/
   fd = open(a_dev, O_RDWR | O_NOCTTY | O_NONBLOCK);
   if (fd == -1) {
      fprintf(stderr, "can not open serial port %s\n", a_dev);
   } else {
      fprintf(stderr, "port %s is successfully open at %d\n", a_dev, fd);
      fcntl (fd, F_SETFL, 0);
   }
   /*---(get current term settings)------*/
   tcgetattr(fd, &oldtio);
   /*---(prepare new one)----------------*/
   bzero(&newtio, sizeof(TERMIOS));
   newtio.c_cflag = B115200 | CS8 | CLOCAL | CREAD;
   cfsetispeed(&newtio, B115200);
   cfsetospeed(&newtio, B115200);
   /*> newtio.c_cflag |= (CLOCAL | CREAD);                                            <* 
    *> newtio.c_cflag &= ~CSIZE;                                                      <* 
    *> newtio.c_cflag &= ~PARENB;                                                     <* 
    *> newtio.c_cflag &= ~CSTOPB;                                                     <* 
    *> newtio.c_cflag |= CS8;                                                         <*/
   newtio.c_iflag  = IGNPAR  | ICRNL   ;
   newtio.c_oflag  = 0       ;
   newtio.c_lflag  = ICANON  ;
   tcflush(fd, TCIFLUSH);
   tcsetattr(fd, TCSANOW, &newtio);
   /*---(complete)-----------------------*/
   return fd;
}

char       /*----: PURPOSE : close a serial port -----------------------------*/
ySERIAL_close      (int a_port)
{
   /*---(defenses)-----------------------*/
   if (a_port < 0) return -1;
   /*---(locals)-------------------------*/
   int       rc        = 0;       /* generic return code                      */
   /*---(close)--------------------------*/
   rc = close(a_port);
   if (rc == 0) {
      fprintf(stderr, "port is successfully closed\n");
   } else {
      fprintf(stderr, "could not close serial port\n");
   }
   /*---(set terminal back)--------------*/
   tcsetattr(a_port, TCSANOW, &oldtio);
   /*---(complete)-----------------------*/
   return 0;
}


/*============================----end-of-source---============================*/
