#Gnomato

Pomodoro Technique timer for GNU/Linux.

![Gnomato](/images/gnomato "Gnomato Interface")

##Requirements

* Boost
* Gtkmm 3.0
* Libnotify
* Python 2.7
* Sqlite3

##Installing

Download the stable tarball [1.0.1](https://github.com/diegorubin/gnomato/releases/download/1.0.1/gnomato-1.0.1.tar.gz)

    tar -zxvf gnomato-1.0.1.tar.gz
    cd gnomato-1.0.1
    ./configure
    make
    sudo make install

##Building

To build the project you need to have installed all development libraries 
of the projects listed in Requirements.

In addition you must have installed the autotools and gettext.

To generate `configure` script and `Makefiles` execute:

    autoreconf -vi

##Contacting

To report bugs send email to rubin.diego[at]gmail.com or
create issues in this github repository.

