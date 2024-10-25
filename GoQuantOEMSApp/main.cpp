
#include <conio.h>

#include <chrono>
#include <iostream>

#include <drogon/drogon.h>

#include "order_manager.h"
#include "web_socket_client.h"

int main()
{
    std::ios_base::sync_with_stdio(false);

    try
    {
        //WebSocketClient ws_client;

        //// Connect to WebSocket and subscribe
        //ws_client.connect();
        //ws_client.subscribeToTicker("ETH-PERPETUAL",
        //                            [](const std::string& data)
        //                            {
        //                                std::cout << "Real-time ticker data: " << data << std::endl;
        //                            });

        //// Run WebSocket in a separate thread
        //std::thread ws_thread(
        //    [&ws_client]()
        //    {
        //        ws_client.run();
        //    });

        // Initialize ApiCredentials
        //const ApiCredentials credentials("api_key.txt", "api_secret.txt");

        // Initialize TokenManager with access_token, refresh_token, and expiry time
        TokenManager token_manager("access_token.txt", "refresh_token.txt", 2505599);

        // Create the OrderManager with TokenManager
        const OrderManager order_manager(token_manager);

        const OrderParams params{"ETH-PERPETUAL", 1, 2300, "market0000234", OrderType::LIMIT};

        std::cout << "Placing order...\n";
        std::string response;

        // Place an order
        order_manager.PlaceOrder(params, "buy", response);
        order_manager.PlaceOrder(params, "buy", response);
        std::cout << "Response: " << response << '\n';
        response = "";
        //order_manager.CancelOrder("ETH-14308180751", response);
        std::cout << "Response: " << response << '\n';
        //order_manager.ModifyOrder("ETH-14308180751", 4.0, 2200.0, response);
        std::cout << "Response: " << response << '\n';
        //order_manager.GetOrderBook("ETH-PERPETUAL", response);
        std::cout << "Response: " << response << '\n';
        //order_manager.GetCurrentPositions("ETH", "future", response);
        std::cout << "Response: " << response << '\n';
        std::cout << "Press any key to exit...\n";
        _getch();
        //ws_thread.join();
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    }
    return 0;
}
