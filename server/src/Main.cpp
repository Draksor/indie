#include <memory>
#include <vector>

#include <boost/bind.hpp>
#include <boost/asio.hpp>

#include <indie/log/Logger.hpp>

class Session : public std::enable_shared_from_this<Session>
{
public:
    explicit Session(boost::asio::io_service &ios) :
        _sock(ios)
    {}

    void Start()
    {
        //Read from client, make json and send appropriate response
        boost::asio::async_read(_sock, _msg,
                boost::bind(&Session::HandleRead, shared_from_this(),
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred));
    }

    void HandleRead(const boost::system::error_code& error, size_t bytes_transferred) noexcept
    {
        if (error && error != boost::asio::error::eof) {
            _log.Critical(error.message().c_str());
            return;
        }

        _log.Info("New msg");
    }

    auto &GetSocket() { return _sock; }

private:
    boost::asio::ip::tcp::socket _sock;
    boost::asio::streambuf _msg;

    indie::log::Logger _log{"Server"};
};

class Server
{
public:
    using SessionPtr = std::shared_ptr<Session>;
public:
    Server()
    {
        InitSignalHandler();
        SetupListener();
        StartAccept();
        _ios.run();
    }
    ~Server() = default;

private:
    void InitSignalHandler() noexcept
    {
        _sig_set.async_wait([this](auto, auto) {
            this->_log.Info("Stopping server...");
            this->_ios.stop();
            this->_log.Info("Server stopped gracefully.");
        });
    }

    bool SetupListener() noexcept
    {
        boost::system::error_code ec;
        _listener.open(boost::asio::ip::tcp::v4(), ec);
        if (ec)
            return false;
        _listener.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
        _listener.bind(boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 4002), ec);
        if (ec)
            return false;
        _listener.listen(boost::asio::ip::tcp::socket::max_connections, ec);
        if (ec)
            return false;
        return true;
    }

    void StartAccept() noexcept
    {
        auto session = std::make_shared<Session>(_ios);

        _listener.async_accept(session->GetSocket(),
            boost::bind(&Server::HandleAccept, this, session, boost::asio::placeholders::error));
    }

    void HandleAccept(std::shared_ptr<Session> session, const boost::system::error_code& error) noexcept
    {
        if (!error) {
            _log.Info("New client.");
            session->Start();
        }

        StartAccept();
    }

    void OnClientConnected()
    {
        _log.Info("New client connected");
    }

private:
    boost::asio::io_service _ios;
    boost::asio::signal_set _sig_set{_ios, SIGINT, SIGTERM};
    boost::asio::ip::tcp::acceptor _listener{_ios};

    indie::log::Logger _log{"Server"};
};

int main()
{
    Server s;
    return 0;
}