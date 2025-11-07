#include <iostream>
#include "set.hpp"

int main(int argc, char **argv) {
	(void) argc;
	(void) argv;

	ft::set<int>	set;
	char			command;
	int				key;

	while (std::cin >> command >> key) {
		switch ((int)command) {
			case (int)'i':
				if (set.insert(key).second == false) {
					std::cerr << "i " << key << ": The key already exists" << std::endl;
					continue;
				}
				break;
			case (int)'d':
				if (set.erase(key) == 0) {
					std::cerr << "d " << key << ": The key does not exist" << std::endl;
					continue;
				}
				break;
			default:
				std::cerr << "Invalid command: " << command << std::endl;
				return (1);
		}
		std::cout << set << std::endl;
		//for (ft::set<int>::iterator it = set.begin(); it != set.end(); ++it) {
		//	std::cout << *it << " ";
		//}
		//std::cout << std::endl;
	}
	return (0);
}