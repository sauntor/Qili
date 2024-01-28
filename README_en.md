# Qili  <small>[中文](README.md)|English</small>

### What's it?
Qili is a subtitle spearker for live broadcasting at bilibili.com.

### License
This software is licensed under [GPL v3](https://www.gnu.org/licenses/gpl-3.0.txt)

### How it works?
Qili using `Qt`'s `TextToSpeech` engine to speak subtitles out, targeting on Linux platform.
So, you may need to install some extra components provided by your Linux vendor.

### Schedules
- [] Adopt to Dark Mode
- [] Subtitle Storage
- [] Subtitle Analysis

### Supported OS
1. Windows 10 x86_64/amd64
2. Ubuntu/Kylin 22.04
3. openSUSE Tumbleweed

### Installation
> Please replace `/path/to/...` with the real path to the file

##### Windows
1. Download the [Installer](https://github.com/sauntor/Qili/releases/download/v1.0.0/Qili-1.0.0-Windows-AMD64.exe)
2. Double click `Qili-1.0.0-Windows-AMD64.exe` and follow the instructions.
3. Launch Qili, login by scanning QRCode or just as anonymous, test voices using `Qili Settings`, and `Apply` if some one is ok.

##### openSUSE
> Please replace `/path/to/...` with the real path to the file
1. Install required components
    ```bash
    sudo zypper in --recommends speech-dispatcher speech-dispatcher-configure speech-dispatcher-module-espeak espeak-ng
    ```
2. Download [Tumbleweed RPM](https://gitee.com/sauntor/Qili/releases/download/v1.0.0/me.sauntor.qili-1.0.0-opensuse-tumbleweed.20240105_x86_64.rpm)
3. Click on the rpm and following the instruction, or run the command with a terminal:<br/>
   ```bash
   sudo zypper in ./me.sauntor.qili-1.0.0-opensuse-tumbleweed.20240109_x86_64.rpm
   ```
4. Optional: compile and install `mbrola` from source code, you may search Google for help
5. Optional: run `spd-conf -u` and following the instruction to generate a `speech-dispatcher` config file for you
6. Launch Qili, login by scanning QRCode or just as anonymous, test voices using `Qili Settings`, and `Apply` if some one is ok.

##### Ubuntu/Kylin 22.04
1. Download [Ubuntu DEB](https://gitee.com/sauntor/Qili/releases/download/v1.0.0/me.sauntor.qili-1.0.0-Deepin.20.9_x86_64.deb)
2. Open `Terminal`, and run:<br/>
   ```bash
   sudo apt install --fix-broken --install-recommends ./me.sauntor.qili-1.0.0-ubuntu.22.04_x86_64.deb
   ```
3. Optional: `sudo apt install python3-speechd`
4. Optional: run `spd-conf -u` and following the instruction to generate a `speech-dispatcher` config file for you
5. Launch Qili, login by scanning QRCode or just as anonymous, test voices using `Qili Settings`, and `Apply` if some one is ok.

### Usage
1. `ROOM` number is not limited to your own
2. Qili may not receive subtitles from bilibili.com or receive subtitles without user names, if you login as an anonymous,
    biblibili controls this on it's privacy. But Qili can still speak it out when someone enter the room.
3. Qili will hide to System Tray after it connected to bilibili.com.
4. Click the tray icon to show the last 100 subtitles.
5. Double click or Middle click to `Pause`/`Resume` (if it supported by your system)
6. Qili does not collection any user's data, and no server side on its own

### Contribution
1. Following [KDE Code Style](https://community.kde.org/Policies/Frameworks_Coding_Style)
2. Fix bug, migrate, test and/or package Qili for other Linux distributions
3. Working on `Schedules` and make a `Pull Request`

### Compile & Package
> Please add the following parameter to cmake arguments when Releasing/Packaging：<br/>
> `-DCMAKE_BUILD_TYPE=Release` or `-DCMAKE_BUILD_TYPE=RelWithDebInfo`

1. openSUSE Tumbleweed
    ```
    # Install required components
    sudo zypper in -t pattern devel_C_C++ devel_qt6
    # The following line may not needed
    sudo zypper in --recommends cmake qt6-texttospeech-devel qt6-websockets-devel libbrotli-devel
    # Make a directory for building
    mkdir build && cd build
    # [1]Setup with cmake
    cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo -B . -S /path/to/source/of/Qili
    # Compile
    cmake --build . --clean-first --verbose --target all
    # Packaging，you should add -DUSE_CPACK=ON to the arguments on [1], or re-execute [1] with this param
    cpack --config CPackConfig.cmake -G RPM -V
    # Integrate Custom Widgets with Qt Creator
    #     replace/execute the command at [1]
    cmake -DDEV_MODE=ON \
        -DCMAKE_BUILD_TYPE=Debug \
        -DCMAKE_INSTALL_PREFIX=$HOME/QiliDev \
        -B . \
        -S /path/to/source/of/Qili
    #     Install all (include qt designer plugin) components
    cmake --build . --clean-first --verbose --target install
    #     Launch QtCreator
    LD_LIBRARY_PATH=$HOME/QiliDev/lib64 \
    QT_PLUGIN_PATH=$HOME/QiliDev/plugins \
    qtcreator
    ```
2. Ubuntu/Kylin 22.04
    ```bash
    # Install required components
    sudo apt install --install-recommends \
    build-essential \
    cmake \
    qtbase5-dev \
    qtbase5-dev-tools \
    qttools5-dev \
    qttools5-dev-tools \
    libqt5texttospeech5-dev \
    libqt5websockets5-dev \
    libbrotli-dev
    # Make a directory for building
    mkdir build && cd build
    # [1]Setup with cmake
    cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo -DUSE_QT5=ON -B . -S /path/to/source/of/Qili
    # Compile
    cmake --build . --clean-first --verbose --target all
    # Packaging，you should add -DUSE_CPACK=ON to the arguments on [1], or re-execute [1] with this param
    cpack --config CPackConfig.cmake -G DEB -V
    ```

#### Thanks to
1. https://github.com/SocialSisterYi/bilibili-API-collect
2. https://www.qt.io/
3. https://github.com/google/brotli
4. https://github.com/nayuki/QR-Code-generator
