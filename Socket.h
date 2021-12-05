//
// Created by AAI2002 on 05.12.2021.
//

#ifndef HT3_SOCKET_H
#define HT3_SOCKET_H

#include <iostream>
#include <functional>
#include <utility>

namespace my_std {
    using SOCKET = long;

    struct defCloserSocket {
        void (*_deleter_function)(SOCKET *);

        defCloserSocket() : _deleter_function([](SOCKET *ptr) { *ptr = 0; }) {}

        defCloserSocket(void (*deleter)(SOCKET *)) : _deleter_function(deleter) {}

        void operator()(SOCKET *ptr) const { _deleter_function(ptr); }
    };

    template <class deleter = defCloserSocket>
    class SocketGuard {
    private:
        SOCKET _sock;

        deleter closeSocket;

    public:
        SocketGuard(SOCKET socket)
        : closeSocket(), _sock(socket) {}

        SocketGuard(SOCKET socket, deleter del_function)
        : closeSocket(std::move(del_function)), _sock(socket) {}

        operator SOCKET() { return _sock; }

        ~SocketGuard() {
            closeSocket(&_sock);
        }
    };

    struct SocketDeleter {
        void operator()(SOCKET *sock) {
            *sock = 4;
        }
    };

    void SocketCloser(SOCKET *sock) {
        *sock = 2;
    }

    void DemonstrationTask2() {
        // Default deleter
        auto instance1 = SocketGuard(58925);
        instance1.~SocketGuard();
        std::cout << "sizeof:" << sizeof(instance1) << "\t" << instance1 << std::endl;
        // Function pointer deleter
        auto instance2 = SocketGuard(58925, SocketCloser);
        instance2.~SocketGuard();
        std::cout << "sizeof:" << sizeof(instance2) << "\t" << instance2 << std::endl;
        // Lambda deleter
        auto instance3 = SocketGuard<void(*)(SOCKET *)>(58925, [](SOCKET *sock) { *sock = 3; });
        instance3.~SocketGuard();
        std::cout << "sizeof:" << sizeof(instance3) << "\t" << instance3 << std::endl;
        // Functor deleter
        auto instance4 = SocketGuard<SocketDeleter>(58925);
        instance4.~SocketGuard();
        std::cout << "sizeof:" << sizeof(instance4) << "\t" << instance4 << std::endl;
        // std::function
        std::function<void(SOCKET*)> stdFuncDeleter = [](SOCKET *sock) { *sock = 5; };
        auto instance5 = SocketGuard<decltype(stdFuncDeleter)>(58925, stdFuncDeleter);
        instance5.~SocketGuard();
        std::cout << "sizeof:" << sizeof(instance5) << "\t" << instance5 << std::endl;

    }
}

#endif //HT3_SOCKET_H
