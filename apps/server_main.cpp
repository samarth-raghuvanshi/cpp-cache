#include "Cache.h"
#include "CommandParser.h"
#include "Server.h"

int main()
{
    Cache cache(1000);

    CommandParser parser(cache);

    Server server(9000, parser);

    server.start();

    return 0;
}