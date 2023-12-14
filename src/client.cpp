#include "crypto.h"
#include "client.h"

#include <random>

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

bool Client::transfer_money(std::string receiver, double value) const {
    std::string trx = id + "-" + receiver + "-" + std::to_string(value);
    return server->add_pending_trx(trx, sign(trx));
}

size_t Client::generate_nonce() {
    static std::default_random_engine e;
    static std::uniform_int_distribution u(0, INT_MAX);
    return u(e);
}