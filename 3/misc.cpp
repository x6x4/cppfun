
class Singleton {

    Singleton () {};

    static Singleton *ptr;

public:

    static Singleton &getInstance () {
        if (!ptr) ptr = new Singleton();
        return *ptr;
    }
};

Singleton *Singleton::ptr = nullptr;