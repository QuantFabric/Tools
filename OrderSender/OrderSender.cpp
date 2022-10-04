#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <string.h>
#include "PackMessage.hpp"
#include "YMLConfig.hpp"
#include "IPCLockFreeQueue.hpp"
#include "Util.hpp"

int main(int argc, char *argv[])
{
    unsigned int key = 0XFF000002;
    int ch;
    std::string ConfigPath;
    while ((ch = getopt(argc, argv, "k:f:")) != -1)
    {
        switch (ch)
        {
        case 'k':
            key = strtoul(optarg, NULL, 0);
            break;
        case 'f':
            ConfigPath = optarg;
            break;
        default:
            break;
        }
    }
    Utils::OrderParameter Config;
    std::string errorString;
    if(!Utils::LoadOrderParameter(ConfigPath.c_str(), Config, errorString))
    {
        printf("LoadOrderParameter %s failed, %s\n", ConfigPath.c_str(), errorString.c_str());
    }

    Utils::IPCLockFreeQueue<Message::PackMessage, 1000> OrderChannelQueue;
    OrderChannelQueue.Init(key);
    OrderChannelQueue.RegisterChannel(Config.Account.c_str());
    OrderChannelQueue.ResetChannel(Config.Account.c_str());
    Message::TOrderRequest OrderRequest;
    memset(&OrderRequest, 0, sizeof(OrderRequest));
    strncpy(OrderRequest.Account, Config.Account.c_str(), sizeof(OrderRequest.Account));
    strncpy(OrderRequest.Ticker, Config.Ticker.c_str(), sizeof(OrderRequest.Ticker));
    strncpy(OrderRequest.ExchangeID, Config.ExchangeID.c_str(), sizeof(OrderRequest.ExchangeID));
    OrderRequest.OrderType = Config.OrderType;
    OrderRequest.Direction = Config.Direction;
    OrderRequest.Offset = Config.Offset;
    OrderRequest.Price = Config.Price;
    OrderRequest.Volume = Config.Volume;
    OrderRequest.RiskStatus = Message::ERiskStatusType::EPREPARE_CHECKED;
    Message::PackMessage message;
    message.MessageType = Message::EMessageType::EOrderRequest;
    memcpy(&message.OrderRequest, &OrderRequest, sizeof(message.OrderRequest));
    
    for(int i = 0; i < Config.Count; i++)
    {
        strncpy(message.OrderRequest.SendTime, Utils::getCurrentTimeUs(), sizeof(message.OrderRequest.SendTime));
        if(!OrderChannelQueue.Push(Config.Account.c_str(), message))
        {
            printf("Push OrderRequest to OrderChannelQueue: 0X%X failed, Queue is full.\n", key);
        }
        usleep(Config.TimeOut * 1000);
    }
    
    return 0;
}