#include "web_socket_client.h"

#include <chrono>
#include <iomanip>
#include <iostream>
#include <sstream>

DrogonWebSocket::DrogonWebSocket() = default;

DrogonWebSocket::~DrogonWebSocket()
{
    if (ws_client && is_connected)
    {
        ws_client->stop();
    }
}

std::string DrogonWebSocket::getFormattedTimestamp()
{
    const auto now = std::chrono::system_clock::now();
    const auto now_time = std::chrono::system_clock::to_time_t(now);
    const auto ms =
        std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count() % 1000;

    struct tm timeinfo;
    char timestamp[20];
    localtime_s(&timeinfo, &now_time);
    std::strftime(timestamp, sizeof(timestamp), "%H:%M:%S", &timeinfo);

    std::stringstream ss;
    ss << timestamp << "." << std::setfill('0') << std::setw(3) << ms;
    return ss.str();
}

void DrogonWebSocket::ConnectToServer(const std::string& symbol)
{
    ws_symbol = symbol;

    try
    {
        std::cout << getFormattedTimestamp() << " Connecting to Deribit WebSocket...\n";

        const auto req = drogon::HttpRequest::newHttpRequest();
        req->setPath("/ws/api/v2");
        req->setMethod(drogon::Get);

        ws_client = drogon::WebSocketClient::newWebSocketClient("wss://test.deribit.com");

        ws_client->setMessageHandler(
            [this](std::string&& msg, const drogon::WebSocketClientPtr& ws_ptr,
                   const drogon::WebSocketMessageType& type)
            {
                HandleMessage(std::move(msg), ws_ptr, type);
            });

        const drogon::WebSocketRequestCallback callback =
            [this, symbol](const drogon::ReqResult& result, const drogon::HttpResponsePtr& resp,
                           const drogon::WebSocketClientPtr& ws_conn)
        {
            if (result == drogon::ReqResult::Ok)
            {
                is_connected = true;
                std::cout << getFormattedTimestamp() << " Connected!\n";
                SubscribeToSymbol(symbol);
            }
            else
            {
                std::cerr << getFormattedTimestamp()
                          << " Failed to connect: " << (resp ? std::to_string(resp->getStatusCode()) : "N/A")
                          << "\n";
            }
        };

        ws_client->connectToServer(req, callback);
    }
    catch (const std::exception& e)
    {
        std::cerr << getFormattedTimestamp() << " Exception: " << e.what() << "\n";
    }
}

void DrogonWebSocket::SubscribeToSymbol(const std::string& symbol)
{
    try
    {
        Json::Value msg;
        msg["jsonrpc"] = "2.0";
        msg["method"] = "public/subscribe";
        msg["params"]["channels"] = Json::Value(Json::arrayValue);
        msg["params"]["channels"].append("ticker." + symbol + ".100ms");
        msg["id"] = 0;

        const Json::StreamWriterBuilder writer;
        const std::string msg_str = Json::writeString(writer, msg);
        const drogon::WebSocketConnectionPtr& ws_conn = ws_client->getConnection();
        ws_conn->send(msg_str);
        std::cout << getFormattedTimestamp() << " Subscription request sent for: " << symbol << "\n";
    }
    catch (const std::exception& e)
    {
        std::cerr << getFormattedTimestamp() << " Exception during subscription: " << e.what() << "\n";
    }
}

void DrogonWebSocket::HandleMessage(std::string&& msg, const drogon::WebSocketClientPtr& ws_ptr,
                                    const drogon::WebSocketMessageType& type)
{
    try
    {
        if (type == drogon::WebSocketMessageType::Text)
        {
            Json::Value json_data;
            const Json::CharReaderBuilder reader_builder;
            std::string errs;
            std::istringstream s(msg);

            if (Json::parseFromStream(reader_builder, s, &json_data, &errs))
            {
                if (json_data.isMember("params"))
                {
                    const auto& params = json_data["params"];
                    if (params.isMember("channel") && params.isMember("data"))
                    {
                        std::cout << getFormattedTimestamp() << " " << params["channel"].asString() << "\n";
                    }
                }
            }
            else
            {
                std::cerr << getFormattedTimestamp() << " Failed to parse message: " << errs << "\n";
            }
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << getFormattedTimestamp() << " Exception processing message: " << e.what() << "\n";
    }
}
