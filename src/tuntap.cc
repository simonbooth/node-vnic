#include <node.h>
#include <nan.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <net/if.h>
#include <linux/if_tun.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <arpa/inet.h> 
#include <sys/select.h>
#include <sys/time.h>
#include <errno.h>
#include <stdarg.h>

using v8::FunctionTemplate;
using v8::Handle;
using v8::Object;
using v8::String;
using v8::Function;
using v8::Local;
using v8::Null;
using v8::Number;
using v8::Value;
using v8::Exception;


int tun_alloc(char *dev, int flags) {

  struct ifreq ifr;
  int fd, err;
  char *clonedev = "/dev/net/tun";

  /* Arguments taken by the function:
   *
   * char *dev: the name of an interface (or '\0'). MUST have enough
   *   space to hold the interface name if '\0' is passed
   * int flags: interface flags (eg, IFF_TUN etc.)
   */

   /* open the clone device */
   if( (fd = open(clonedev, O_RDWR)) < 0 ) {
     return fd;
   }

   /* preparation of the struct ifr, of type "struct ifreq" */
   memset(&ifr, 0, sizeof(ifr));

   ifr.ifr_flags = flags;   /* IFF_TUN or IFF_TAP, plus maybe IFF_NO_PI */

   if (*dev) {
     /* if a device name was specified, put it in the structure; otherwise,
      * the kernel will try to allocate the "next" device of the
      * specified type */
     strncpy(ifr.ifr_name, dev, IFNAMSIZ);
   }

   /* try to create the device */
   if( (err = ioctl(fd, TUNSETIFF, (void *) &ifr)) < 0 ) {
     perror("creating TUNSETIFF");
     close(fd);
     return err;
   }

  /* if the operation was successful, write back the name of the
   * interface to the variable "dev", so the caller can know
   * it. Note that the caller MUST reserve space in *dev (see calling
   * code below) */
  strcpy(dev, ifr.ifr_name);

  /* this is the special file descriptor that the caller will use to talk
   * with the virtual interface */
  return fd;
}

NAN_METHOD(AddTap) {
  
  NanScope();
  
  if (args.Length() < 1) {
    ThrowException(Exception::TypeError(String::New("Wrong number of arguments")));
    NanReturnUndefined();
  }
  
  int tap_fd;
  //strcpy(tun_name, "tun1");
  //tunfd = tun_alloc(tun_name, IFF_TUN);  /* tun interface */

  tap_fd = tun_alloc(*NanUtf8String(args[0]), IFF_TAP);  /* tap interface */
  
  if(ioctl(tap_fd, TUNSETPERSIST, 1) < 0){
      perror("enabling TUNSETPERSIST");
      exit(1);
    }

  NanReturnValue(NanNew<int>(tap_fd))
}

NAN_METHOD(AddTun) {
  
  NanScope();
  
  if (args.Length() < 1) {
    ThrowException(Exception::TypeError(String::New("Wrong number of arguments")));
    NanReturnUndefined();
  }
  
  int tun_fd;
 
  tun_fd = tun_alloc(*NanUtf8String(args[0]), IFF_TUN);  /* tun interface */
  
  if(ioctl(tun_fd, TUNSETPERSIST, 1) < 0){
      perror("enabling TUNSETPERSIST");
      exit(1);
    }

  NanReturnValue(NanNew<int>(tun_fd))
}

void init(Handle<Object> exports) {
    exports->Set(String::NewSymbol("addTap"),
      NanNew<FunctionTemplate>(AddTap)->GetFunction());
    exports->Set(String::NewSymbol("addTun"),
      NanNew<FunctionTemplate>(AddTun)->GetFunction());
}

NODE_MODULE(tuntap, init)