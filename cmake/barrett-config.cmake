set(BARRETT_INCLUDE_DIRS /usr/local/include;/usr/xenomai/include;/usr/include;/usr/include;/usr/include/eigen3)
set(BARRETT_LIBRARY_DIRS /usr/local/lib;/usr/lib/x86_64-linux-gnu)
set(BARRETT_DEFINITIONS -D_GNU_SOURCE -D_REENTRANT -Wall -Wno-unused-local-typedefs -pipe -D__XENO__)
set(BARRETT_LIBRARIES /usr/lib/x86_64-linux-gnu/libboost_system.so;/usr/lib/x86_64-linux-gnu/libboost_thread.so;/usr/lib/x86_64-linux-gnu/libboost_python.so;/usr/lib/x86_64-linux-gnu/libpthread.so;-L/usr/lib;-lgsl;-lgslcblas;-lm;config;config++;/usr/lib/libnative.so;/usr/lib/libxenomai.so;/usr/lib/librtdm.so;/usr/lib/x86_64-linux-gnu/libpython2.7.so;barrett)
