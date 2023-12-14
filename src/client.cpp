#include "crypto.h"
#include "client.h"

#include <random>

Client::Client(std::string id, const Server& server) 
    : server(&server), id(id) {
    crypto::generate_key(public_key, private_key);
}
/**
 * Retrieves the ID of the client.
 *
 * @return The ID of the client as a string.
 */
std::string Client::get_id() {
    return this->id;
}
/**
 * Returns the public key of the client.
 *
 * @return the public key of the client.
 */
std::string Client::get_publickey() const {
    return this->public_key;
}
/**
 * Retrieves the wallet balance for the client.
 *
 * @return the wallet balance as a double
 *
 * @throws ErrorType a description of the error that can be thrown
 */
double Client::get_wallet() {
    return server->get_wallet(id);
}
/**
 * Signs the given text using the client's private key.
 *
 * @param txt the text to be signed
 *
 * @return the signed text
 *
 * @throws ErrorType if there is an error while signing the text
 */
std::string Client::sign(std::string txt) const {
    return crypto::signMessage(private_key, txt);
}
/**
 * Transfers money to another client.
 *
 * @param receiver the client to transfer money to
 * @param value the amount of money to transfer
 *
 * @return true if the money transfer was successful, false otherwise
 *
 * @throws ErrorType if an error occurs during the transfer process
 */
bool Client::transfer_money(std::string receiver, double value) const {
    std::string trx = id + "-" + receiver + "-" + std::to_string(value);
    return server->add_pending_trx(trx, sign(trx));
}
/**
 * Generates a nonce for the Client class.
 *
 * @return The generated nonce.
 */
size_t Client::generate_nonce() {
    static std::default_random_engine e;
    static std::uniform_int_distribution u(0, INT_MAX);
    return u(e);
}