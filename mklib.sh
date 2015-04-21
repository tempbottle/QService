#!/bin/sh
#***********************************************
# File Name  : mklib
# Description: ���������
# Make By    :lqf/200309129@163.com
# Date Time  :2011/06/15 
#***********************************************

UsAge="UsAge:\"./mklib\""

OSName=`uname`
LibPath=`pwd`
LibPath=$LibPath/
echo current path $LibPath

RstPath=Bin
DepLib=DependLib

#����---------------------------����Ϣ---------------------------
cppunit_tar=cppunit-1.12.1.tar.gz
cppunit_path=cppunit-1.12.1

curl_tar=curl-7.38.0.tar.gz
curl_path=curl-7.38.0

libevent_tar=libevent-2.0.22-stable.tar.gz
libevent_path=libevent-2.0.22-stable

#����---------------------------cppunit---------------------------
makecppunit=0
if [ "$OSName" = "Linux" ]
then
    makecppunit=1
fi
if [ "$OSName" = "FreeBSD" ]
then
    makecppunit=1
fi

if [ $makecppunit -eq 1 ]
then
    cd $LibPath$DepLib

    rm -rf $cppunit_path
    tar -xvzf $cppunit_tar
    cd $LibPath$DepLib/$cppunit_path

    ./configure --enable-shared=no --enable-static=yes
    make

    if [ -f "$LibPath$RstPath/libcppunit_$OSName.a" ]
    then
        rm -rf $LibPath$RstPath/libcppunit_$OSName.a
    fi

    mv $LibPath$DepLib/$cppunit_path/src/cppunit/.libs/libcppunit.a $LibPath$RstPath/libcppunit_$OSName.a
fi

#����-----------------libevent---------------------------
cd $LibPath$DepLib

rm -rf $libevent_path
if [ "$OSName" = "SunOS" ]
then
    gzcat $libevent_tar | tar xvf -
else
    tar -xvzf $libevent_tar
fi
cd $LibPath$DepLib/$libevent_path

./configure --enable-shared=no --enable-static=yes
make

if [ -f "$LibPath$RstPath/libevent_core_$OSName.a" ]
then
    rm -rf $LibPath$RstPath/libevent_core_$OSName.a
fi
if [ -f "$LibPath$RstPath/libevent_extra_$OSName.a" ]
then
    rm -rf $LibPath$RstPath/libevent_extra_$OSName.a
fi
if [ -f "$LibPath$RstPath/libevent_pthreads_$OSName.a" ]
then
    rm -rf $LibPath$RstPath/libevent_pthreads_$OSName.a
fi

mv $LibPath$DepLib/$libevent_path/.libs/libevent_core.a $LibPath$RstPath/libevent_core_$OSName.a
mv $LibPath$DepLib/$libevent_path/.libs/libevent_extra.a $LibPath$RstPath/libevent_extra_$OSName.a
mv $LibPath$DepLib/$libevent_path/.libs/libevent_pthreads.a $LibPath$RstPath/libevent_pthreads_$OSName.a

#����-----------------curl---------------------------
cd $LibPath$DepLib

rm -rf $curl_path
if [ "$OSName" = "SunOS" ]
then
    gzcat $curl_tar | tar xvf -
else
    tar -xvzf $curl_tar
fi
cd $LibPath$DepLib/$curl_path

./configure --enable-shared=no --disable-ldap --disable-ldaps
make

if [ -f "$LibPath$RstPath/libcurl_$OSName.a" ]
then
    rm -rf $LibPath$RstPath/libcurl_$OSName.a
fi

mv $LibPath$DepLib/$curl_path/lib/.libs/libcurl.a $LibPath$RstPath/libcurl_$OSName.a