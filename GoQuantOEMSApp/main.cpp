
#include <conio.h>

#include <chrono>
#include <iostream>

#include <drogon/HttpAppFramework.h>

#include "api_credentials.h"
#include "order_manager.h"

int main()
{
    drogon::app().setLogLevel(trantor::Logger::kDebug);
    try
    {
        // Initialize ApiCredentials
        const ApiCredentials credentials("api_key.txt", "api_secret.txt");

        // Initialize TokenManager with access_token, refresh_token, and expiry time
        TokenManager token_manager("access_token.txt", "refresh_token.txt", 2505599);

        // Create the OrderManager with TokenManager
        const OrderManager order_manager(token_manager);

        static constexpr auto INSTRUMENT = "ETH-PERPETUAL";
        static constexpr double AMOUNT = 40.0;
        static constexpr auto LABEL = "market0000234";
        static constexpr auto TYPE = "market";

        std::cout << "Placing order...\n";
        std::string response;
        // Place an order
        order_manager.PlaceOrder(INSTRUMENT, AMOUNT, LABEL, TYPE, response);

        std::cout << "Response: " << response << '\n';
        std::cout << "Press any key to exit...\n";
        _getch();
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    }
    return 0;
}
