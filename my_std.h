
namespace my_std {
    template<class U, class del>
    class weak_ptr;

    template<class T>
    struct defaultDeleter {
        void (*_deleter_function)(T *);

        defaultDeleter() : _deleter_function([](T *ptr) { delete ptr; }) {}

        defaultDeleter(void (*deleter)(T *)) : _deleter_function(std::move(deleter)) {}

        void operator()(T *ptr) { _deleter_function(ptr); }
    };

    struct CommandBlock {
        unsigned long ReferenceCount;
        unsigned long WeakCount;

        CommandBlock() : ReferenceCount(1), WeakCount(0) {}

        virtual void destroy() = 0;

        virtual ~CommandBlock() = default;

        virtual long use_count() const = 0;
    };

    template<class T, class Deleter = defaultDeleter<T>>
    class shared_ptr {
    private:
        T *data;

        struct CommandBlockImp : public CommandBlock {
            T *data;
            Deleter deleter;

            CommandBlockImp() : CommandBlock(), data(nullptr), deleter(Deleter()) {}

            CommandBlockImp(T *data_ptr, Deleter deleter_ptr)
                    : CommandBlock(), data(data_ptr), deleter(std::move(deleter_ptr)) {}


            void destroy() override { deleter(data); }

            ~CommandBlockImp() override = default;

            [[nodiscard]] long use_count() const override{
                return (data == nullptr) ? 0 : ReferenceCount;
            }
        };

        CommandBlockImp *returnComBlockImp() const{
            if (auto cmi = dynamic_cast<CommandBlockImp *>(ComBlock_ptr)) {
                return cmi;
            } else {
                throw std::runtime_error("Undefined error!");
            }
        }

    public:

        shared_ptr(T *data_ptr = nullptr)
                : ComBlock_ptr(new CommandBlockImp(data_ptr, Deleter())), data(data_ptr) {}

        shared_ptr(T *data_ptr, void (*deleter)(T *))
                : ComBlock_ptr(new CommandBlockImp(data_ptr, deleter)), data(data_ptr) {}

        shared_ptr(const shared_ptr &other) noexcept: ComBlock_ptr(
                other.ComBlock_ptr), data(other.data) { ++other.ComBlock_ptr->ReferenceCount; }

        shared_ptr(const weak_ptr<T, Deleter> &other) : ComBlock_ptr(
                other.ComBlock_ptr), data(nullptr) { ++ComBlock_ptr->ReferenceCount; }

        shared_ptr(shared_ptr<T, Deleter> &&rvalue_other) : ComBlock_ptr(
                rvalue_other.ComBlock_ptr), data(rvalue_other.data) { ++ComBlock_ptr->ReferenceCount; }

        shared_ptr &operator=(const shared_ptr &other) noexcept {
            if (&other == this) {
                return *this;
            }
            ++other.ComBlock_ptr->ReferenceCount;
            ComBlock_ptr = other.ComBlock_ptr;
            return *this;
        }

        shared_ptr &operator=(const shared_ptr &&rvalue_other) noexcept {
            ComBlock_ptr = rvalue_other.ComBlock_ptr;
            return *this;
        }

        [[nodiscard]] T *get() const {
            if (auto cmi = dynamic_cast<CommandBlockImp *>(ComBlock_ptr)) {
                return cmi->data;
            } else {
                return nullptr;
            }
        }

        [[nodiscard]] Deleter getDeleter() const {
            if (auto cmi = dynamic_cast<CommandBlockImp *>(ComBlock_ptr)) {
                return cmi->deleter;
            } else {
                return nullptr;
            }
        }

        T *operator->() const {
            if (auto cmi = dynamic_cast<CommandBlockImp *>(ComBlock_ptr)) {
                return cmi->data;
            } else {
                return nullptr;
            }
        }

        T &operator*() const {
            auto cmi = returnComBlockImp();
            if (!cmi->data) {
                throw std::runtime_error("Error: trying to dereference a null pointer!");
            }
            return *cmi->data;
        }

