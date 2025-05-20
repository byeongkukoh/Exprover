# 🔥 IoT RC CAR DashBoard

프로젝트에서 사용 가능한 윈도우 기반 어플리케이션입니다.

## 🧑‍💻 기술 스택

![C++](https://img.shields.io/badge/C++-00599C?style=for-the-badge&logo=c%2b%2b&logoColor=white)

### IDE

![Visual Studio 2022](https://img.shields.io/badge/Visual%20Studio%202022-5C2D91?style=for-the-badge&logo=visualstudio&logoColor=white)

### API

![Windows API](https://img.shields.io/badge/Windows%20API-0078D6?style=for-the-badge&logo=windows&logoColor=white)


## 🛠️ 세팅 방법

### 라이브러리 설치

1. Eclipse Paho MQTT C++ Client 설치
- [공식 사이트](https://eclipse.dev/paho/)
- `paho-mqttpp3` (C++ wrapper), `paho-mqtt3as` (C 기반 core library) 라이브러리 설치 필요

    1. [vcpkg 설치](https://learn.microsoft.com/ko-kr/vcpkg/get_started/get-started-vs?pivots=shell-powershell)
        ```bash
        git clone https://github.com/microsoft/vcpkg.git
        cd vcpkg
        ```
        vcpkg 빌드 진행
        ```bash
        .\bootstrap-vcpkg.bat
        ```
    2. 명령어를 이용해 `paho-mqttpp3` 설치 (전역으로 경로 세팅 필요)
        ```bash
        .\vcpkg install paho-mqttpp3:x64-windows
        ```
