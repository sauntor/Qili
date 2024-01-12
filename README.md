# Qili弹幕姬   <small>中文|[English](README_en.md)</small>

### 介绍
Qili 是一款免费且开源的B站直播弹幕语音播报软件，主要适配适配 `Windows` 及 `Linux` 平台。
> 主站: https://github.com/sauntor/Qili
> 镜像：https://gitee.com/sauntor/Qili

### 版权声明
本软件使用 [GPL v3](https://www.gnu.org/licenses/gpl-3.0.txt) 进行授权。

### 软件架构
语音播报功能依赖Qt的TextToSpeech引擎，Qt会利用平台系统的TTS组件，
所以（可能）需要安装一些系统组件。<br/>
本软件采用模拟浏览器登录的方式从B站获取弹幕数据，当然，不登录也可使用基本功能。

### 计划实现的功能
- [] 适配暗色模式
- [] 历史弹幕存储
- [] 弹幕数据分析

### 已适配系统
1. Windows 10+ x86_64/amd64
1. Ubuntu/Kylin 22.04
2. openSUSE Tumbleweed

### 安装教程
##### Windows
1. 下载 [安装包](https://github.com/sauntor/Qili/releases/download/v1.0.0/Qili-1.0.0-Windows-AMD64.exe)
2. 双击软件包，按照引导进行安装
3. 打开软件并登录（可匿名），在 `设置` 中测试并选择合适的语音

##### openSUSE
1. 安装系统语音组件
    ```bash
    sudo zypper in --recommends speech-dispatcher speech-dispatcher-configure speech-dispatcher-module-espeak espeak-ng
    ```
2. 下载 [RPM 安装包](https://github.com/sauntor/Qili/releases/download/v1.0.0/me.sauntor.qili-1.0.0-opensuse-tumbleweed.20240105_x86_64.rpm)
3. 单/双击软件包安装，或执行命令<br/>
   ```bash
   sudo zypper in me.sauntor.qili-1.0.0-opensuse-tumbleweed.20240105_x86_64.rpm
   ```
4. （可选）编译安装`mbrola`（自行百度）
5. （可选）运行 `spd-conf -u` 按照引导为**当前用户**生成 `speech-dispatcher` 配置
6. 打开软件并登录（可匿名），在 `设置` 中测试并选择合适的语音

##### Ubuntu/Kylin 22.04
1. 下载 [DEB 安装包](https://github.com/sauntor/Qili/releases/download/v1.0.0/me.sauntor.qili-1.0.0-ubuntu.22.04_x86_64.deb)
2. 打开终端，并用下面的命令安装<br/>
   ```bash
   sudo apt install --fix-broken --install-recommends me.sauntor.qili-1.0.0-ubuntu.22.04_x86_64.deb
   ```
3. （可选）`sudo apt install python3-speechd`
4. （可选）运行 `spd-conf -u` 按照引导为**当前用户**生成 `speech-dispatcher` 配置
5. 打开软件并登录（可匿名），在 `设置` 中测试并选择合适的语音

### 使用说明
1. `房间号` 不限于自己的直播间
2. 匿名登录可能会接收不到直播间的弹幕数据，（由于B站隐私策略，）即使收到弹幕也无法看到用户全名，但仍能接收到用户进入、本场观众数等信息。
3. 连接B站成功后，此程序会隐藏到系统托盘
4. 单击系统托盘图标可显示本场弹幕记录
5. 双击或中键单击系统托盘图标可以 暂停/继续 语音播报（如果系统支持的话）
6. 本软件不会收集用户的任何信息（纯本地软件，无服务端）

### 参与贡献
1. 代码风格：[KDE Code Style](https://community.kde.org/Policies/Frameworks_Coding_Style)
2. 在龙芯等国产硬件上测试并打包本软件
3. bug修复、帮助实现`计划实现的功能`并`Pull Request`

### 编译打包
> 1. 主要依赖：`Qt`(`Widgets` `Network`, `WebSockets` `TextToSpeech`), `brotli`
> 2. 发布时请添加： `-DCMAKE_BUILD_TYPE=Release` 或者 `-DCMAKE_BUILD_TYPE=RelWithDebInfo`

1. openSUSE Tumbleweed
    ```
    # 安装依赖
    sudo zypper in -t pattern devel_C_C++ devel_qt6
    # 下面的命令可能不需要执行
    sudo zypper in --recommends cmake qt6-texttospeech-devel qt6-websockets-devel libbrotli-devel
    # 创建编译文件夹
    mkdir build && cd build
    # (A)初始化编译配置
    cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo -B . -S /path/to/source/of/Qili
    # 编译
    cmake --build . --clean-first --verbose --target all
    # 打包，在 (A) 处的命令上增加参数: -DUSE_CPACK=ON
    cpack --config CPackConfig.cmake -G RPM -V
    # Qt Creator集成
    #     将 (A) 处命令换成
    cmake -DDEV_MODE=ON \
        -DCMAKE_BUILD_TYPE=Debug \
        -DCMAKE_INSTALL_PREFIX=$HOME/QiliDev \
        -B . \
        -S /path/to/source/of/Qili
    #     安装
    cmake --build . --clean-first --verbose --target install
    #     启动QtCreator
    LD_LIBRARY_PATH=$HOME/QiliDev/lib64 \
    QT_PLUGIN_PATH=$HOME/QiliDev/plugins \
    qtcreator
    ```
2. Ubuntu/Kylin 22.04
    ```bash
    # 安装依赖
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
    # 创建编译文件夹
    mkdir build && cd build
    # (A) 初始化编译配置
    cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo -DUSE_QT5=ON -B . -S /path/to/source/of/Qili
    # 编译
    cmake --build . --clean-first --verbose --target all
    # 打包，另需在 (A) 处的命令上增加参数: -DUSE_CPACK=ON
    cpack --config CPackConfig.cmake -G DEB -V
    ```

### 赞助
> 自愿赞助作者工作的，可通过应用内的 `鸣谢` 或下面的二维码捐助

- 支付宝<br/>

![支付宝](App/images/alipay.png)

- 微信支付<br/>

![微信支付](App/images/wechat.png)

### 鸣谢
> 本项目使用或参考了以下项目
1. https://github.com/SocialSisterYi/bilibili-API-collect
2. https://www.qt.io/
3. https://github.com/google/brotli
4. https://github.com/nayuki/QR-Code-generator
