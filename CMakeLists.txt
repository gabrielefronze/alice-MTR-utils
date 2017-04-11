cmake_minimum_required(VERSION 3.6)
project(alice_MTR_utils)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        AliOCDBRun.cpp
        AliOCDBRun.h
        AliRPCAutoIntegrator.cpp
        AliRPCAutoIntegrator.h
        AliRPCData.cpp
        AliRPCData.h
        AliRPCOverflowStatistics.cpp
        AliRPCOverflowStatistics.h
        AliRPCRunStatistics.cpp
        AliRPCRunStatistics.h
        AliRPCValueCurrent.cpp
        AliRPCValueCurrent.h
        AliRPCValueDCS.cpp
        AliRPCValueDCS.h
        AliRPCValueScaler.cpp
        AliRPCValueScaler.h
        AliRPCValueVoltage.cpp
        AliRPCValueVoltage.h
        CompileAllClasses.C)

add_executable(alice_MTR_utils ${SOURCE_FILES})

include_directories(/usr/include
        /Users/Gabriele/alice_sw/sw/osx_x86-64/ROOT/v5-34-30-alice7-1/include
        /Users/Gabriele/alice_sw/sw/osx_x86-64/AliRoot/latest/include
        /Users/Gabriele/alice_sw/sw/osx_x86-64/AliPhysics/latest/include
        /usr/local/opt/libiomp/include/libiomp)