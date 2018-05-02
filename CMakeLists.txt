cmake_minimum_required(VERSION 3.0)
project(MotorBoard)

add_avr_executable(${PROJECT_NAME} main.cpp spi.cpp control.cpp datamanager.cpp protokoll.h engine.cpp capacitor.cpp)
