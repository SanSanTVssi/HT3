//
// Created by AAI2002 on 05.12.2021.
//

#ifndef HT3_DEFAULTDELETER_H
#define HT3_DEFAULTDELETER_H

#include <utility>

namespace my_std {
    template<class T>
    struct defaultDeleter {
        void (*_deleter_function)(T *);

        defaultDeleter() : _deleter_function([](T *ptr) { delete ptr; }) {}

        defaultDeleter(void (*deleter)(T *)) : _deleter_function(std::move(deleter)) {}

        void operator()(T *ptr) { _deleter_function(ptr); }
    };
}


#endif //HT3_DEFAULTDELETER_H
