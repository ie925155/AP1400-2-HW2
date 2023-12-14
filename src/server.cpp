
#include <random>
#include <regex>

#include "server.h"
#include "crypto.h"

std::vector<std::string> pending_trxs;

Server::Server() {}
/**
 * Adds a new client to the server with the specified ID.
 *
 * @param id The ID of the client.
 *
 * @return A shared pointer to the newly added client.
 *
 * @throws None
 */
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
/**
 * Retrieves a client with the specified ID from the server.
 *
 * @param id The ID of the client to retrieve.
 *
 * @return A shared pointer to the client with the specified ID, or nullptr if no client is found.
 *
 * @throws None
 */
std::shared_ptr<Client> Server::get_client(std::string id) const {
    for (auto& client : clients) {
        if (client.first->get_id() == id) {
            return client.first;
        }
    }
    return nullptr;
}
/**
 * Gets the wallet balance for a given client ID.
 *
 * @param id the ID of the client
 *
 * @return the wallet balance of the client
 *
 * @throws None
 */
double Server::get_wallet(std::string id) const {
    for (auto& client : clients) {
        if (client.first->get_id() == id) {
            return client.second;
        }
    }
    return 0.0;
}
/**
 * Parses a transaction string and extracts the sender, receiver, and value.
 *
 * @param trx The transaction string to be parsed.
 * @param sender The variable to store the sender.
 * @param receiver The variable to store the receiver.
 * @param value The variable to store the value.
 *
 * @return True if the transaction string is successfully parsed, false otherwise.
 *
 * @throws std::runtime_error if the transaction string does not match the expected format.
 */
bool Server::parse_trx(std::string trx, std::string& sender, std::string& receiver, double& value) {
	std::regex pattern("(\\w+)-(\\w+)-(\\d+\\.\\d+)");
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
/**
 * Adds a pending transaction to the server.
 *
 * @param trx The transaction string to be added.
 * @param signature The signature of the transaction.
 *
 * @return True if the transaction was successfully added, false otherwise.
 *
 * @throws None
 */
bool Server::add_pending_trx(std::string trx, std::string signature) const {
    std::string sender{}, receiver{};
    double value;
    if(!parse_trx(trx, sender, receiver, value)) {
        return false;
    }

    auto psender = get_client(sender);
    auto preceiver = get_client(receiver);
    if (preceiver == nullptr)
        return false;
    if (clients.at(psender) < value)
        return false;
    if(!crypto::verifySignature(psender->get_publickey(), trx, signature))
        return false;
    pending_trxs.push_back(trx);
    return true;
}
/**
 * Mines a block on the server.
 *
 * @return The nonce value generated for the mined block.
 *
 * @throws None
 */
size_t Server::mine() {
    std::string mempool{};
    for (auto& trx : pending_trxs) {
        mempool += trx;
    }
    size_t nonce = 0;
    bool isMined = false;
    while (!isMined) {
        for (auto& [client, balance] : clients) {
            nonce = client->generate_nonce();
            const std::string hash = crypto::sha256(mempool + std::to_string(nonce));
            if (hash.substr(0,10).find("0000") != std::string::npos) {
                balance += 6.25;
                isMined = true;
                for (auto& trx : pending_trxs) {
                    std::string sender{}, receiver{};
                    double value;
                    parse_trx(trx, sender, receiver, value);
                    clients[get_client(sender)] -= value;
                    clients[get_client(receiver)] += value;
                }
                pending_trxs.clear();
                break;
            }
        }
    }
    return nonce;
}
/**
 * Shows the wallets of all clients connected to the server.
 *
 * @param server the server object that contains the list of clients and their wallets
 *
 * @throws ErrorType description of error
 */
void  show_wallets(const  Server& server)
 {
 	std::cout << std::string(20, '*') << std::endl;
 	for(const auto& client: server.clients)
 		std::cout << client.first->get_id() <<  " : "  << client.second << std::endl;
 	std::cout << std::string(20, '*') << std::endl;
 }
