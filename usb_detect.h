

#include <libusb-1.0/libusb.h>
#include <stdio.h>

class USB_detect {
	public :	
	static libusb_device**;
	void list_devices(libusb_device **devices);
};
