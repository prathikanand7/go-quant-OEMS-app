#pragma once

#include <memory>
#include <string>

#include <drogon/HttpClient.h>

#include "token_manager.h"

enum class OrderType
{
    LIMIT,
    MARKET,
    STOP_LIMIT,
    STOP_MARKET
};

enum class InstrumentType
{
    SPOT,
    FUTURES,
    OPTION
};

struct OrderParams
{
    std::string instrument_name;  // e.g., "BTC-PERPETUAL", "BTC-28JUN24"
    double amount;                // Amount in base currency
    double price;                 // Optional for market orders
    std::string label;            // Client order ID
    OrderType type;               // Order type
    std::string time_in_force;    // "good_til_cancelled", "fill_or_kill" "immediate_or_cancel"
    bool reduce_only{false};      // Optional, for futures/options
    double trigger_price{0};      // Required for stop orders
    bool post_only{false};        // Optional
};

class OrderManager
{
  private:
    static constexpr const char* BASE_URL = "https://test.deribit.com";
    static constexpr const char* API_PATH = "/api/v2/private/buy";
    static constexpr std::chrono::milliseconds TIMEOUT{1000};  // 1 second timeout

    std::shared_ptr<drogon::HttpClient> m_client;
    TokenManager& m_token_manager;

  public:
    OrderManager(TokenManager& token_manager);

    bool PlaceOrder(const std::string& instrument_name, const double& amount, const std::string& label,
                    const std::string& type, std::string& response) const;
};
