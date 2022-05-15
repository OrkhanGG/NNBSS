# NNBSS
This repository houses the source code for NNBSS.


### Building
In order to build binary files, you will need to download some thirdparty SDKs. The rest will be done by CMake. 

### SDKs
NNBSS uses following SDKs: 
WxWidgets 3.1.6 version
OpenCV 4.5.5 version
SDL2 2.0.20 version
ffmpeg
CrashRpt 1403 version
Boost 1.78.0 version

### Branches
Development takes place primarily in the "development" branch. 
The "stabilisation" branch is used for fixing bugs in the run-up to release, 
and the "release" branch provides stable snapshots of NNBSS engine.

To prepare for a major (feature) release, I (Orkhan Aliyev) may integrate "development" into "stabilisation", 
and then continue fixing bugs in "stabilisation". 
To prepare for a minor (stability) release, individual changes from 'development are integrated directly into "stabilisation".
In each case, when the release is deemed ready, "stabilisation" is integrated to "release".


### The Terms of Use (Brief)
Please be aware that these files you have downloaded can only be modified, copied or duplicated after
obtaining permission from both of the owners of this project and the Loam company, namely Abdulov Kanan Zaur and Orkhan Aliyev Arif .
Otherwise, you accept the penalty imposed by the Loam company: https://www.loaminc.space/terms-of-service
(Information about the penalty may be imposed[in case of violation of the terms of use] and other license information are available in the License.md file.)