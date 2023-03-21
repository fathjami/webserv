#include "config/Configuration.hpp"
#include "lib/toml/data/table.hpp"
#include "lib/toml/toml.hpp"
#include "request/Request.hpp"
#include "request/parser/HttpParser.hpp"
#include <iostream>
#include <string>
#include "Server.hpp"

using namespace std;




int main(int ac, char** av) {

    std::string config_file = "default.toml";

    if (ac == 2) {
        config_file = av[1];
    }

    toml::table* t = toml::parse_file(config_file);
	if (t == nullptr) {
		std::cerr << "Error parsing file" << std::endl;
		return 1;
	}
        Configuration config(*t);

//		config.print();

		Server server(config);

		server.start();

    delete t;

    return 0;
}

