find_package(SDL REQUIRED)
find_package(SDL_image REQUIRED) # <- old style package
find_package(CURL REQUIRED)
find_package(zlib-ng REQUIRED)
find_package(libzip REQUIRED)

add_executable(
    apple2

    6821.cpp Applewin.cpp AY8910.cpp CPU.cpp
    Debug.cpp DiskChoose.cpp Disk.cpp DiskFTP.cpp
    DiskImage.cpp Frame.cpp ftpparse.cpp Harddisk.cpp
    Joystick.cpp Keyboard.cpp Log.cpp Memory.cpp
    Mockingboard.cpp MouseInterface.cpp ParallelPrinter.cpp Registry.cpp
    Riff.cpp SaveState.cpp SerialComms.cpp SoundCore.cpp
    Speaker.cpp stretch.cpp Timer.cpp Video.cpp wwrapper.cpp
)

target_include_directories(apple2 PRIVATE ${SDLIMAGE_INCLUDE_DIRS})
target_link_libraries(apple2 SDL::SDL ${SDLIMAGE_LIBRARY} CURL::libcurl zlib-ng::zlib z libzip::zip)
