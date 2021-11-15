#include <iostream>
#include <fmt/format.h>
#include <Core/IMidnightEngine.h>

int main(int argc, char** argv) {

    Logger logger = Logger("New");
    logger.error("Error: {}", 5);
	return 0;

}
