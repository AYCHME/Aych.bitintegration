#include <iostream>
#include <stdexcept>

#include "easyhttpcpp/EasyHttp.h"
#include "info.hpp"
#include "util.h"
#include "validationinterface.h"

// cache dir = current working dir; cache size = 100 KB
static easyhttpcpp::HttpCache::Ptr pCache = easyhttpcpp::HttpCache::createCache(Poco::Path::current(), 1024 * 100);

// a default http connection pool
static easyhttpcpp::ConnectionPool::Ptr pConnectionPool = easyhttpcpp::ConnectionPool::createConnectionPool();

// configure http cache and connection pool instance (optional but recommended)
static easyhttpcpp::EasyHttp::Builder httpClientBuilder = easyhttpcpp::EasyHttp::Builder().setCache(pCache).setConnectionPool(pConnectionPool);

// notify wallet client insert database
int notify(const std::string &info, const std::string &postfix) {
    int result = 0;
    try{
        // create http client
        easyhttpcpp::EasyHttp::Ptr pHttpClient = httpClientBuilder.build();

        // create a new request and execute synchronously
        easyhttpcpp::RequestBody::Ptr pBody = easyhttpcpp::RequestBody::create(easyhttpcpp::MediaType::Ptr(new easyhttpcpp::MediaType("application/json")), info);
        easyhttpcpp::Request::Builder requestBuilder = easyhttpcpp::Request::Builder().httpPost(pBody);
        std::string url = gArgs.GetArgs("-eospark").front() + postfix;
        easyhttpcpp::Request::Ptr pRequest = requestBuilder.setUrl(url).build();
        easyhttpcpp::Call::Ptr pCall = pHttpClient->newCall(pRequest);
        easyhttpcpp::Response::Ptr pResponse = pCall->execute();

        if (!pResponse->isSuccessful()) {
            std::cout << "HTTP GET Error: (" << pResponse->getCode() << ")" << std::endl;
            result = -1;
        } 
    } catch (const std::exception& e) {
        std::cout << "Error occurred: " << e.what() << std::endl;
        result = -1;
    }

    return result;
}

// handle new block
void blockHandle(const std::shared_ptr<const CBlock> &block, const CBlockIndex *pindex, const std::vector<CTransactionRef>& vtrx){
    notify(blockInfo(*block, *pindex), "/block");
}

// handle new transaction
void trxHandle(const CTransactionRef & vtrx){
    notify(transactionInfo(vtrx), "/transaction");
}


