#pragma once

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/ordered_index.hpp>

class WebSocketServer {
     typedef websocketpp::server<websocketpp::config::asio> Server;
     typedef WebSocketServer ThisType;
public:
    WebSocketServer()
    {
        using std::placeholders::_1;
        using std::placeholders::_2;

        server_.init_asio();
        server_.set_open_handler(std::bind(&ThisType::on_open,this,_1));
        server_.set_close_handler(std::bind(&ThisType::on_close,this,_1));
        server_.set_message_handler(std::bind(&ThisType::on_message,this,_1,_2));
        server_.clear_access_channels(websocketpp::log::alevel::all);
    }
    
    void run(uint16_t port)
    {
        server_.set_reuse_addr(true);
        server_.listen(port);
        server_.start_accept();
        server_.run();
    }
    
private:
    struct Entry
    {
        websocketpp::connection_hdl con;
    };
    
    typedef std::owner_less<websocketpp::connection_hdl> HdlCompare;
    
    struct ByCon {};
    typedef boost::multi_index::multi_index_container<
        Entry,
        boost::multi_index::indexed_by<
            boost::multi_index::ordered_unique<boost::multi_index::tag<ByCon>, BOOST_MULTI_INDEX_MEMBER(Entry,websocketpp::connection_hdl,con), HdlCompare>
    >> Connections;

    void on_open(websocketpp::connection_hdl hdl)
    {
        connections_.insert(Entry{hdl});
    }
    
    void on_close(websocketpp::connection_hdl hdl)
    {
        auto found = connections_.get<ByCon>().find(hdl);
        if (found != connections_.get<ByCon>().end())
        {
            connections_.get<ByCon>().erase(found);
        }
    }
    
    void on_message(websocketpp::connection_hdl hdl, Server::message_ptr msg)
    {
        //const auto path = server_.get_con_from_hdl(hdl)->get_uri()->get_resource();
        std::cout << msg->get_payload() << std::endl;
    }
    
    Server server_;
    Connections connections_;
};
