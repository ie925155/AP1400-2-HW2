
#include <random>

#include "server.h"
#include <regex>

Server::Server() {}

std::shared_ptr<Client> Server::add_client(std::string id) {

    for (auto& client : clients) {
        if (get_client(id) != nullptr) {
            std::default_random_engine generator;
            std::uniform_int_distribution<int> distribution(0, 9);
            std::string tail = "";
            for (int i=0; i<4; i++) {
                tail+=std::to_string(distribution(generator));
            }
            id += tail;
        }
    }
    auto client = std::make_shared<Client>(id, *this);
    clients[client] = 5;
    return client;
}

std::shared_ptr<Client> Server::get_client(std::string id) const {
    for (auto& client : clients) {
        if (client.first->get_id() == id) {
            return client.first;
        }
    }
    return nullptr;
}

double Server::get_wallet(std::string id) const {
    for (auto& client : clients) {
        if (client.first->get_id() == id) {
            return client.second;
        }
    }
    return 0.0;
}

bool Server::parse_trx(std::string trx, std::string& sender, std::string& receiver, double& value) {
	std::regex pattern(R"(([a-zA-Z]+)-([a-zA-Z]+)-([\d.]+))");
	std::smatch results;
	if (std::regex_match(trx, results, pattern)) {
		sender = results[1];
		receiver = results[2];
		value = std::stod(results[3]);
		return true;
	} else {
		throw std::runtime_error("trx doesn't match format");
	}
	return false;
}

bool Server::add_pending_trx(std::string trx, std::string signature) {
    return false;
}

size_t Server::mine() {
    return 0;
}

void  show_wallets(const  Server& server)
 {
 	std::cout << std::string(20, '*') << std::endl;
 	for(const auto& client: server.clients)
 		std::cout << client.first->get_id() <<  " : "  << client.second << std::endl;
 	std::cout << std::string(20, '*') << std::endl;
 }
