#pragma once

#define RESET   "\033[0m"
#define BLUE    "\033[34m"

#define dlog(x) std::cout << BLUE << #x << " = " << (x) << RESET << std::endl