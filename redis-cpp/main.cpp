#include "redis.h"
#include <iostream>

int main()
{
    Redis redis("127.0.0.1", 6379);

    if (redis.Connect()) {
        std::cout << "Connected to Redis server." << std::endl;

        std::string result = redis.ExecuteCommand("SET key1 value1");
        std::cout << "SET command result: " << result << std::endl;

        result = redis.ExecuteCommand("GET key1");
        std::cout << "GET command result: " << result << std::endl;

        redis.Disconnect();
    }
    else {
        std::cerr << "Failed to connect to Redis server." << std::endl;
    }

    return 0;
}
