#ifndef SERVER_HPP
#define SERVER_HPP

#include <vector>
#include <sys/stat.h>
#include "utils.hpp"
#include "VirtualServer.hpp"
#include "SubServer.hpp"
#include "Configuration.hpp"
#include "Request.hpp"
#include "typedefs.hpp"
#include "HttpParser.hpp"
#include "Response.hpp"
#include "CGIHandler.hpp"
#include <sstream>

#define MSG(msg) (std::string(getBasename((__FILE__)) + ":" + std::string(std::to_string(__LINE__)) + ": " + msg))



/*
 * Server class - This class is responsible for creating a server object
 *
 * @var _virtualServer - vector of VirtualServer objects (one for each virtual server)
 * @var _config - Configuration object that contains all the information from the config file
 *
 * @method Server - constructor that takes a Configuration object as an argument
 * @method ~Server - destructor
 * @method start - method to start the server
 */
class Server {

private:
    virtualServerMap_t      _virtualServer;
    Configuration           _config;
    pollfdsVector_t         _fds;
    errorPagesMap_t         _errorPages;
    Request                 _request;
    string                  _uploadPath;
    stringMap_t             _CGIEnv;
    clientHttpParserMap_t   _clientHttpParserMap;
    stringMap_t             _mimeTypes;
public:
    Server(Configuration config);

    ~Server();

    void start();

    void printData() const;

private:
    void _handleConnections(int fd);
    void _handleRequest(pollfdsVectorIterator_t it);
    void _handleGET(pollfdsVectorIterator_t, const subServersIterator_t &, const Request &);
    void _handlePOST(pollfdsVectorIterator_t, const subServersIterator_t &, const Request &);
    void _handleDELETE(pollfdsVectorIterator_t, const subServersIterator_t &, const Request &);
    void _setupVirtualServer(VirtualServer &vserver);
    void _handleCGI(pollfdsVectorIterator_t, const subServersIterator_t &iter, const Request &request, location_t *pS);
    const string handleFormData(const Request &request, Response &response, stringVector_t error_page);
    const string handleFileUploads(const Request &request, Response &response, const string &uploadPath, stringVector_t error_page);
    void _clearPollfds();
    void _error(const string &msg, int err) const;
    void _setMimeTypes();

    string _getIndexPage(const string&, const stringVector_t&) const;
    string _getErrorPage(int code, stringVector_t error_page) const;

    void _generateIndexPage(const pollfdsVectorIterator_t&, const string&, const string&);

    location_t *matchLocation(const locationVector_t &locations, const string &uri);


    void sendResponse(pollfdsVectorIterator_t it, const string &resourcePath, const Response &response);
    void sendCGIResponse(pollfdsVectorIterator_t it, const Response &response, const string &body);

};
#endif
