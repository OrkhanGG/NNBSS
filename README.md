# NNBSS
This repository houses the source code for NNBSS.

## Building
In order to build binary files, you will need to download some thirdparty SDKs. The rest will be done by CMake. 

NNBSS engine uses these SDKs:
- wxWidgets 3.1.6 version

- OpenCV 4.5.5 version

- SDL2 2.0.20 version

- FFmpeg

- CrashRpt 1403 version

- Boost 1.78.0 version

In order to compile, you will need to follow these steps:
1. Install Visual Studio 2019.
2. Install latest CMake version.
2. Download the SDKs and Assets zip from the releases page.
3. Extract the Assets to build folder(where solution is located. When you use executable file directly, Assets folder should be in the same directory.).
4. Extract the SDKs zip to Code/SDKs

CMake must be used to compile NNBSS engine.

## Branches
Development will take place primarily in the "development" branch. 
The "stabilisation" branch is used for fixing bugs in the run-up to release, 
and the "release" branch provides stable snapshots of NNBSS engine.

To prepare for a major (feature) release, we may integrate "development" into "stabilisation", 
and then continue fixing bugs in "stabilisation". 
To prepare for a minor (stability) release, individual changes from 'development are integrated directly into "stabilisation".
In each case, when the release is deemed ready, "stabilisation" is integrated to "release".

## Source Code usage
We're welcome to contribitions.
Please, don't forget that this project cannot be modified or published in any way without the permission of Loam Inc, especially for any income.
Otherwise, you accept the penalty imposed by the Loam Inc: https://www.loaminc.space/terms-of-service
(Information about the penalty may be imposed and other license information are available in the [License.md](https://github.com/OrkhanGG/NNBSS/blob/main/LICENSE.md) file.)

# NNBSS Forum
If you have any questions other than an issue, please use NNBSS forum:
https://www.loaminc.space/forum