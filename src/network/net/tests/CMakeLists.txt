add_executable(test_eventLoopThread EventLoopThreadTest.cpp)
target_link_libraries(test_eventLoopThread network base)

add_executable(AcceptorTest AcceptorTest.cpp)
target_link_libraries(AcceptorTest network base)

add_executable(InetAddressTest InetAddressTest.cpp)
target_link_libraries(InetAddressTest network base)

add_executable(TcpConnectionTest TcpConnectionTest.cpp)
target_link_libraries(TcpConnectionTest network base)

add_executable(SocketOptTest SocketOptTest.cpp)
target_link_libraries(SocketOptTest network base)

add_executable(DnsServerTest DnsServerTest.cpp)
target_link_libraries(DnsServerTest network base)

add_executable(TcpClientTest TcpClientTest.cpp)
target_link_libraries(TcpClientTest network base)

add_executable(TcpServerTest TcpServerTest.cpp)
target_link_libraries(TcpServerTest network base)


add_executable(UdpClientTest UdpClientTest.cpp)
target_link_libraries(UdpClientTest network base)

add_executable(UdpServerTest UdpServerTest.cpp)
target_link_libraries(UdpServerTest network base)