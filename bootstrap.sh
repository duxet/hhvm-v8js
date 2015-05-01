#!/usr/bin/env bash
locale-gen UTF-8

# Start with installation of HHVM
apt-get -y install software-properties-common git chrpath gdb

apt-key adv --recv-keys --keyserver hkp://keyserver.ubuntu.com:80 0x5a16e7281be7a449
add-apt-repository 'deb http://dl.hhvm.com/ubuntu trusty main'
apt-get -y update
apt-get -y install hhvm hhvm-dev

# Then continue with V8 building
git clone https://chromium.googlesource.com/chromium/tools/depot_tools.git
export PATH=`pwd`/depot_tools:"$PATH"
gclient

fetch v8
cd v8

git checkout 4.2.77.18
gclient sync
make native library=shared -j4

#sudo mkdir -p /usr/lib /usr/include
#cp out/native/lib.target/lib*.so /usr/lib/
#cp -R include/* /usr/include
#echo -e "create /usr/lib/libv8_libplatform.a\naddlib out/native/obj.target/tools/gyp/libv8_libplatform.a\nsave\nend" | sudo ar -M
