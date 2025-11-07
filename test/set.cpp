#include <iostream>
#include <set>

std::ostream& operator<<(std::ostream& os, const std::set<int>& set) {
	for (std::set<int>::iterator it = set.begin(); it != set.end(); it++) {
		os << *it << " ";
	}
	return os;
}

int main(int argc, char **argv) {
	(void) argc;
	(void) argv;

	std::set<int>	set;
	char			command;
	int				key;

	while (std::cin >> command >> key) {
		switch ((int)command) {
			case (int)'i':
				if (set.find(key) != set.end()) {
					std::cerr << "i " << key << ": The key already exists" << std::endl;
					continue;
				}
				set.insert(key);
				break;
			case (int)'d':
				if (set.find(key) == set.end()) {
					std::cerr << "d " << key << ": The key does not exist" << std::endl;
					continue;
				}
				set.erase(key);
				break;
			default:
				std::cerr << "Invalid command: " << command << std::endl;
				return (1);
				break;
		}
		std::cout << set << std::endl;
	}
	return (0);
}