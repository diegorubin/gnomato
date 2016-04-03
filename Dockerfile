FROM fedora:23
MAINTAINER rubin.diego@gmail.com

# System dependences 
RUN dnf install -y gcc gtkmm30-devel sqlite-devel python-devel autoconf
RUN dnf install -y gettext automake libnotify-devel intltool make
RUN dnf install -y gcc-c++ boost-devel

# Copy application
RUN mkdir /gnomato

ADD COPYING /gnomato
ADD Doxyfile /gnomato
ADD AUTHORS /gnomato
ADD NEWS /gnomato
ADD po /gnomato/po
ADD doc /gnomato/doc
ADD gnomato.desktop.in /gnomato
ADD Makefile.am /gnomato
ADD README /gnomato
ADD ChangeLog /gnomato
ADD configure.ac /gnomato
ADD README.md /gnomato

ADD data /gnomato/data
ADD src /gnomato/src

WORKDIR /gnomato

RUN autoreconf -vi && ./configure && make && make install
 
