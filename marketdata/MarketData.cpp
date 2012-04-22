/**
 * William Wang
 */

#include "MarketData.hh"

// called by any subscriber thread
void MarketData::subscribe(const std::string& symbol, IQuoteListener* listener)
{    
    Guard<Mutex> g(tm);
    
    // check whether symbol is in the subscribed symbols list
    if(subSym.find(symbol) != subSym.end()) {
        
        // subscribed symbol listeners
        std::vector<IQuoteListener*> subSymList;
        subSymList = subSym(symbol);
                
        // check whether symbol reached maximum subscribers
        if(subSymList.size() < MAX_SUB_PER_SYM) {
            // subscribe successfully, add listener to the subscribed list
            subSymList.push_back(listener);
            std::cout << "Subscribe successful for " << symbol << std::endl;
        } else {
            // maximum subscribers reached, subscribe unsuccessful
            std::cout << "Subscribe unsuccessful, maxium subscribers reached for "
                      << symbol << std::endl;
        }
        
    } else {
        std::vector<IQuoteListener*> subSymList;
        subSymList.push_back(listener);
        subSym.insert(symbol, subSymList);
    }
}

//called by any subscriber thread
void MarketData::unsubscribe(const std::string& symbol, IQuoteListener* listener)
{
    Guard<Mutex> g(tm);
    
    // check that the symbol is subscribed
    if (subSym.find(symbol) != subSym.end()) {
        
        std::vector<IQuoteListener*> subSymList;
        subSymList = subSym(symbol);
        // check that the listener is subscribed
        for (auto iter = subSymList.begin(); iter != subSymList.end(); ++iter) {
            if (sumSymList[iter] == listener) {
                subSymList.erase(iter);
                std::cout << "Unsubscribe successful for " << symbol << std::endl;
                return;
            }
        }
        
        // listener not subscribed
        std::cout << "Unsubscribe failed, listener not in the subscribed list for "
                  << symbol << std::endl;
        
    } else {
        // symbol has not subscribers
        std::cout << symbol << " has no subscribers, can't unsubscribe!" << std::endl;
    }
}

// called by any of the 10 price feed threads and call up to 16 subscriber threads
void MarketData::onQuote(const char* symbol, double price, double qty)
{
    // 10 price feed threads as producers
    std::vector<IQuoteListener*> symSymList;
    subSymList = subSym(symbol);
    
    // 16 subscribers threads as consumers
    int numThreads = subSymList.size();
    pthread_t Thread[numThreads];
    
    typedef struct {
        const char* symbol; 
        double price; 
        double qty;
    } tParameter;
    
    for(auto iter = subSymList.begin(); iter != subSymList.end(); ++iter) {
        // subscriber's onQuote() callback can take 100ms
        // parallel callback
        tParameter parameters = {symbol, price, qty};
        pthread_create(&Thread[--numThreads], NULL, &subSymList[iter]->onQuote, &parameters);
        // sequential callback
        // subSymList[iter]->onQuote(symbol, price, qty);
    }
    
    for(auto iter = subSymList.begin(); iter != subSymList.end(); ++iter) {
        // wait for the threads to finish
        pthread_join(Thread[++numThreads], NULL);        
    }
}
