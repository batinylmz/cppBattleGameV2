cmake_minimum_required(VERSION 3.20)
project(oyunprojesi4)

# SFML 3 en az C++17 standardini gerektirir
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Apple Silicon icin Homebrew kutuphaneleri bulabilmek icin
set(CMAKE_PREFIX_PATH "/opt/homebrew/opt/sfml")

# SFML 3 surumunu zorunlu olarak bul
find_package(SFML 3 COMPONENTS Graphics Window System Audio REQUIRED)

add_executable(oyunprojesi4 main.cpp)

# Modern CMake sozdizimi ile modulleri projeye bagla
target_link_libraries(oyunprojesi4 PRIVATE SFML::Graphics SFML::Window SFML::System SFML::Audio)
