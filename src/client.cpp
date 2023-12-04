#include "crypto.h"
#include "client.h"

Client::Client(std::string id, const Server& server) 
    : server(&server), id(id) {
    crypto::generate_key(public_key, private_key);
}

std::string Client::get_id() {
    return this->id;
}

std::string Client::get_publickey() const {
    return this->public_key;
}

double Client::get_wallet() {
    return server->get_wallet(id);
}

std::string Client::sign(std::string txt) const {
    return crypto::signMessage(private_key, txt);
}

bool Client::transfer_money(std::string receiver, double value) {
    return false;
}

size_t Client::generate_nonce() {
    return 0;
}