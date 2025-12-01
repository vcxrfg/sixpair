/*
 * sixpair.c - program to set the master bluetooth address of a PS3 Sixaxis
 *
 * Based on work from: http://ps3.jim.sh/sixaxis/usb.html
 */

#include <stdio.h>
#include <string.h>
#include <usb.h>

int main(int argc, char *argv[])
{
    struct usb_bus *bus;
    struct usb_device *dev;
    usb_dev_handle *udev;
    unsigned char cmd[8];
    unsigned char mac[6];
    int i;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <Bluetooth MAC>\n", argv[0]);
        return 1;
    }

    if (sscanf(argv[1], "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx",
               &mac[0], &mac[1], &mac[2], &mac[3], &mac[4], &mac[5]) != 6) {
        fprintf(stderr, "Invalid MAC format\n");
        return 1;
    }

    usb_init();
    usb_find_busses();
    usb_find_devices();

    for (bus = usb_get_busses(); bus; bus = bus->next) {
        for (dev = bus->devices; dev; dev = dev->next) {
            if (dev->descriptor.idVendor == 0x054c &&
                dev->descriptor.idProduct == 0x0268) {

                udev = usb_open(dev);

                if (!udev) {
                    fprintf(stderr, "Unable to open device\n");
                    continue;
                }

                printf("Found Sixaxis controller\n");
                usb_claim_interface(udev, 0);

                cmd[0] = 0x01;
                cmd[1] = 0x00;
                memcpy(cmd + 2, mac, 6);

                usb_control_msg(udev,
                                USB_TYPE_CLASS | USB_RECIP_INTERFACE | USB_ENDPOINT_OUT,
                                0x09,
                                0x0301,
                                0,
                                (char *)cmd,
                                sizeof(cmd),
                                0);

                printf("Wrote master Bluetooth address: %02x:%02x:%02x:%02x:%02x:%02x\n",
                       mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

                usb_release_interface(udev, 0);
                usb_close(udev);
                return 0;
            }
        }
    }

    printf("No Sixaxis found.\n");
    return 1;
}
