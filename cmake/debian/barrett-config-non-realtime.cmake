set(BARRETT_INCLUDE_DIRS /usr/local/include/eigen3;/usr/include/eigen3)
set(BARRETT_DEFINITIONS -D_GNU_SOURCE -D_REENTRANT -Wall -pipe -D__XENO__)
set(BARRETT_LIBRARIES libboost_thread-mt.so;pthread;libboost_python.so;-L/usr/lib;-lgsl;-lgslcblas;-lm;config;config++;libpython2.7.so;barrett)
