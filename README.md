# Wooting Analog SDK
Direct access to raw analog values from an analog keyboard for game and application developers.

## Getting Started

This project will allow you to build your own DLL and customize the source code. Looking to directly get started with the DLL? Check out the downloads and guides in our [developer portal](https://dev.wooting.io)

### Prerequisites

#### Windows
[Visual studio](https://visualstudio.microsoft.com/)
#### Linux
to-do

### Installing


```
git clone --recursive https://github.com/WootingKb/wooting-analog-sdk.git 
```
Open the visual studio project and select x64 (64-bit) or x86 (32-bit)

Now hit the build and find your DLL in the folder depending on your configuration

64-bit
```
\wooting-analog-sdk\windows\x64\Release\wooting-analog-sdk.dll
```
32-bit
```
\wooting-analog-sdk\windows\Release\wooting-analog-sdk.dll
```

## Example

For examples check out the [wootdev website](https://dev.wooting.io)

## Keyboard Matrix
Keyboard matrix with just rows and columns
![Keyboard Matrix Row Columns](resources/keyboard-matrix-rows-columns.png)

Keyboard matrix with scan indexes for the read full buffer
![Keyboard Matrix Scan Index](resources/keyboard-matrix-scan-index.png)

## License

This project is licensed under the Mozilla Public License Version 2.0 - see the [LICENSE](LICENSE) file for details

## Acknowledgments

* Jeroen
* Erik

