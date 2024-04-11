set(APP_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/DataModule/data_module_framework.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/MemoryPool/memory_pool_framework.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Thread/thread_framework.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Mailbox/mailbox_framework.cpp
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
                REQUIRES "Common" "Osal" "Utils" "Drivers")
endif()

