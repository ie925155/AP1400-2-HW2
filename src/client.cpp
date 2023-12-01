#include "client.h"

Client::Client(std::string id, const Server& server) 
    : server(&server), id(id) {

}

std::string Client::get_id() {
    return this->id;
}

std::string Client::get_publickey() {
    return "";
}

double Client::get_wallet() {
    return server->get_wallet(id);
}

std::string Client::sign(std::string txt) {
    return "";
}

bool Client::transfer_money(std::string receiver, double value) {
    return false;
}

size_t Client::generate_nonce() {
    return 0;
}