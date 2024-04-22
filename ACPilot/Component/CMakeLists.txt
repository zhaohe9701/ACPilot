set(APP_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/AHRS/attitude.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/AHRS/attitude_solver.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/AHRS/Mahony/mahony.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Algorithm/BiasDetect/bias_detect.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Algorithm/EllipsoidFit/ellipsoid_fit.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Algorithm/RotaryTrans/rotary_trans.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Command/Calibrate/calibrate_command.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Command/DataModule/data_module_command.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Command/Memory/memory_command.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Command/Thread/thread_command.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Command/Mailbox/mailbox_command.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Light/Led/led.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Receive/test_input_parser.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Sensor/Imu/tdk_imu.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Sensor/Imu/mpu6X00.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Sensor/Imu/icm20602.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Sensor/Imu/icm20689.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Sensor/Imu/icm20948.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Sensor/Imu/icm42688.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Sensor/Baro/ms5611.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Sensor/Baro/dps310.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Sensor/Mag/qmc5883.cpp
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
                REQUIRES "Common" "Osal" "Utils" "Drivers" "Framework" "Service")
endif()

