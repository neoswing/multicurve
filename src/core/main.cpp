#include <atomic>
#include <httplib.h>
#include <api/multiserver.h>

std::atomic<bool> daemon_running{true};

void signalHandler(int signal) {
    std::cout << "Received signal " << signal << ", shutting down..." << std::endl;
    daemon_running = false;
}

int main() {

    std::signal(SIGTERM, signalHandler);
    std::signal(SIGINT, signalHandler);

    std::cout << "Daemon starting at " << time(nullptr) << std::endl;
    MultiServer MS;
    MS.start();

    while (daemon_running) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    std::cout << "Stopping server..." << std::endl;
    MS.stop();

    std::cout << "Daemon stopped" << std::endl;
    return 0;
}
