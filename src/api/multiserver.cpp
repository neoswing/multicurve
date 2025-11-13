#include <api/multiserver.h>
#include <vector>
#include <thread>
#include <atomic>
#include <httplib.h>

const int MAX_NUM_LISTENERS = 16;

class MultiServer::Impl {
private:
    std::vector<std::unique_ptr<httplib::Server>> servers_;
    std::vector<std::thread> server_threads_;
    std::vector<int> server_ports_;
    std::array<std::atomic<int>, MAX_NUM_LISTENERS> counters_;
    std::atomic<bool> running_{false};
    int base_port_;

public:
    Impl(int num_servers = 4, int base_port = 8080){
        base_port_ = base_port;

        if (num_servers > MAX_NUM_LISTENERS) throw std::runtime_error("Too many servers");
        servers_.clear();
        servers_.reserve(num_servers);

        for (int i = 0; i < num_servers; i++) {
            counters_[i].store(0);

            int server_port = base_port_ + i;
            server_ports_.push_back(server_port);

            auto server = std::make_unique<httplib::Server>();
            servers_.push_back(std::move(server));

        }

    }

    bool start() {
        if (running_) return false;

        running_ = true;

        for (size_t i = 0; i < servers_.size(); ++i) {
            int port = base_port_ + i;
            auto& server = servers_[i];
            auto& counter = counters_[i];

            // Setup GET
            server->Get("/api/number", [&counter, i](const httplib::Request& req, httplib::Response& res) {
                int value = ++counter;
                res.set_content(
                    "{\"server_id\": " + std::to_string(i) +
                    ", \"value\": " + std::to_string(value) + "}",
                    "application/json"
                );
            });

            server->Get("/health", [i](const httplib::Request& req, httplib::Response& res) {
                std::cout << "Health check OK" << std::endl;
                res.set_content("{\"status\": \"healthy\", \"server_id\": " + std::to_string(i) + "}", "application/json");
            });

            // Each server in separate thread
            server_threads_.emplace_back([&server, port]() {
                std::cout << "Server listening on port " << port << std::endl;
                server->listen("0.0.0.0", port);
            });
        }

        std::cout << "Started " << servers_.size() << " servers" << std::endl;
        return true;
    }

    void stop() {
        if (!running_) return;

        running_ = false;

        for (auto& server : servers_) {
            if (server) {
                server->stop();
            }
        }

        for (auto& thread : server_threads_) {
            if (thread.joinable()) {
                thread.join();
            }
        }

        server_threads_.clear();
        servers_.clear();
        std::cout << "Load balancer stopped" << std::endl;
    }

    std::vector<int> getPorts() const {
        std::vector<int> ports;
        for (size_t i = 0; i < servers_.size(); ++i) {
            ports.push_back(base_port_ + i);
        }
        return ports;
    }
};



MultiServer::MultiServer(int num_servers, int base_port) : pimpl_(std::make_unique<Impl>(num_servers, base_port)) {}
MultiServer::~MultiServer() = default;  // Must be in .cpp for unique_ptr<Impl>

bool MultiServer::start() { return pimpl_->start(); }
void MultiServer::stop() { pimpl_->stop(); }
