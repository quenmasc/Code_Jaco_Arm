

#include <libusb-1.0/libusb.h>
#include <stdio.h>
#include "types.h"
#include "exception.h"
struct libusb_device;
struct libusb_device_handle;


class USB_detect {
	public :	
	error_t init_usb();
	void close_usb();

	void list_devices();
};
