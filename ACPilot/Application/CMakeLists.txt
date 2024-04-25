set(APP_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/AircraftState/aircraft_state.cpp

        ${CMAKE_CURRENT_SOURCE_DIR}/Board/board_esp32_mini.cpp

        ${CMAKE_CURRENT_SOURCE_DIR}/Control/pid_core.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Debug/ac_debug.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Protocol/Ibus/ibus.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Protocol/Crsf/crsf.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Remote/remote_channel.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/SysTask/sys_init.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/SysTask/test_task.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Filter/filter.cpp

        ${CMAKE_CURRENT_SOURCE_DIR}/PoseCalculating/pose_calculating.cpp
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

