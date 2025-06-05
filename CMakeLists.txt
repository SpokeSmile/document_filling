cmake_minimum_required(VERSION 3.18)
project(App LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# --- Qt6 ---
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets LinguistTools)

# --- Опция: использовать FetchContent для libpqxx (только на Windows) ---
if(WIN32)
    set(USE_FETCH_LIBPQXX ON)
else()
    set(USE_FETCH_LIBPQXX OFF)
endif()

if(USE_FETCH_LIBPQXX)
    include(FetchContent)
    FetchContent_Declare(
      libpqxx
      GIT_REPOSITORY https://github.com/jtv/libpqxx.git
      GIT_TAG        main
    )
    FetchContent_MakeAvailable(libpqxx)
endif()

# --- Исходники ---
set(SOURCES
    src/main.cpp
    src/mainwindow.cpp
    src/studentitemwidget.cpp
    src/pdfgenerator.cpp
    # src/syncBD.cpp
)

set(HEADERS
    src/mainwindow.h
    src/studentitemwidget.h
    src/pdfgenerator.h
    # src/syncBD.h
    src/structs.h
)

set(UIS
    src/mainwindow.ui
)

# --- Локализация ---
qt_add_translation(TRANSLATION_SRCS i18n/application1_ru_RU.ts)

# --- Исполняемый файл ---
add_executable(app ${SOURCES} ${HEADERS} ${UIS})

# --- Qt библиотеки ---
target_link_libraries(app PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets)

# --- Подключаемые директории ---
target_include_directories(app PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# --- Пути и библиотеки PostgreSQL / libpq ---

if(WIN32)
    set(POSTGRES_INCLUDE_DIR_HINTS "C:/Program Files/PostgreSQL/include")
    set(POSTGRES_LIBRARY_DIR_HINTS "C:/Program Files/PostgreSQL/lib")
elseif(APPLE)
    set(POSTGRES_INCLUDE_DIR_HINTS "/opt/homebrew/opt/libpq/include")
    set(POSTGRES_LIBRARY_DIR_HINTS "/opt/homebrew/opt/libpq/lib")
else()
    set(POSTGRES_INCLUDE_DIR_HINTS "/usr/include" "/usr/local/include")
    set(POSTGRES_LIBRARY_DIR_HINTS "/usr/lib" "/usr/local/lib")
endif()

find_path(PostgreSQL_INCLUDE_DIR NAMES libpq-fe.h PATHS ${POSTGRES_INCLUDE_DIR_HINTS} REQUIRED)
find_library(PostgreSQL_LIBRARY NAMES pq PATHS ${POSTGRES_LIBRARY_DIR_HINTS} REQUIRED)

target_include_directories(app PRIVATE ${PostgreSQL_INCLUDE_DIR})
target_link_libraries(app PRIVATE ${PostgreSQL_LIBRARY})

# --- libpqxx ---

if(NOT USE_FETCH_LIBPQXX)
    if(WIN32)
        set(PQXX_INCLUDE_DIR_HINTS "C:/Program Files/libpqxx/include")
        set(PQXX_LIBRARY_DIR_HINTS "C:/Program Files/libpqxx/lib")
    elseif(APPLE)
        set(PQXX_INCLUDE_DIR_HINTS "/opt/homebrew/opt/libpqxx/include")
        set(PQXX_LIBRARY_DIR_HINTS "/opt/homebrew/opt/libpqxx/lib")
    else()
        set(PQXX_INCLUDE_DIR_HINTS "/usr/include" "/usr/local/include")
        set(PQXX_LIBRARY_DIR_HINTS "/usr/lib" "/usr/local/lib")
    endif()

    find_path(PQXX_INCLUDE_DIR NAMES pqxx/pqxx PATHS ${PQXX_INCLUDE_DIR_HINTS} REQUIRED)
    find_library(PQXX_LIBRARY NAMES pqxx PATHS ${PQXX_LIBRARY_DIR_HINTS} REQUIRED)

    target_include_directories(app PRIVATE ${PQXX_INCLUDE_DIR})
    target_link_libraries(app PRIVATE ${PQXX_LIBRARY})
else()
    # Если FetchContent — libpqxx добавляется автоматически:
    target_link_libraries(app PRIVATE pqxx)
endif()

# --- Установка ---
install(TARGETS app RUNTIME DESTINATION bin)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${TRANSLATION_SRCS} DESTINATION translations)
