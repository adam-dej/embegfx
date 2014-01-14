embegfx
=======

Minimalistic, modular and easily extensible graphics library for embedded systems like AVR.

Goal of this library is to provide basic drawing routines and drivers for various displays or display controllers. It supports arbitrary number of displays being connected to a single MCU (if its resources allow), and adding drivers for new devices is a simple matter of implementing just a few functions.

At present, this library supports only 1 bit displays, but support for other display types can be added as necessary.

For more detailed documentation of drawing routines, display devices and help with extending this library please see the Wiki.

Supported devices
-----------------

  - pcf8531 - based displays
  - Virtual mirroring device

Why aren't low level interface drivers implemented?
---------------------------------------------------

  - More things than displays can be connected to those interfaces, therefore graphic library has no business implementing drivers for them.
  - Letting the user implement those leads to more freedom in the design.
  - Not implementing MCU-specific features in the library makes it more universal for usage on many MCU's
  - It allows for more extensibility: no one ever said that for example display which would usually connect directly to the I/O lines of the processor won't be connected through an I2C GPIO extender.
