#include <iostream>
#include <chrono>
#include <thread>
#include <future>

#include <CLI/Error.hpp>
#include <CLI/App.hpp>
#include <CLI/Formatter.hpp>
#include <CLI/Config.hpp>

#include <zmq.hpp>
#include <zmq_addon.hpp>

#include <spdlog/spdlog.h>

#include <concurrentqueue/concurrentqueue.h>

#include <adc/ads1256/ADS1256Reader.h>

using namespace moodycamel;

std::string address{"tcp://0.0.0.0:*"};

std::promise<void> exitSignal;
ConcurrentQueue<adc::SignalValues> q;

void Handler(int signo)
{
    exitSignal.set_value();
    DEV_ModuleExit();
    exit(0);
}

int main(int argc, char **argv)
{
    CLI::App app{"Geophone daemon"};
    app.add_option("-a,--address", address, "Address to listen on");

    spdlog::info("GEOD Initialization!");

    CLI11_PARSE(app, argc, argv);

    adc::ads1256::AnalogDataReader reader;

    zmq::context_t ctx;

    std::future<void> futureObj = exitSignal.get_future();

    std::thread producer([&]() {
        spdlog::info("GEOD Starting reading thread");

        while (futureObj.wait_for(std::chrono::microseconds(10)) == std::future_status::timeout)
        {
            auto values = reader.readData();

            q.enqueue(values);
        }
    });

    auto publisherThread = std::thread([&]() {

        spdlog::info("GEOD Publisher thread start");
        
        //  Prepare publisher
        zmq::socket_t publisher(ctx, zmq::socket_type::pub);

        publisher.bind(address);

        spdlog::info("GEOD bind to {}",address);

        const std::string geod_endpoint = publisher.get(zmq::sockopt::last_endpoint);

        spdlog::info("GEOD is listening on {}", geod_endpoint);

        std::cout.precision(std::numeric_limits< double >::digits10);

        while (true) 
        {
            adc::SignalValues values;

			if (q.try_dequeue(values)) 
            {
                // auto [signal0, signal1, signal2] = values;

                std::array<adc::Signal,3> msg_content{std::get<0>(values), std::get<1>(values), std::get<2>(values)};

                // std::cout << std::fixed << msg_content[0] << ","
                //         << std::fixed << msg_content[1] << ","
                //         << std::fixed << msg_content[2] << std::endl;

                zmq::message_t msg(msg_content.begin(), msg_content.end());
                publisher.send(std::move(msg), zmq::send_flags::none);

                std::this_thread::sleep_for(std::chrono::microseconds(20));
            }
        }

    });

    publisherThread.join();

    spdlog::info("GEOD Finishing!");

    return 0;
}