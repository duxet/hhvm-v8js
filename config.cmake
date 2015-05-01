#FIND_PATH(V8_INCLUDE_DIR NAMES include/v8.h PATHS /usr/include /usr/local/include)
#include_directories(${V8_INCLUDE_DIR})

include_directories("/home/vagrant/v8" "/home/vagrant/v8/include")

HHVM_EXTENSION(v8js
	src/ext_v8js.cpp
	src/v8js.cpp
)
HHVM_SYSTEMLIB(v8js src/ext_v8js.php)

target_link_libraries(v8js
	"/home/vagrant/v8/out/native/obj.target/tools/gyp/libv8_libplatform.a"
	"/home/vagrant/v8/out/native/lib.target/libv8.so"
	"/home/vagrant/v8/out/native/lib.target/libicuuc.so"
	"/home/vagrant/v8/out/native/lib.target/libicui18n.so"
)
