#pragma once

#include <boost/asio.hpp>
#include <memory>
#include <iostream>
#include <vector>
#include <string>
#include "ArduinoConnector.h"

using namespace std;
using net = boost::asio::ip::tcp;

namespace ClientJSON {
    class Client {
    private:
        boost::asio::io_service io_service;
        net::socket socket;
        string ip;
        short port;
        vector<string> messages;

    public:
        Client(const string& s, const short& t, const string& key);
        void sendMessage();
        void setMessages(vector<string> temp);
    };
}