        void reset(T *ptr = nullptr) noexcept {
            shared_ptr newShr = *this;
            --newShr.ComBlock_ptr->ReferenceCount;
            CommandBlockImp * newCM = new CommandBlockImp;
            newCM->deleter = returnComBlockImp()->deleter;
            newCM->ReferenceCount = 1;
            newCM->WeakCount = 0;
            newCM->data = ptr;
            this->ComBlock_ptr = newCM;
        }

        void reset(T *ptr, Deleter deleter) noexcept {
            returnComBlockImp()->data = ptr;
            returnComBlockImp()->deleter = deleter;
        }

        explicit operator bool() const noexcept {
            if (auto pcm = dynamic_cast<CommandBlockImp *>(ComBlock_ptr)) {
                return pcm->data != nullptr;
            } else {
                return false;
            }
        }

        void swap(shared_ptr &other) noexcept{
            T* ptr = other.get();
            Deleter del = other.getDeleter();
            other.reset(this->get(), del);
            this->reset(ptr, del);
        }

        [[nodiscard]] long use_count() const noexcept { return (returnComBlockImp()->data == nullptr) ? 0 : ComBlock_ptr->ReferenceCount; }

        ~shared_ptr() {
            if (!--ComBlock_ptr->ReferenceCount) {
                ComBlock_ptr->destroy();
                if (!ComBlock_ptr->WeakCount) {
                    delete ComBlock_ptr;
                }
            }
        }

    private:
        CommandBlock *ComBlock_ptr;

        template<class U, class del>
        friend
        class weak_ptr;
    };

    template<class U, class del = defaultDeleter<U>>
    class weak_ptr {
    private:
        CommandBlock *ComBlock_ptr;

        template<class T, class Deleter>
        friend
        class shared_ptr;

    public:
        weak_ptr() : ComBlock_ptr(nullptr) {}

        weak_ptr(const shared_ptr<U, del> &other) : ComBlock_ptr(other.ComBlock_ptr) {
            ++ComBlock_ptr->WeakCount;
        }

        weak_ptr(const weak_ptr &other) : ComBlock_ptr(other.ComBlock_ptr) {
            ++ComBlock_ptr->WeakCount;
        }

        weak_ptr &operator=(const weak_ptr &other) noexcept {
            if (&other != this) {
                ++other.ComBlock_ptr->WeakCount;
                ComBlock_ptr = other.ComBlock_ptr;
                return *this;
            }
        }

        long use_count() const{
            if (ComBlock_ptr != nullptr) {
                return ComBlock_ptr->use_count();
            }
            else {
                return 0;
            }
        }

        weak_ptr &operator=(const shared_ptr<U, del> &other) noexcept {
            ++other.ComBlock_ptr->WeakCount;
            ComBlock_ptr = other.ComBlock_ptr;
            return *this;
        }

        void reset(U *ptr = nullptr) noexcept {
            shared_ptr sharedPtr = shared_ptr(*this);
            sharedPtr.reset(ptr);
        }

        [[nodiscard]] bool expired() const { return use_count() == 0; }

        [[nodiscard]] shared_ptr<U, del> lock() const noexcept {
            return shared_ptr<U, del>(*this);
        }

        ~weak_ptr() {
            if (ComBlock_ptr != nullptr)
            {
                --ComBlock_ptr->WeakCount;
                if (!ComBlock_ptr->WeakCount && !ComBlock_ptr->ReferenceCount) {
                    delete ComBlock_ptr;
                }
            }
        }
    };

    template<class T, class Deleter = defaultDeleter<T>>
    shared_ptr<T, Deleter> make_shared() {
        return shared_ptr<T, Deleter>();
    }

    template<class T, class Deleter = defaultDeleter<T>>
    shared_ptr<T, Deleter> make_shared(T data) {
        return shared_ptr<T, Deleter>(new T(data));
    }
}

