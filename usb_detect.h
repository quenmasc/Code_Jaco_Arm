

#include <libusb_1.0/libusb.h>
#include <stdio.h>

class USB_detect {
	
	static libusb_device** __devices;
	void list_devices(libusb_device **devices);
