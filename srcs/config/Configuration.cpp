#include "Configuration.hpp"
#include "utils.hpp"
#include <iostream>
#include <sys/_types/_size_t.h>

LocationConfig fill_location(toml::table& location) {
	(void)location;
	LocationConfig l;

	l.prefix = location["prefix"].as_str("/");
	l.root = location["root"].as_str("/var/www/html");
    ITER_FOREACH(vector<toml::table>, location["index"].vec, it) {
        l.index.push_back(it->as_str("default.com"));
    }
    ITER_FOREACH(vector<toml::table>, location["allowed_methods"].vec, it) {
        l.allowed_methods.push_back(it->as_str(""));
    }
	l.autoindex = location["autoindex"].as_str("off");
	l.upload_path = location["upload_path"].as_str("www/html/upload");
	l.upload_store = location["upload_store"].as_str("on");
	l.cgi_path = location["cgi_path"].as_str("www/html/cgi");
	l.cgi_extension = location["cgi_extension"].as_str(".cgi");
	l.cgi_pass = location["cgi_pass"].as_str("");
	l.client_max_body_size = location["_clientMaxBodySize"].as_str("10m");
	l.error_page = location["error_page"].as_str("404 /errors/error-404.html");
	return l;
}

ServerConfig fill_server(toml::table& server) {
	ServerConfig s;
	s.port = server["port"].as_int(s.port);
	s.host = server["host"].as_str("127.0.0.1");

    ITER_FOREACH(vector<toml::table>, server["index"].vec, it) {
        s.index.push_back(it->as_str("default.com"));
    }

	ITER_FOREACH(vector<toml::table>, server["server_name"].vec, it) {
//		s.server_name.push_back(it->as_str("localhost"));
		s.server_name.push_back(it->as_str());
	}
    ITER_FOREACH(vector<toml::table>, server["allowed_methods"].vec, it) {
        s.allowed_methods.push_back(it->as_str(""));
    }
	s.root = server["root"].as_str("www");
	s.error_page = server["error_page"].as_str("404 /errors/error-404.html");
	s.client_max_body_size = server["_clientMaxBodySize"].as_str("10m");

	toml::table& t = server["location"];
	for (size_t i = 0; i < t.vec.size(); i++) {
		s.locations.push_back(fill_location(t[i]));
	}
	return s;
}

Configuration::Configuration(toml::table& config) {

	toml::table& t = config["server"];
	if (t.is_type(toml::table::NONE))
		return;
	for (size_t i = 0; i < t.vec.size(); i++) {
		_servers.push_back(fill_server(t[i]));
	}
}

Configuration::~Configuration() {

}

const vector<ServerConfig> &Configuration::getServers() const {
    return _servers;
}

void Configuration::print() {
	cout << "\n<==================== Configuration ====================>" << endl;

	for (int i = 0; i < _servers.size(); i++)
		_servers[i].print();

	cout << "\n<==================== END Configuration ====================>\n" << endl;
}
