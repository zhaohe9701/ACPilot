set(LIBS_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/DataTree/data_tree.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Mail/mailbox.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/MemoryPool/memory_pool.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Tree/ac_tree.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Json/ac_json.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Url/url.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Notify/notify.cpp
        )
if(STM32)
        set(INCLUDES
                ${INCLUDES}
                ${CMAKE_CURRENT_SOURCE_DIR}
                PARENT_SCOPE
                )

        set(SOURCES
                ${SOURCES}
                ${LIBS_SOURCES}
                PARENT_SCOPE
                )
else()
        idf_component_register(SRCS ${LIBS_SOURCES}
                INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}
                REQUIRES "Osal")
endif()