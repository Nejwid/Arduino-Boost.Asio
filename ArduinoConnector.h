#pragma once

#include <boost/asio.hpp>
#include <iostream>
#include <unordered_map>
#include <mutex>
#include <memory>
#include <vector>
#include <string>
#include "json.hpp"

using namespace std;
using json = nlohmann::json;

namespace ArduinoReader
{
    class Connector
    {
    private:
        boost::asio::io_service usb;
        boost::asio::serial_port port;
        unordered_map<string, int> data;
        vector<string> logs;

        // singleton part:
        Connector(const string& com);
        static unique_ptr<Connector> instance;
        static mutex mtx;

    public:
        Connector(const Connector&) = delete;
        Connector operator= (const Connector&) = delete;
        static Connector& getInstance(const string& com);

        // koniec singletona
        void readData();
        void displayInfo() const;
        vector<string> getLogs() const;
        ~Connector();
    };

    class Exe { // klasa wykonująca połączenie
    private:
        Connector* temp;
    public:
        Exe(const string& com);
        void read();
        ~Exe();
    };

    string serialize(const string& s); // funkcja do serializacji danych
}
