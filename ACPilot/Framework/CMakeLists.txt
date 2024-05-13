set(APP_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/DataModule/data_module.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Device/Virtual/virtual_device.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Device/Virtual/Accelerometer/accelerometer.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Device/Virtual/Altimeter/altimeter.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Device/Virtual/Ammeter/ammeter.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Device/Virtual/Barometer/barometer.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Device/Virtual/Coulometer/coulometer.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Device/Virtual/Gyroscope/gyroscope.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Device/Virtual/Locator/locator.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Device/Virtual/Magnetometer/magnetometer.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Device/Virtual/Thermometer/thermometer.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Device/Virtual/Voltmeter/voltmeter.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Device/Physical/Battery/battery.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Device/Physical/Baro/baro.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Device/Physical/physical_device.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Device/Physical/Imu/imu.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Device/Physical/Mag/mag.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Motor/motor.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Mailbox/mailbox_manager.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/MemoryPool/memory_pool_manager.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Thread/thread_manager.cpp
)

if (STM32)
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
else ()
    idf_component_register(SRCS ${APP_SOURCES}
            INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}
            REQUIRES "Common" "Osal" "Utils" "Drivers")
endif ()

