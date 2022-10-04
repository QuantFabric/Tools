#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include "PackMessage.hpp"
#include "IPCMarketQueue.hpp"
#include "MarketDataLogger.hpp"

int main(int argc, char *argv[])
{
    unsigned int key = 0XFF000001;
    unsigned int size = 28801;
    int ch;
    bool check = false;
    while ((ch = getopt(argc, argv, "s:k:")) != -1)
    {
        switch (ch)
        {
        case 's':
            size = atol(optarg);
            break;
        case 'k':
            key = strtoul(optarg, NULL, 0);
            break;
        default:
            break;
        }
    }

    Utils::IPCMarketQueue<MarketData::TFutureMarketDataSet> reader(size, key);
    Utils::MarketDataLogger *pMarketDataLogger = Utils::Singleton<Utils::MarketDataLogger>::GetInstance();
    pMarketDataLogger->Init();
    int last = reader.LastTick();
    for (int i = 0; i <= last; ++i)
    {
        MarketData::TFutureMarketDataSet dataset;
        reader.Read(i, dataset);
        pMarketDataLogger->WriteMarketDataSet(dataset);
    }
    printf("LastTick: %d Read Done!\n", last);
    return 0;
}