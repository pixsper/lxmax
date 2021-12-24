<img alt="LXMax" src="/icon.png?raw=true" width="200" height="200">

Lighting and DMX control extensions package for Cycling 74's Max.

## Why LXMax
The goal of LXMax is to go beyond simple DMX protocol support/hardware communication (which there are numerous externals already available for) and instead provide full first-class support for the DMX protocol and lighting control as a concept in the Max environment. You don't have to understand how a MIDI note on message is formatted in order to send one in Max, you use the noteout object. Similarily LXMax frees you from having to understand much about DMX or build a mechanism in your patcher to aggregate data into a long list of values. Instead you can simply create objects which represent each DMX fixture.

## Features
* Application-wide DMX service. LXMax maintains a constantly present DMX IO service which all active patchers can access via LXMax objects.
* Fixture objects. Rather than build a complex system to assemble and aggregate a DMX universe from various data sources, simply create an lx.dimmer or lx.colorfixture object and control it's attributes. 
* Abstracted fixture control. Control dimmers, LED and intelligent lights using values that make sense rather than raw DMX 8-bit values.
* Decoupled DMX IO. Setup DMX universes assignments and protocol configuration independently of your patch through an application wide setup window, in the same way that you would assign MIDI/audio IO.
* Flexible protocol usage. Assign and swap protocols and DMX hardware without making changes to your patch.
* Multiple patcher merging. If you have multiple patchers open trying to send data to the same fixtures, LXMax will intelligently merge them to a single output data stream.


## Roadmap
LXMax is still in the development stage and has not yet reached a first release.

- [X] Service extension object, lx.config object and DMX universe configuration UI.
- [X] Basic working IO service and lx.dmx.write/lx.dmx.read objects.
- [X] Support for Art-Net and sACN protocols.
- [ ] Support for Enttec DMX USB Pro (original and Mk 2) hardware.
- [ ] lx.dimmer object for dimmer fixture and basic control.
- [ ] lx.colorfixture object for basic LED and color fixture control.
- [ ] lx.fixture object for intelligent fixture control.
- [ ] lx.personality object for DMX input mapping to object attributes.
- [ ] jit.lx.pixelmap object for mapping Jitter matrices to DMX output.
- [ ] Centralized fixture management/patching UI.

## License
LXMax is available under the [MIT License](https://choosealicense.com/licenses/mit/). This means you can use it free of charge in both commerical and non-commercial projects as long as the license and copyright notice are reproduced.

## Build Requirements
* Visual Studio 2019 or later (Windows)
* XCode 10 or later (Mac OS)
* [CMake](https://cmake.org/) 3.0 or later
* [Conan](https://conan.io/) 1.60

## Dependencies
* Cycling 74's [Max SDK Base](https://github.com/Cycling74/max-sdk-base) (via git submodule)
* [Asio](https://think-async.com/Asio/) (via conan)

## Acknowledgements
* LXMax incorporates code adapted from [Serial Communication Library](https://github.com/wjwwood/serial) under the MIT license.
* Art-Net™ Designed by and Copyright Artistic Licence Holdings Ltd.