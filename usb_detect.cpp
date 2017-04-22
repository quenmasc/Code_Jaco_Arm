
#include <usb.h>
#include <stdio.h>




int main(){
	static libusb_device** __devices;
	
	list_devices(__device);
	return 0;
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
