#pragma once

#include <boost/asio.hpp>
#include <string>
#include <iostream>
#include "json.hpp"
#include <vector>
#include <memory>

using namespace std;
using net = boost::asio::ip::tcp;
using json = nlohmann::json;

namespace TCP{
    class Server{ // instance working on endless loop
    private:
        json verificationKey = "fjuaydgia3y79e698"; // my custom "verification key"
        vector<string> messages;
        net::acceptor acceptor;
        boost::asio::io_service io_service;

        bool verification(const string& key);
        void manageConnections();
        void handleClient(net::socket socket);

    public:
        Server(short port);
        void runServer();
        void getMessages() const;
        ~Server();
    };
}
