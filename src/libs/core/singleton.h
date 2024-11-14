#ifndef SINGLETON_H
#define SINGLETON_H

#define SINGLETON_INSTANCE(CLASS) \
    static CLASS* instance() { \
        static CLASS i; \
        return &i; \
    }

template <class T>
class Singleton
{
protected:
    Singleton() {}
    virtual ~Singleton() {}

public:
    Singleton(const Singleton&) = delete;
    Singleton& operator= (const Singleton) = delete;

    static T* instance()
    {
        static T instance;
        return &instance;
    }
};

#endif // SINGLETON_H
