#include "order_manager.h"

#include <iostream>

#include <drogon/drogon.h>
#include <openssl/sha.h>

OrderManager::OrderManager(TokenManager& token_manager)
    : m_client(drogon::HttpClient::newHttpClient(BASE_URL)), m_token_manager(token_manager)
{
}

bool OrderManager::PlaceOrder(const std::string& instrument_name, const double& amount,
                              const std::string& label, const std::string& type, std::string& response) const
{
    // Check if the access token is still valid
    if (m_token_manager.IsAccessTokenExpired())
    {
        std::cerr << "Access token expired. Refreshing...\n";
        // Refresh the token
        if (!m_token_manager.RefreshAccessToken("YOUR_CLIENT_ID", "YOUR_CLIENT_SECRET"))
        {
            std::cerr << "Failed to refresh token. Cannot place order.\n";
            return false;
        }
    }

    // Use the valid access token
    const std::string access_token = m_token_manager.GetAccessToken();

    // Create the request
    const auto req = drogon::HttpRequest::newHttpRequest();
    req->setMethod(drogon::Get);
    req->setPath(API_PATH);

    const std::string url_params = "?amount=" + std::to_string(amount) +
                                   "&instrument_name=" + instrument_name + "&label=" + label +
                                   "&type=" + type;

    req->setPath(req->path() + url_params);
    req->addHeader("Authorization", "Bearer " + access_token);
    //req->addHeader("Content-Type", "application/json");

    std::cout << "Sending request to: " << req->path() << "\n";

    // Variables to wait for response
    std::mutex mtx;
    std::condition_variable cv;
    bool request_done = false;

    // Send the request
    m_client->sendRequest(
        req,
        [&](const drogon::ReqResult& result, const drogon::HttpResponsePtr& http_response)
        {
            std::lock_guard lock(mtx);

            std::cout << "Callback triggered.\n";
            if (result == drogon::ReqResult::Ok && http_response->getStatusCode() == drogon::k200OK)
            {
                std::cout << "Order placed successfully!\n";
                response = http_response->body();
            }
            else
            {
                std::cerr << "Failed to place the order. HTTP Status Code: " << http_response->getStatusCode()
                          << '\n';
                response = "Failed to place order";
            }
            request_done = true;
            cv.notify_one();
        });
    // Wait for the request to complete
    std::unique_lock lock(mtx);
    std::cout << "Waiting for request completion...\n";  // Add log to track waiting
    cv.wait(lock,
            [&]
            {
                return request_done;
            });
    std::cout << "Request completed.\n";
    return true;
}
