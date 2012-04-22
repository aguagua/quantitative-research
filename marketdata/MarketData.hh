/**
 * William Wang
 */

/** This implementation uses pthreads, libraries such as boost::thread can be used if installed. 
 */

#include <pthread.h>
#include <vector>
#include <map>

// Mutex class
class Mutex
{
  public:
    Mutex() {}
    virtual ~Mutex() {}
    virtual void lock() { pthread_mutex_lock(&mutex); }
    virtual void unlock() { pthread_mutex_unlock(&mutex); }
    
  private:
    pthread_mutex_t mutex;
};

// Guard class
template <typename T>
class Guard
{
  public:
    explicit Guard(T& t) : mutex(t) { mutex.lock(); }
    ~Guard() { mutex.unlock; }
    
  private:
    T& mutex;
    Guard(const Guard&);
    Guard& operator=(const Guard&);
};

// Market data class
class MarketData
{
  public:
    // Subscribe / unsubscribe functions. Can be called from the different threads at any moment.
    void subscribe(const std::string& symbol, IQuoteListener* listener);
    void unsubscribe(const std::string& symbol, IQuoteListener* listener);
    
    // This function gets called by Price feed handler. It can be called from the different threads.
    // The implementation should notify all subscribers for that symbol
    void onQuote(const char* symbol, double price, double qty);

  private:
    // consts
    static const int MAX_SUB_PER_SYM = 16;
    //static const int PRICE_FEED_THREADS = 10;
    //static const int SUBSCRIBER_THREADS = 16;
    
    // symbol and subscribed listeners
    // 10k symbols with 16 subscribers each
    std::map<const std::string, std::vector<IQuoteListener*> > subSym;
    
    // Mutex object
    Mutex tm;
};

// IQuoteListener class
class IQuoteListener
{
    virtual void onQuote(const std::string& symbol, double price, double qty) = 0;
};
