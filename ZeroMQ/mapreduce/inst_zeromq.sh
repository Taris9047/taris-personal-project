#!/bin/sh -e
echo "ZeroMQ library installer"

# Input is prefix
if [ $# -eq 0 ]; then
	PREFIX=/usr/local
else
	PREFIX=$1
fi

# Check prefix directory permission
if [ -w $PREFIX ]; then
	NEED_SUDO="0"
else
	NEED_SUDO="1"
fi

# URLs
LIBZMQ_URL="git://github.com/zeromq/libzmq.git"
CZMQ_URL="git://github.com/zeromq/czmq.git"

if [ ! -x "$(command -v git)" ]; then
	echo "Without git, we can't do nothing!!"
	exit -1
fi

if [ ! -x "$(command -v libtool)" ]; then
	echo "Need libtool to configure libzmq and czmq!!"
	exit -1
fi

# Let's make a working directory
CURRPWD=$PWD
BLD_DIR=$HOME/.zmq_build

if [ -d $BLD_DIR ]; then
	rm -rf $BLD_DIR
fi
mkdir -p $BLD_DIR
cd $BLD_DIR

# Install libzmq first
git clone $LIBZMQ_URL $BLD_DIR/libzmq
cd $BLD_DIR/libzmq
./autogen.sh
./configure --prefix=$PREFIX --without-docs
make -j 8
if [ $NEED_SUDO == "1" ]; then
	sudo make install
else
	make install
fi

# Return to build dir
cd $BLD_DIR

# then czmq
git clone $CZMQ_URL $BLD_DIR/czmq
cd $BLD_DIR/czmq
./autogen.sh
./configure --prefix=$PREFIX
make -j 8
if [ $NEED_SUDO == "1" ]; then
	sudo make install
else
	make install
fi

# cleanup
cd $CURRPWD
rm -rf $BLD_DIR



