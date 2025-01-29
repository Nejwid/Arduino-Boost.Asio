#include "ArduinoConnector.h"

namespace ArduinoReader{

    unique_ptr<Connector> Connector::instance;
    mutex Connector::mtx;

    Connector::Connector(const string& com): port(usb) {
        try {
            port.open(com); 
            port.set_option(boost::asio::serial_port_base::baud_rate(9600));
        }
        catch (const std::exception& e) {
            std::cerr << "Error opening port: " << e.what() << std::endl;
            throw; 
        }
    }

    Connector& Connector::getInstance(const string& com) {
        lock_guard<mutex> lock(mtx); 
        if (!instance)
            instance.reset(new Connector(com));
        return *instance;
    }

    void Connector::readData(){
        string temp; 
        char c;
        while (true) {
            boost::asio::read(port, boost::asio::buffer(&c, 1));  
            if (c == '\n') {
                data[temp]++;
                logs.push_back(temp);
                break; // łamanie pętli powoduje, że odczytamy tylko do napotkania endline
            }
            else {
                temp.push_back(c);  
            }
        }
    }

    void Connector::displayInfo() const{
        if (!data.empty())
            for (auto& it : data) {
                cout << it.first << " happened total " << it.second << " times" << endl;
            }
        else cout << "read data first\n";
    }

    // metoda do pobierania logów
    vector<string> Connector::getLogs() const {
        return logs;
    }

    Connector::~Connector() {}

    Exe::Exe(const string& com){
        temp = &Connector::getInstance(com);
    }

    // menu do odczytu danych z arduino
    void Exe::read() {
        char UR = ' ';
        cout << "read data from arduino: 1\ndisplay info: 2\n";
        while (UR != 'x') {
            cout << "what we do? ";
            cin >> UR;
            switch (UR) {
                case '1': 
                    temp->readData();
                    break;
                case '2':
                    temp->displayInfo();
                    break;
                default:
                    cout << "invalid input\n";
            }
        }
    }

    Exe::~Exe() {}

    // Funkcja do serializacji
    string serialize(const string& s) {
        json temp = s;
        return temp.dump(4); // return serialized data in json format (easy to send)
    }
}
