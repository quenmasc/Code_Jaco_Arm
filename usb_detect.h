

#include <libusb-1.0/libusb.h>
#include <stdio.h>
#include "types.h"
struct libusb_device;
struct libusb_device_handle;


class USB_detect {
	public :	
	void list_devices();
};
