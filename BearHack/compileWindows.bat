@echo off
g++ -w -g -Wall -Wl,--enable-auto-import -Iinclude main_windows.cpp -o BearHack -lgdi32 -lwsock32 -Llib -lboost_system-mgw45-mt-1_53 -lboost_date_time-mgw45-mt-1_53 -lboost_regex-mgw45-mt-1_53
