#include "Client.h"
#include "ArduinoConnector.h"

namespace ClientJSON {
    Client::Client(const string& s, const short& t, const string& key)
        : socket(io_service), port(t), ip(s) {
        try {
            // connect to server
            socket.connect(net::endpoint(boost::asio::ip::address::from_string(ip), port)); 
            for (const auto& it : messages) {
                boost::asio::write(socket, boost::asio::buffer(ArduinoReader::serialize(key))); // sends a verification key 
            }
        }
        catch (exception& e) {
            cerr << e.what() << endl;
        }
    }

    void Client::sendMessage() {
        if (!messages.empty()) {
            try{
                // sending each message
                for (const auto& it : messages) {
                    boost::asio::write(socket, boost::asio::buffer(ArduinoReader::serialize(it)));
                }
            }
            catch (exception& e){
                cerr << e.what() << endl;
            }
        }
    }
    
    void Client::setMessages(vector<string> vec) { 
        // preparing messages to send
        copy(vec.begin(), vec.end(), back_inserter(messages));
    }
}
