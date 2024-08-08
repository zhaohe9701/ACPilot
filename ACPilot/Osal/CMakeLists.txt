set(OSAL_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/Critical/critical.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Memory/memory.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Semaphore/semaphore.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Thread/thread.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Mutex/mutex.cpp
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