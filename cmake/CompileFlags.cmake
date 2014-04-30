 #--------  SET COMPILER FLAGS
set(CMAKE_CXX_FLAGS_COVERAGE "${CMAKE_CXX_FLAGS_RELEASE} -fprofile-arcs -ftest-coverage")

include(CheckCCompilerFlag)
include(CheckCXXCompilerFlag)
include(CheckCXXSourceCompiles)

set(WITH_CXX11 true)
if("${CMAKE_C_COMPILER}" MATCHES "clang$" OR "${CMAKE_C_COMPILER_ID}" STREQUAL "Clang")
    set(CMAKE_COMPILER_IS_CLANG 1)
endif()

if("${CMAKE_CXX_COMPILER}" MATCHES "clang(\\+\\+)?$" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(CMAKE_COMPILER_IS_CLANGXX 1)
endif()

macro(add_gcc_compiler_cxxflags FLAGS)
      if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_CLANGXX)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${FLAGS}")
      endif()
endmacro(add_gcc_compiler_cxxflags)

macro(add_gcc_compiler_cflags FLAGS)
    if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_CLANG)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${FLAGS}")
    endif()
endmacro(add_gcc_compiler_cflags)

macro(add_gcc_compiler_flags FLAGS)
    add_gcc_compiler_cxxflags("${FLAGS}")
    add_gcc_compiler_cflags("${FLAGS}")
endmacro(add_gcc_compiler_flags)



#add_gcc_compiler_flags("-fno-common -fstack-protector --param=ssp-buffer-size=4 ")
add_gcc_compiler_flags("-Wall -Wextra -Wundef -Wpointer-arith -Wno-long-long")
add_gcc_compiler_flags("-Wformat=2 -Wmissing-format-attribute")
add_gcc_compiler_flags("-fvisibility=hidden")
add_gcc_compiler_cxxflags("-fvisibility-inlines-hidden")

#add_gcc_compiler_cxxflags("-fno-exceptions -fno-rtti")
add_gcc_compiler_cxxflags("-Wnon-virtual-dtor -Wold-style-cast -Woverloaded-virtual")
add_gcc_compiler_cflags("-Wchar-subscripts -Wwrite-strings")

check_c_compiler_flag("-Werror=format-security -Werror=implicit-function-declaration" WERROR_C_AVAILABLE)
check_cxx_compiler_flag("-Werror=format-security" WERROR_CXX_AVAILABLE)

if(WERROR_C_AVAILABLE AND WERROR_CXX_AVAILABLE)
      add_gcc_compiler_flags("-Werror=format-security")
     add_gcc_compiler_cflags("-Werror=implicit-function-declaration")
endif()

if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wcast-align")
endif()

if(CMAKE_COMPILER_IS_GNUCC)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wcast-align")
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
      set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--no-add-needed -Wl,--as-needed -Wl,--no-undefined")
      set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-z,relro")
      set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} -Wl,--no-add-needed -Wl,--as-needed")
      set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} -Wl,-z,relro")
endif()

if(WITH_LTO)
  if(CMAKE_COMPILER_IS_GNUCC AND CMAKE_COMPILER_IS_GNUCXX)
    check_cxx_compiler_flag("-flto -fuse-linker-plugin" LTO_AVAILABLE)

    if(LTO_AVAILABLE)
      add_gcc_compiler_flags("-flto -fuse-linker-plugin")
    else()
      message(FATAL_ERROR "This version of gcc doesn't support LTO")
    endif(LTO_AVAILABLE)
  else()
    message(FATAL_ERROR "LTO is only supported with gcc")
  endif()
endif()

if (WITH_CXX11)
  add_gcc_compiler_cxxflags("-std=c++0x")
  add_gcc_compiler_cflags("-ansi")
else()
  add_gcc_compiler_flags("-ansi")
endif()

if(MINGW)
  set(CMAKE_RC_COMPILER_INIT windres)
  enable_language(RC)
  set(CMAKE_RC_COMPILE_OBJECT "<CMAKE_RC_COMPILER> <FLAGS> -O coff <DEFINES> -i <SOURCE> -o <OBJECT>")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")

# -DQT_NO_KEYWORDS
add_definitions(-DQT_NO_EXCEPTIONS -DQT_NO_STL -DQT_STRICT_ITERATORS -DQT_NO_CAST_TO_ASCII)
