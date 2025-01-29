#include "ServerTCP.h"

namespace TCP{
    Server::Server(short port) : acceptor(io_service, net::endpoint(net::v4(), port)){
        manageConnections();
    }

    // autoryzacja
    bool Server::verification(const string& key)
    {
        json thisKey = json::parse(key);
        return thisKey == verificationKey;
    }

    // przyjmowanie clientów
    void Server::manageConnections() {
        auto socket = make_shared<net::socket>(io_service); // shared ptr do użycia poniżej
        acceptor.async_accept(*socket, /*lambda*/ [this, socket](boost::system::error_code error) {
            if (!error) {
                handleClient(move(*socket));
            }
            manageConnections();
        });
    }

    // Metoda obsługująca klienta
    void Server::handleClient(net::socket socket) {
        try {
            boost::system::error_code error;
            char key[1024];
            size_t temp = socket.read_some(boost::asio::buffer(key), error);  // czytamy raz 

            if (verification(key)) {
                try {
                    char data[1024];
                    while (true)  {
                        size_t długość = socket.read_some(boost::asio::buffer(data), error); // czytamy w nieskończonej pętli while

                        // Obsługa błędów
                        if (error == boost::asio::error::eof){
                            break; // Połączenie zamknięte przez klienta
                        }
                        else if (error)  {
                            throw boost::system::system_error(error);
                        }
                        messages.push_back(string(data, długość));
                    }
                }
                catch (exception& e) {
                    cerr << e.what() << endl;
                }
            }
        }
        catch (exception& e) {
            cerr << e.what() << endl;
        }
    }

    // uruchomienie serwera
    void Server::runServer() {
        io_service.run();
    }

    void Server::getMessages() const {
        for (auto& it : messages)
        {
            json parser = json::parse(it); // parsowanie jsonów
            cout << parser << endl;
        }
    }

    Server::~Server() {
        io_service.stop();
    }
}
