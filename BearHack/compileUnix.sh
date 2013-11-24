#g++ -w -g -Wall -Iinclude main.cpp -o BearHack `pkg-config gtkmm-3.0 --cflags --libs` -lboost_system-mgw45-mt-1_53 -lboost_date_time-mgw45-mt-1_53 -lboost_regex-mgw45-mt-1_53
g++ -w -g -Wall -Iinclude main.cpp -o BearHack `pkg-config gtkmm-3.0 --cflags --libs` -Llib -lboost_system
