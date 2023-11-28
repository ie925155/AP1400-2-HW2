#include "server.h"

Server::Server() {}

std::shared_ptr<Client> Server::add_client(std::string id) {
    return nullptr;
}

std::shared_ptr<Client> Server::get_client(std::string id) {
    return nullptr;
}

double Server::get_wallet(std::string id) {
    return 0.0;
}

bool Server::parse_trx(std::string trx, std::string sender, std::string receiver, double value) {
    return false;
}

bool Server::add_pending_trx(std::string trx, std::string signature) {
    return false;
}

size_t Server::mine() {
    return 0;
}
