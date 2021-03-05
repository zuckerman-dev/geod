#include <iostream>
#include <chrono>
#include <thread>
#include <future>
#include <fstream>

#include <CLI/Error.hpp>
#include <CLI/App.hpp>
#include <CLI/Formatter.hpp>
#include <CLI/Config.hpp>

#include <zmq.hpp>
#include <zmq_addon.hpp>

#include <spdlog/spdlog.h>

std::string address{"tcp://0.0.0.0:*"};
std::string filename{"test"};
std::ofstream output;

void SubscriberThread(zmq::context_t *ctx) 
{
    spdlog::info("GEOC Subscribe thread start");

    if(!filename.empty())
    {
        spdlog::info("Opening file {}", filename);
        output.open(filename);
    }

    //  Prepare our context and subscriber
    zmq::socket_t subscriber(*ctx, zmq::socket_type::sub);

    spdlog::info("GEOC connecting to {}", address);

    subscriber.connect(address);

    spdlog::info("GEOD Client is {}", (subscriber.handle() != nullptr) ? "connected" : "disconnected");

    subscriber.set(zmq::sockopt::subscribe, "");

    spdlog::info("GEOC subscribe");

    output.precision(std::numeric_limits< double >::digits10);
    
    while (1) 
    {
        zmq::message_t msg;
        
        auto result = subscriber.recv(msg, zmq::recv_flags::none);

        assert(result && "recv failed");
        assert(*result == 2);
        
        double *values = msg.data<double>();

        output << std::fixed << values[0] << ","
                << std::fixed << values[1] << ","
                << std::fixed << values[2] << std::endl;

        std::this_thread::sleep_for(std::chrono::microseconds(20));
    }
}

int main(int argc, char **argv)
{
    CLI::App app{"Geophone client"};
    app.add_option("-a,--address", address, "Address to listen on");
    app.add_option("-o,--output", filename, "Output filename");

    spdlog::info("GEOD Client start");

    CLI11_PARSE(app, argc, argv);

    zmq::context_t ctx;

    auto thread2 = std::async(std::launch::async, SubscriberThread, &ctx);

    thread2.wait();

    spdlog::info("GEOD Client finishing.");

    return 0;
}
