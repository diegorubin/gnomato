Gnomato
=======

[![Join the chat at https://gitter.im/diegorubin/gnomato](https://badges.gitter.im/Join%20Chat.svg)](https://gitter.im/diegorubin/gnomato?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)

Pomodoro Technique timer for GNU/Linux.

![Gnomato](http://diegorubin.com/images/gnomato "Gnomato Interface")

## Requirements

* Boost
* Gtkmm 3.0
* Libnotify
* Python 2.7
* Sqlite3

## Installing

Download the stable tarball [1.2.0](https://github.com/diegorubin/gnomato/releases/download/1.2.0/gnomato-1.2.0.tar.gz)

    tar -zxvf gnomato-1.2.0.tar.gz
    cd gnomato-1.2.0
    ./configure
    make
    sudo make install

## Building

To build the project you need to have installed all development libraries 
of the projects listed in Requirements.

In addition you must have installed the autotools and gettext.

To generate `configure` script and `Makefiles` execute:

    autoreconf -vi

## Usage

### Shortcuts

- __Crtl+s__: Start/Stop timer
- __Crtl+a__: Open New Task Dialog 
- __Crtl+r__: Remove selected task
- __Crtl+d__: Finish selected task
- __Crtl+c__: Cancel selected task
- __Crtl+f__: Filter field focus
- __Crtl+l__: Task list focus

## Contacting

To report bugs send email to rubin.diego[at]gmail.com or
create issues in this github repository.

