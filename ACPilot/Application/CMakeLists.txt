set(APP_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/AircraftState/aircraft_state.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Actuator/Motor/motor.cpp
#        ${CMAKE_CURRENT_SOURCE_DIR}/AHRS/mahony.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Board/board.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Control/AttitudeControl/default_attitude_controller.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Control/pid_core.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Debug/ac_debug.cpp
#        ${CMAKE_CURRENT_SOURCE_DIR}/Protocol/Dshot/dshot.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Protocol/Ibus/ibus.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Protocol/Crsf/crsf.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Remote/remote_channel.cpp
#        ${CMAKE_CURRENT_SOURCE_DIR}/Sensor/Mag/ak09916.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/SysTask/sys_init.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/SysTask/test_task.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Light/light.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Filter/filter.cpp
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

