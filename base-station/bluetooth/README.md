EE175-SeniorDesign-ASSP-Base Station-Bluetooth
==============================================
Code for bluetooth communication on the Raspberry Pi B+

Hardware
--------
[Bluetooth 4.0 USB Module](http://www.adafruit.com/product/1327)

Setup
-----
Install Required Libraries

  sudo apt-get install libusb-dev libdbus-1-dev libglib2.0-dev libudev-dev libical-dev libreadline-dev

Download Bluez

  sudo mkdir bluez
  cd bluez
  sudo wget www.kernel.org/pub/linux/bluetooth/bluez-5.11.tar.xz

Unzip and Compile Bluez

  sudo unxz bluez-5.11.tar.xz
  sudo tar xvf bluez-5.11.tar
  cd bluez-5.11
  sudo ./configure --disable-systemd
  sudo make
  sudo make install

Insert the USB Module and Reset
  Once Bluez has been built, shut down your computer

    sudo shutdown -h

  insert Bluetooth 4.0 USB Module and then reset the Raspberry Pi so that all of the changes we have made can take effect.
