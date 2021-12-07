#include <functional>
#include <memory>
#include "FileWin32ApiGuard.h"

using Socket = long;

struct SocketDeleter {
    void operator()(const Socket *sock) { delete sock; }
};

void SocketCloser(const Socket *sock) { delete sock; }

int main() {
    // Task1
    my_std::FileWin32ApiGuard file = my_std::FileWin32ApiGuard("file.txt");
    // Task2
    std::function<void(Socket*)> stdFuncDeleter = [](Socket *sock) { *sock = 5; };
    // 1
    auto deleter = [](const Socket *sock) { delete sock; };
    std::unique_ptr<Socket, decltype(deleter)> ptr1(new Socket(5934), deleter);
    // 2
    std::unique_ptr<Socket, void(*)(const Socket*)> ptr2(new Socket(5934), SocketCloser);
    // 3
    std::unique_ptr<Socket, SocketDeleter> ptr3(new Socket(5934));
    // 4
    std::function<void(Socket*)> FuncDeleter = [](const Socket *sock) { delete sock; };
    std::unique_ptr<Socket, decltype(stdFuncDeleter)> ptr4(new Socket(5934), FuncDeleter);
}
