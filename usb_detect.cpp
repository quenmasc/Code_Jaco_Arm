
#include <libusb-1.0/libusb.h>
#include <stdio.h>
#include "usb_detect.h"


USB_detect USB_detect;
static libusb_device** __devices;
static libusb_context *__ctx = NULL;
void list_devices();
void close_usb();
void list_devices(libusb_device **devices);
error_t init_usb();
int main(){
	list_devices();
	 
	return 0;
}


error_t
init_usb()
{
  int r = libusb_init(&__ctx);
  if( r<0 ) {
    fprintf(stderr, "Error initializing libusb. libusb-error: %i", r);
    return ERROR_USB_INIT;
  }

  return ERROR_NONE;
}

/** Exit libusb.
 * This closes the explicitly created libusb context. It is not needed to call
 * this when 'init_usb()' has not been called before. However, calling this does
 * no harm (no exception throwing etc.).
 */
void
close_usb()
{
  libusb_exit(__ctx);
  __ctx = NULL;
}
void
list_devices(libusb_device **devices)
{
  libusb_device *dev;
  int i = 0;

  while ((dev = devices[i++]) != NULL) {
    struct libusb_device_descriptor desc;
    int r = libusb_get_device_descriptor(dev, &desc);
    if (r < 0) {
      fprintf(stderr, "failed to get device descriptor");
      return;
    }

    printf("idVendor:%04x  idProduct:%04x  SN:%02x (bus %d, device %d)\n",
    desc.idVendor, desc.idProduct, desc.iSerialNumber,
    libusb_get_bus_number(dev), libusb_get_device_address(dev));
  }
}

void
list_devices()
{
  bool tmp_ctx = false;
  if( __ctx == NULL ) {
    // try creating a temporary context
    error_t err = init_usb();
  //  if( err != ERROR_NONE )
   //    throw KinDrvException(err, "Failed to initialize temporary libusb context");
  //  else
    tmp_ctx = true;
  }

  // Get devices
  ssize_t cnt;
  cnt = libusb_get_device_list(__ctx, &__devices);
  if( cnt<0 ) {
    fprintf( stderr, "Get_device_list error: %i \n", cnt);
  } else {
    printf("%i USB devices detected \n", cnt);

    list_devices(__devices);

    // Clear devices list
    libusb_free_device_list(__devices, /*auto-unref*/ true);
  }

  if( tmp_ctx )
    close_usb();
}

