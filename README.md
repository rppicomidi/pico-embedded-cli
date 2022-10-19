# pico-embedded-cli
demo of embedded-cli library for the Raspberry Pi Pico C SDK

This program uses [Sviatoslav Kokurin's embedded-cli library](https://github.com/funbiscuit/embedded-cli)
to implement an example CLI on Rapsberry Pi Pico UART 0 with UART pins located as follows

```
PICO_DEFAULT_UART_TX_PIN=16
PICO_DEFAULT_UART_RX_PIN=17
```

If you want to use the standard UART0 pins, delete those lines from the CMakeLists.txt file.
If you prefer to use the Pico's USB port for the console, replace the line

```
pico_enable_stdio_uart(${target_name} 1)
```

with

```
pico_enable_stdio_usb(${target_name} 1)
```


To build this demo, clone the source code to some directory `$PICO_PROJECTS` then

```
cd $PICO_PROJECTS
git submodule update --recursive --init
mkdir build
touch compile_commands.json
cmake ..
make
```

You can also use the included .vscode directory to build and debug using VS Code.

I use a Picoprobe for debug and UART console.

