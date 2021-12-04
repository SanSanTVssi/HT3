//
// Created by AAI2002 on 05.12.2021.
//

#ifndef HT3_UT_MY_STD_H
#define HT3_UT_MY_STD_H
//#define AllocGuard int
//#define gMemoryLeak 0

// TODO: set to 1 once implemented
#define RUN_TESTS_ON_MY_IMPLEMENTATION 1

#if RUN_TESTS_ON_MY_IMPLEMENTATION
using int_shared_ptr = my_std::shared_ptr<int>;
using int_weak_ptr = my_std::weak_ptr<int>;
#else
using int_shared_ptr = std::shared_ptr<int>;
    using int_weak_ptr = std::weak_ptr<int>;
#endif
void expect(bool val) {
    std::cout << val << std::endl;
}

void ut() {
    auto foo = [] {
        {
            AllocGuard allocGuard;
            int_shared_ptr sharedPtr;
        }
        expect(0 == gMemoryLeak);
    };

    auto foo2 = [] {
        {
            AllocGuard allocGuard;
            int_shared_ptr sharedPtr(new int(42));

        }
        expect(0 == gMemoryLeak);
    };

    auto foo3 = [] {
        {
            AllocGuard allocGuard;
            int_shared_ptr sharedPtr(new int(42));

            expect(42 == *sharedPtr);
        }
        expect(0 == gMemoryLeak);
    };

    auto foo4 = [] {
        intptr_t resourcePtr, getResult = 0;
        {
            AllocGuard allocGuard;
            int* resource = new int(42);
            resourcePtr = reinterpret_cast<intptr_t>(resource);
            int_shared_ptr sharedPtr(resource);
            getResult = reinterpret_cast<intptr_t>(sharedPtr.get());
        }
        expect(resourcePtr == getResult);
        expect(0 == gMemoryLeak);
    };

    auto foo5 = [] {
        intptr_t resourcePtr, getResult = 0;
        {
            AllocGuard allocGuard;
            int* resource = new int(42);
            resourcePtr = reinterpret_cast<intptr_t>(resource);
            int_shared_ptr sharedPtr(resource);
            getResult = reinterpret_cast<intptr_t>(sharedPtr.get());
        }
        expect(resourcePtr == getResult);
        expect(0 == gMemoryLeak);
    };

    auto foo6 = [] {
//        // По стандарту здесь не должно быть проверок на валидность указателя.
//        // Не нужно выполнять дополнительные проверки, выбрасывать исключения в случае если
//        // указатель не инициализирован. Проверки подразумевают оверхед, что неприемлемо
//        // для такого базового объекта как shared_ptr
//
//        {
////            AllocGuard allocGuard;
////            int_shared_ptr sharedPtr;
////            expect(boost::ut::nothrow([&]{
////                sharedPtr.get();
////                sharedPtr.operator->();
////            })) << "doesn't throw";
//        }
//        expect(0 == gMemoryLeak);
    };

    auto foo7 = [] {
        {
            AllocGuard allocGuard;
            int_shared_ptr sharedPtr(new int(42));
            sharedPtr.reset();
        }
        expect(0 == gMemoryLeak);
    };

    auto foo8 = [] {
        {
            AllocGuard allocGuard;

            int_shared_ptr sharedPtr(new int(42));
            int_shared_ptr sharedPtr2 = sharedPtr;
            expect((intptr_t)sharedPtr.get() == (intptr_t)sharedPtr2.get());
        }
        expect(0 == gMemoryLeak);
    };

    auto foo9 = [] {
        {
            AllocGuard allocGuard;

            int_shared_ptr sharedPtr(new int(42));
            sharedPtr = sharedPtr;
            sharedPtr = sharedPtr;
            expect(42 == *sharedPtr);
        }
        expect(0 == gMemoryLeak);
    };

    auto foo10 = [] {
        {
            AllocGuard allocGuard;

            int_shared_ptr sharedPtr(new int(42));
            int_shared_ptr sharedPtr2 = sharedPtr;
            std::swap(sharedPtr, sharedPtr2);

            expect(42 == *sharedPtr);
            expect(42 == *sharedPtr2);
        }
        expect(0 == gMemoryLeak);
    };

    auto foo11 = [] {
        {
            AllocGuard allocGuard;

            int_shared_ptr sharedPtr(new int(42));
            int_shared_ptr sharedPtr2 = sharedPtr;
            int_shared_ptr sharedPtr3 = sharedPtr2;

            expect(42 == *sharedPtr);
            expect(42 == *sharedPtr2);
            expect(42 == *sharedPtr3);
        }
        expect(0 == gMemoryLeak);
    };

    auto foo12 = [] {
        {
            AllocGuard allocGuard;

            int* resource = new int(42);
            int_shared_ptr sharedPtr(resource);
            int_shared_ptr sharedPtr2;
            sharedPtr.swap(sharedPtr2);
            int_shared_ptr sharedPtr3;
            sharedPtr3.swap(sharedPtr2);
            expect((intptr_t)sharedPtr3.get() == (intptr_t)resource);
        }
        expect(0 == gMemoryLeak);
    };

    auto foo13 = [] {
        {
            AllocGuard allocGuard;

            int_shared_ptr sharedPtr(new int(42));
            int_shared_ptr sharedPtr2 = sharedPtr;
            sharedPtr.reset();
            int_shared_ptr sharedPtr3 = sharedPtr2;
            expect(0 == (intptr_t)sharedPtr.get());
            expect(42 == *sharedPtr3.get());
        }
        expect(0 == gMemoryLeak);
    };

    auto foo14 = [] {
        {
            AllocGuard allocGuard;

            int_shared_ptr sharedPtr(new int(42));
            //sharedPtr.reset(new int(4242));
            int_shared_ptr sharedPtr2 = sharedPtr;
            int_shared_ptr sharedPtr3 = sharedPtr2;
            expect(4242 == *sharedPtr2);
            expect(4242 == *sharedPtr3);
        }
        expect(0 == gMemoryLeak);
    };

    auto foo15 = [] {
        {
            AllocGuard allocGuard;

            int_shared_ptr sharedPtr(new int(42));
            int_shared_ptr sharedPtr2 = sharedPtr;
            sharedPtr.reset();
            int_shared_ptr sharedPtr3 = sharedPtr2;
            sharedPtr2.reset();
            expect(!sharedPtr2);
            expect(!sharedPtr);
            expect(42 == *sharedPtr3);
        }
        expect(0 == gMemoryLeak);
    };

    auto foo16 = [] {
        {
            AllocGuard allocGuard;

            int_shared_ptr sharedPtr;

            // No throw
            expect(0 == sharedPtr.use_count());
        }
        expect(0 == gMemoryLeak);
    };

    auto foo17 = [] {
        {
            AllocGuard allocGuard;

            int_shared_ptr sharedPtr(new int(42));
            expect(1 == sharedPtr.use_count());
            int_shared_ptr sharedPtr2 = sharedPtr;
            expect(2 == sharedPtr.use_count());
            expect(2 == sharedPtr2.use_count());
            sharedPtr.reset();
            expect(1 == sharedPtr2.use_count());
            int_shared_ptr sharedPtr3 = sharedPtr2;
            expect(2 == sharedPtr3.use_count());
            sharedPtr2.reset();
            expect(1 == sharedPtr3.use_count());

            int_shared_ptr sharedPtr4;
            std::swap(sharedPtr3, sharedPtr4);

            expect(1 == sharedPtr4.use_count());
        }
        expect(0 == gMemoryLeak);
    };

    auto foo18 = [] {
        {
            int_weak_ptr weakPtr;

            expect(0 == weakPtr.use_count());
            expect(1 == weakPtr.expired());
        }

        expect(0 == gMemoryLeak);
    };

    auto foo19 = [] {
        {
            int_weak_ptr weakPtr;

            int_shared_ptr sharedPtr(new int(42));
            weakPtr = sharedPtr;
            expect(1 == weakPtr.use_count());
            expect(0 == weakPtr.expired());

            sharedPtr.reset();

            expect(0 == weakPtr.use_count());
            expect(1 == weakPtr.expired());
        }

        expect(0 == gMemoryLeak);
    };

    auto foo20 = [] {
        {
            int_weak_ptr weakPtr;

            int_shared_ptr sharedPtr(new int(42));
            weakPtr = sharedPtr;
            weakPtr = weakPtr;
            expect(1 == weakPtr.use_count());
            expect(0 == weakPtr.expired());

            sharedPtr.reset();
            weakPtr = weakPtr;

            expect(0 == weakPtr.use_count());
            expect(1 == weakPtr.expired());
        }

        expect(0 == gMemoryLeak);
    };

    auto foo21 = [] {
        {
            AllocGuard allocGuard;
            int_shared_ptr sharedPtr(new int(42));
            int_weak_ptr weakPtr = sharedPtr;
            expect(1 == weakPtr.use_count());
            expect(1 == sharedPtr.use_count());
            int_shared_ptr sharedPtr2(weakPtr);
            expect(2 == sharedPtr2.use_count());
            expect(42 == *sharedPtr2);
        }
        expect(0 == gMemoryLeak);
    };

    auto foo22 = [] {
        {
            AllocGuard allocGuard;
            int_shared_ptr sharedPtr(new int(42));
            int_weak_ptr weakPtr = sharedPtr;
            expect(1 == weakPtr.use_count());
            expect(1 == sharedPtr.use_count());
            int_shared_ptr sharedPtr2 = weakPtr.lock();
            expect(2 == sharedPtr2.use_count());
            int_weak_ptr weakPtr2 = sharedPtr2;
            expect(2 == sharedPtr2.use_count());
            expect(!weakPtr.expired());
            expect(!weakPtr2.expired());
            expect(42 == *sharedPtr);
            expect(42 == *sharedPtr2);
            sharedPtr.reset();
            expect(1 == weakPtr.use_count());
            sharedPtr2.reset();
            expect(weakPtr.expired());
        }
        expect(0 == gMemoryLeak);
    };
    foo();
    std::cout << "foo" << std::endl;
    foo2();
    std::cout << "foo2" << std::endl;
    foo3();
    std::cout << "foo3" << std::endl;
    foo4();
    std::cout << "foo4" << std::endl;
    foo5();
    std::cout << "foo5" << std::endl;
    foo6();
    std::cout << "foo6" << std::endl;
    foo7();
    std::cout << "foo7" << std::endl;
    foo8();
    std::cout << "foo8" << std::endl;
    foo9();
    std::cout << "foo9" << std::endl;
    foo10();
    std::cout << "foo10" << std::endl;
    foo11();
    std::cout << "foo11" << std::endl;
    foo12();
    std::cout << "foo12" << std::endl;
    foo13();
    std::cout << "foo13" << std::endl;
    foo14();
    std::cout << "foo14" << std::endl;
    foo15();
    std::cout << "foo15" << std::endl;
    foo16();
    std::cout << "foo16" << std::endl;
    foo17();
    std::cout << "foo17" << std::endl;
    foo18();
    std::cout << "foo18" << std::endl;
    foo19();
    std::cout << "foo19" << std::endl;
    foo20();
    std::cout << "foo20" << std::endl;
    foo21();
    std::cout << "foo21" << std::endl;
    foo22();
    std::cout << "foo22" << std::endl;
}
#endif //HT3_UT_MY_STD_H
