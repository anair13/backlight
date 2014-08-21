g++ -w -g -Wall -Iinclude src/main_gtk.cpp -o bin/BearHack `pkg-config gtkmm-3.0 --cflags --libs` -Llib -lboost_system
