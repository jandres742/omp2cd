#include <iostream>
#include <stdlib.h>
#include <stdint.h>
#include "darts.h"

#include "theCodelets.h"
#include "theTPs.h"

using namespace darts;

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cout << "error need a num" << std::endl;
        return 0;
    }

    int fibnum = atoi(argv[1]);
	int result = 0;

    ThreadAffinity affin(3U, 1U, COMPACT, 0, 0);
    if (affin.generateMask())
    {
        std::cout << "fib(" << fibnum << ")" << std::endl;
        Runtime(&affin).run(launch<mainTP>(fibnum, &result));
    }
	
	std::cout << result << std::endl;

    return 0;
}