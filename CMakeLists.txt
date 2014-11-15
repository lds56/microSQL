cmake_minimum_required(VERSION 2.8.4)
project(microSQL)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(src)


set(SOURCE_FILES
    #src/models/Address.h
    #src/models/Block.h
    #src/models/Block.cpp
    src/models/storage.cpp
    src/models/storage.h
    #src/models/Buffer.h
    #src/models/Index.h
    #src/models/Table.h
    #src/models/Table.cpp
    #src/models/TableRow.h
    #src/models/Condition.h
    #src/models/TableInfo.h
    src/roles/BTree.cpp
    src/roles/BTree.h
    #src/roles/BufferManager.cpp
    #src/roles/BufferManager.h
    #src/roles/CatalogManager.h
    #src/roles/CatalogManager.cpp
    src/roles/IndexManager.h
    src/roles/IndexManager.cpp
    #src/roles/RecordManager.cpp
    #src/roles/RecordManager.h
    #src/utils/API.h
    #src/utils/Interpreter.h
    #src/utils/Request.h
    #src/utils/Response.h
    #src/main.cpp
    #src/test.cpp
    src/B+tree.cpp
    test.tb
    test.index
    README.md)

add_executable(microSQL ${SOURCE_FILES})