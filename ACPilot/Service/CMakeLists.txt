set(APP_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/Command/command.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Command/command_parser.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Command/command_server.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Receive/receive_parser.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Receive/receive_server.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Transmit/transmit_server.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/StateMachine/state_machine.cpp
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
                REQUIRES "Common" "Osal" "Utils" "Drivers" "Framework")
endif()

