
#include <conio.h>

#include <chrono>
#include <csignal>
#include <iostream>

#include <drogon/drogon.h>

#include "order_manager.h"
#include "utility_manager.h"
#include "web_socket_client.h"

int main()
{
    // Set up signal handler for graceful exit
    std::signal(SIGINT, UtilityManager::HandleExitSignal);

    std::ios_base::sync_with_stdio(false);

    try
    {
        // Initialize TokenManager with access_token, refresh_token, and expiry time
        TokenManager token_manager("access_token.txt", "refresh_token.txt", 2505599);

        // Create the OrderManager with TokenManager
        const OrderManager order_manager(token_manager);

        const OrderParams params{"ETH-PERPETUAL", 2, 2320, "market0000234", OrderType::LIMIT};
        const OrderParams params1{"ETH-PERPETUAL", 2, 2420, "market0000234", OrderType::LIMIT};
        std::string response;

        // Place an order
        order_manager.PlaceOrder(params, "buy", response);    // Open order
        order_manager.PlaceOrder(params1, "sell", response);  // Fill  order
        order_manager.PlaceOrder(params1, "buy", response);   // Open order

        // Modify and cancel orders
        order_manager.ModifyOrder("ETH-14308636889", 4.0, 2200.0, response);
        order_manager.CancelOrder("ETH-14320504994", response);

        // Get order book, positions, and open orders
        order_manager.GetOrderBook("ETH-PERPETUAL", response);
        order_manager.GetCurrentPositions("ETH", "future", response);
        order_manager.GetOpenOrders(response);

        std::cout << "Press any key to exit...\n";

        // Create a WebSocket client and connect to the server
        /*const auto ws_client = std::make_unique<DrogonWebSocket>();
        ws_client->ConnectToServer("ETH-PERPETUAL");*/

        // Start the Drogon event loop in the main thread
        drogon::app().run();
        _getch();
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    }
    return 0;
}
