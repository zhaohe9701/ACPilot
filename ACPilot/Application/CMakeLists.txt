set(APP_SOURCES

        ${CMAKE_CURRENT_SOURCE_DIR}/Board/board_esp32.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Debug/ac_debug.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Control/control.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Control/AltitudeControl/altitude_control.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Control/PoseControl/pose_control.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Control/PoseCalculating/pose_calculating.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Control/Remote/remote.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Monitor/monitor.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Monitor/Battery/battery_monitor.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/SysTask/sys_init.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/SysTask/test_task.cpp

        ${CMAKE_CURRENT_SOURCE_DIR}/SysTask/task_init.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/SensorCalibrate/sensor_calibrate.cpp
)

if(STM32)
        set(INCLUDES
                ${INCLUDES}
                ${CMAKE_CURRENT_SOURCE_DIR}
                PARENT_SCOPE
                )

        set(SOURCES
                ${SOURCES}
                ${APP_SOURCES}
                PARENT_SCOPE
                )
else()
        idf_component_register(SRCS ${APP_SOURCES}
                INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}
                REQUIRES "Common" "Osal" "Utils" "Drivers" "Framework" "Service" "Component")
endif()

