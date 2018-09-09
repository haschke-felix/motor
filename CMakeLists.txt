cmake_minimum_required(VERSION 3.0)
project(MotorBoard)

include_directories("$ENV{HOME}/Software/common/PortPin")
add_avr_executable(${PROJECT_NAME} main.cpp spi.cpp control.cpp datamanager.cpp protokoll.h engine.cpp)
