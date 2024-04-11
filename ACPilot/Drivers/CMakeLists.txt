if(STM32)
        set(DRIVER_SOURCES
                ${CMAKE_CURRENT_SOURCE_DIR}/Interface/com_interface.cpp
                ${CMAKE_CURRENT_SOURCE_DIR}/Interrupt/STM32/interrupt_driver.cpp
                ${CMAKE_CURRENT_SOURCE_DIR}/Gpio/STM32/gpio_driver.cpp
                ${CMAKE_CURRENT_SOURCE_DIR}/Iic/STM32/iic_driver.cpp
                ${CMAKE_CURRENT_SOURCE_DIR}/Pwm/STM32/pwm_driver.cpp
                ${CMAKE_CURRENT_SOURCE_DIR}/Spi/STM32/spi_driver.cpp
                ${CMAKE_CURRENT_SOURCE_DIR}/Uart/STM32/uart_driver.cpp
                ${CMAKE_CURRENT_SOURCE_DIR}/Usb/STM32/usb_driver.cpp
                )
else()
        set(DRIVER_SOURCES
                ${CMAKE_CURRENT_SOURCE_DIR}/Pwm/ESP32/pwm_driver.cpp
                ${CMAKE_CURRENT_SOURCE_DIR}/Gpio/ESP32/gpio_driver.cpp
                ${CMAKE_CURRENT_SOURCE_DIR}/Interrupt/ESP32/interrupt_driver.cpp
                ${CMAKE_CURRENT_SOURCE_DIR}/Spi/ESP32/spi_driver.cpp
                ${CMAKE_CURRENT_SOURCE_DIR}/Uart/ESP32/uart_driver.cpp
                ${CMAKE_CURRENT_SOURCE_DIR}/Usb/ESP32/usb_driver.cpp
                ${CMAKE_CURRENT_SOURCE_DIR}/Iic/ESP32/iic_driver.cpp
                ${CMAKE_CURRENT_SOURCE_DIR}/HardTimer/ESP32/hard_timer_driver.cpp
                ${CMAKE_CURRENT_SOURCE_DIR}/AdConverter/ESP32/ad_converter_driver.cpp
                ${CMAKE_CURRENT_SOURCE_DIR}/Wlan/wlan_driver.cpp
                ${CMAKE_CURRENT_SOURCE_DIR}/Wlan/udp.cpp
                )
endif()

if(STM32)        
        set(INCLUDES
                ${INCLUDES}
                ${CMAKE_CURRENT_SOURCE_DIR}
                PARENT_SCOPE
                )
        
        set(SOURCES
                ${SOURCES}
                ${DRIVER_SOURCES}
                PARENT_SCOPE
                )
else()
        idf_component_register(SRCS ${DRIVER_SOURCES}
                INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}
                REQUIRES "Common" "Utils" "Interface" "driver" "usb" "esp_adc" "esp_timer" "esp_wifi" "nvs_flash")
endif()


