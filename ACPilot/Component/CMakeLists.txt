set(APP_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/Command/DataModule/data_module_command.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Command/Memory/memory_command.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Command/Thread/thread_command.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Command/Mailbox/mailbox_command.cpp
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

