#include <iostream>
#include <mutex>
#include <memory>

using std::cout;
using std::endl;

template <typename T>
class Singleton
{
public:
    class ReleaseTool
    {
    public:
        ~ReleaseTool()
        {
            Singleton::destroy();
        }
    };

protected:
    Singleton() = default;
    inline static T* s_instance = nullptr;
    inline static std::mutex s_mutex;
public:
    Singleton(const Singleton<T> &) = delete;
    Singleton &operator=(const Singleton<T> &st) = delete;
    static T* getInstance()
    {
        cout << "get instance" << endl;
        if (s_instance != nullptr) {
            return s_instance;
        }
        s_mutex.lock();
        if (s_instance != nullptr) {
            s_mutex.unlock();
            return s_instance;
        }
        cout << "new instance" << endl;
        s_instance = new T();
        static ReleaseTool tool;
        s_mutex.unlock();
        return s_instance;
    }
    static void destroy()
    {
        s_mutex.lock();
        delete s_instance;
        s_instance = nullptr;
        s_mutex.unlock();
    }
    void printAddress()
    {
        cout << s_instance << endl;
    }
    virtual ~Singleton()
    {
        cout << "this is singleton destruct" << endl;
    }
};

class Logic: public Singleton<Logic>
{
    friend class Singleton<Logic>;
private:
    Logic()
    {
        cout << "Logic init" << endl;
    }
    ~Logic() override
    {
        cout << "Logic destroy" << endl;
    }
};

class LogicB: public Singleton<LogicB>
{
    friend class Singleton<LogicB>;
private:
    LogicB()
    {
        cout << "LogicB init" << endl;
    }
    ~LogicB() override
    {
        cout << "LogicB destroy" << endl;
    }
};

void test()
{
    auto a = Logic::getInstance();
    auto aa = LogicB::getInstance();
    a->printAddress();
    aa->printAddress();
    a->destroy();
    auto b = Logic::getInstance();
    b->printAddress();
}

int main()
{
    test();
    cout << "run main" << endl;
    auto c = Logic::getInstance();
    c->printAddress();
    c->destroy();
    auto d = Logic::getInstance();
    d->printAddress();
//    c->destroy();
}