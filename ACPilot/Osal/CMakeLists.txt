set(OSAL_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/Action/ac_action.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Critical/ac_critical.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Memory/ac_memory.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Semaphore/ac_semaphore.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Thread/ac_thread.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Mutex/ac_mutex.cpp
        )
if(STM32)
        set(INCLUDES
                ${INCLUDES}
                ${CMAKE_CURRENT_SOURCE_DIR}
                PARENT_SCOPE
                )

        set(SOURCES
                ${SOURCES}
                ${OSAL_SOURCES}
                PARENT_SCOPE
                )
else()
        idf_component_register(SRCS ${OSAL_SOURCES}
                INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}
                REQUIRES "Common")
endif()