ZPEngine
===
선린인터넷고등학교 게임 개발 동아리인 Zer0pen에서 사용하는 C++/OpenGL 기반 게임 엔진입니다.

## Requirement
  - Visual Studio 2019 [다운로드](https://www.visualstudio.com/)
  - vcpkg [다운로드](https://github.com/microsoft/vcpkg)
  - vcpkg를 통해 라이브러리 다운로드
```cmd
vcpkg install freeglut freeglut:x64-windows opengl opengl:x64-windows glm glm:x64-windows glfw3 glfw3:x64-windows sndfile sndfile:x64-windows glew glew:x64-windows glad glad:x64-windows openal-soft openal-soft:x64-windows freetype freetype:x64-windows nanovg nanovg:x64-windows stb stb:x64-windows gli gli:x64-windows
```
**Note**: [Tutorial_01.md](/)에서 ZPEngine의 자세한 설치 과정을 확인할 수 있습니다.

## Development Environment     
  1. 이 Git 저장소를 로컬로 복제합니다.
  2. Visual Studio 2019를 통해 ZPEngin/CMakeList파일을 선택하여 준비합니다.
  3. 'CMakeSettings.json'에서 'CMAKE_TOOLCHAIN_FILE'의 value를 자신의 vcpkg 설치 경로로 변환합니다.
  4. 상단 바의 '프로젝트(P) > ZPEngine 구성'을 선택하여 CMake 캐쉬를 생성합니다.
  5. 'x64-Debug' 선택 후 '시작 항목 선택>ZPEngineTest.exe'로 변환 후 빌드를 진행합니다.
  6. 빌드가 오류없이 진행되는지 확인합니다.

**Note**: 이 프로젝트는 기본적으로
`../Lib` 폴더에 라이브러리 파일이 만들어지도록 되어 있습니다.
[zer0pen/ZPEngine-Tutorial](https://github.com/zer0pen/ZPEngine_Tutorial)
저장소를 내려받아 사용하는 것을 권장합니다.

## Tutorial
[zer0pen/ZPEngine-Tutorial](https://github.com/zer0pen/ZPEngine_Tutorial)
저장소에 간단한 여러 가지 예제와 학습을 위한 자료가 있습니다.

## Documentation
[zer0pen/ZPEngine-Tutoria](https://github.com/zer0pen/ZPEngine-Tutorial)
