cmake_minimum_required(VERSION 3.24)
project(calculator C)

set(CMAKE_C_STANDARD 11)

include_directories(include)
include_directories(source)
include_directories(unit_test)

# 根据OS判断需要引入什么与gtk相关的库
if(WIN32)
    set(DEPS gtk+-3.0)
elseif(APPLE)
    set(DEPS gtk+-3.0 glib-2.0)
else()
    set(DEPS gtk+-3.0)
endif()

# 找pkg-config这个库，作用是确定有没有安装pkg-config这个工具
# 如果找不到包，REQUIRED选项将停止处理并给出错误消息。
FIND_PACKAGE(PkgConfig REQUIRED)

# 由上个if可知，${DEPS}可能为数组，所以这里用foreach
foreach(dep ${DEPS})
    # 检查有无${dep}这个库
    PKG_CHECK_MODULES(${dep} REQUIRED ${dep})
    # 加入头文件查找目录
    INCLUDE_DIRECTORIES(${${dep}_INCLUDE_DIRS})
    # 加入动静态库查找目录
    LINK_DIRECTORIES(${${dep}_LIBRARY_DIRS})
endforeach()

# 如果是windows的话，设置release的某个flag
# 才能使可执行文件不是在命令行打开
if(WIN32)
  set(CMAKE_C_FLAGS_RELEASE -mwindows)
endif()

# 将源目录下的所有c文件名取出放到files数组中
file(GLOB files "${CMAKE_CURRENT_SOURCE_DIR}/*.c")
# 将souce目录下的所有c文件名取出放到files数组中
file(GLOB files1 "${CMAKE_CURRENT_SOURCE_DIR}/source/*.c")
# 将unit_test目录下的所有c文件名取出放到files数组中
file(GLOB files2 "${CMAKE_CURRENT_SOURCE_DIR}/unit_test/*.c")
# 将files数组添加到可执行文件中
add_executable(${PROJECT_NAME} ${files} ${files1} ${files2})




# 链接上用到的动静态库
foreach(dep ${DEPS})
    target_link_libraries(${PROJECT_NAME} ${${dep}_LIBRARIES})
endforeach()