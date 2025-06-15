cmake_minimum_required(VERSION 3.18)
project(App LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# --- Qt6 ---
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets LinguistTools)

# --- Исходники ---
set(SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/Source_Files/main.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source_Files/mainwindow.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source_Files/studentitemwidget.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source_Files/pdfgenerator.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source_Files/datamanager.cpp"
)

set(HEADERS
    "${CMAKE_CURRENT_SOURCE_DIR}/Header_Files/mainwindow.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Header_Files/studentitemwidget.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Header_Files/pdfgenerator.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Header_Files/datamanager.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Header_Files/structs.h"
)

# set(UIS
#     ./mainwindow.ui
# )
set(CMAKE_AUTOUIC_SEARCH_PATHS "${CMAKE_CURRENT_SOURCE_DIR}")

# --- Локализация ---
qt_add_translation(TRANSLATION_SRCS i18n/application1_ru_RU.ts)

# --- Добавляем ресурсы ---
qt_add_resources(RESOURCES_RCC resources.qrc)

# --- Исполняемый файл ---
add_executable(app ${SOURCES} ${HEADERS} ${UIS} ${RESOURCES_RCC})

# --- Пути и библиотеки PostgreSQL / libpq ---

if(WIN32)
    set(POSTGRES_INCLUDE_DIR_HINTS "C:/msys64/mingw64/include")
    set(POSTGRES_LIBRARY_DIR_HINTS "C:/msys64/mingw64/lib")
    set(PQXX_INCLUDE_DIR_HINTS "C:/msys64/mingw64/include")
    set(PQXX_LIBRARY_DIR_HINTS "C:/msys64/mingw64/lib")
elseif(APPLE)
    set(POSTGRES_INCLUDE_DIR_HINTS "/opt/homebrew/opt/libpq/include")
    set(POSTGRES_LIBRARY_DIR_HINTS "/opt/homebrew/opt/libpq/lib")
    set(PQXX_INCLUDE_DIR_HINTS "/opt/homebrew/opt/libpqxx/include")
    set(PQXX_LIBRARY_DIR_HINTS "/opt/homebrew/opt/libpqxx/lib")
else()
    set(POSTGRES_INCLUDE_DIR_HINTS "/usr/include" "/usr/local/include")
    set(POSTGRES_LIBRARY_DIR_HINTS "/usr/lib" "/usr/local/lib")
    set(PQXX_INCLUDE_DIR_HINTS "/usr/include" "/usr/local/include")
    set(PQXX_LIBRARY_DIR_HINTS "/usr/lib" "/usr/local/lib")
endif()

find_path(PostgreSQL_INCLUDE_DIR NAMES libpq-fe.h PATHS ${POSTGRES_INCLUDE_DIR_HINTS} REQUIRED)
find_library(PostgreSQL_LIBRARY NAMES pq PATHS ${POSTGRES_LIBRARY_DIR_HINTS} REQUIRED)

find_path(PQXX_INCLUDE_DIR NAMES pqxx/pqxx PATHS ${PQXX_INCLUDE_DIR_HINTS} REQUIRED)
find_library(PQXX_LIBRARY NAMES pqxx PATHS ${PQXX_LIBRARY_DIR_HINTS} REQUIRED)

# --- Qt библиотеки и PostgreSQL / libpq ---
target_include_directories(app PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}
    "${CMAKE_CURRENT_SOURCE_DIR}/Source_Files"
    "${CMAKE_CURRENT_SOURCE_DIR}/Header_Files"
    ${PostgreSQL_INCLUDE_DIR}
    ${PQXX_INCLUDE_DIR}
)

target_link_libraries(app PRIVATE
    Qt6::Core Qt6::Gui Qt6::Widgets
    ${PostgreSQL_LIBRARY}
    ${PQXX_LIBRARY}
)

# --- Установка ---
install(TARGETS app RUNTIME DESTINATION bin)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${TRANSLATION_SRCS} DESTINATION translations)
