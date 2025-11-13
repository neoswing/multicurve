#ifndef MULTISERVER_H
#define MULTISERVER_H
#include <memory>

class MultiServer {
private:
    class Impl;
    std::unique_ptr<Impl> pimpl_;
public:
    explicit MultiServer(int num_servers = 4, int base_port = 8080);
    ~MultiServer();

    MultiServer(const MultiServer&) = delete;
    MultiServer& operator=(const MultiServer&) = delete;

    bool start();
    void stop();


};
#endif
