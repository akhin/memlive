#include "../memlive.h"
using namespace memlive;

#include <cstddef>
#include <string>
#include <iostream>
using namespace std;

#if __linux__
#include <unistd.h> // geteuid for sudo check
#endif


class TestHTTPReactor : public HTTPCPRReactor<TestHTTPReactor>
{
    public:

        TestHTTPReactor()
        {
        }

        static inline const char* response_html = R"(
            <!DOCTYPE html>
            <html>
            <head>
                <title>Send Data to Server</title>
            </head>
            <body>
                <form id="myForm">
                    <input type="text" id="myInput" name="data" placeholder="Enter data">
                    <button type="button" id="myButton">Send Data</button>
                </form>

                <script>
                    document.getElementById("myButton").addEventListener("click", function() {
                        var inputValue = document.getElementById("myInput").value;

                        var xhr = new XMLHttpRequest();
                        xhr.open("POST", "http://localhost:555", true);
                        xhr.setRequestHeader("Content-Type", "application/x-www-form-urlencoded");
                        xhr.onreadystatechange = function () 
                        {
                            if (xhr.readyState == 4 && xhr.status == 200) 
                            {
                                console.log(xhr.responseText);
                            }
                        };
                        var data = "data=" + inputValue;
                        xhr.send(data);
                    });
                </script>
            </body>
            </html>
                                         )";

        void on_http_get_request(const HttpRequest& http_request, Socket<SocketType::TCP>* connector_socket)
        {
            std::cout << "GET" << std::endl << std::endl;

            HttpResponse response;
            response.set_response_code_with_text("200 OK");
            response.set_connection_alive(false);
            response.set_body(response_html);
            
            auto response_text = response.get_as_text();
            connector_socket->send(response_text.c_str(), response_text.length());
            std::cout << "Sent response : " << response_text << std::endl;
        }

        void on_http_post_request(const HttpRequest& http_request, Socket<SocketType::TCP>* connector_socket)
        {
            std::cout << "POST request data : " << http_request.body() << std::endl << std::endl;

            HttpResponse response;
            response.set_response_code_with_text("200 OK");
            response.set_connection_alive(false);
            response.set_body(response_html);
            
            std::string response_text = response.get_as_text();

            connector_socket->send(response_text.c_str(), response_text.length());
        }

        void on_http_put_request(const HttpRequest& http_request, Socket<SocketType::TCP>* connector_socket)
        {
        }

        void on_http_delete_request(const HttpRequest& http_request, Socket<SocketType::TCP>* connector_socket)
        {
        }

        void on_client_connected(std::size_t peer_index)
        {
            std::cout << "A client connected , peer index : " << std::to_string(peer_index) << std::endl << std::endl;
        }

        void on_client_disconnected(std::size_t peer_index)
        {
            void on_client_disconnected(std::size_t peer_index);
            std::cout << "A client disconnected , peer index : " << std::to_string(peer_index) << std::endl << std::endl;
            TcpReactor<HTTPCPRReactor, Epoll<>>::on_client_disconnected(peer_index);
        }

    private :
};

int main()
{
    #if __linux__
    if(geteuid() != 0)
    {
        std::cout << "You need to run this test with sudo." << std::endl;
        return -1;
    }
    #endif
    
    TestHTTPReactor test_reactor;
    test_reactor.start("127.0.0.1", 555);

    std::string user_input;
    while (true)
    {
        cout << "Press q to quit" << endl;

        cin >> (user_input);

        if (user_input[0] == 'q')
        {
            break;
        }
    }

    test_reactor.stop();
    return 0;
}