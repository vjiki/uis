// This autogenerated skeleton file illustrates how to build a server.
// You should copy it to another filename to avoid overwriting it.

#include <pthread.h>
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/concurrency/ThreadManager.h>
#include <thrift/concurrency/PosixThreadFactory.h>
#include <thrift/server/TNonblockingServer.h>

#include "glog/logging.h"
#include "uid_server_handler.h"
#include "flag.h"

#include "uid_server.h"

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;
using namespace ::apache::thrift::concurrency;

using boost::shared_ptr;

namespace tis {

UidServer::UidServer() {
    _server = NULL;
}

UidServer::~UidServer() {
    if (_server) {
        delete _server;
        _server = NULL;
    }
}

int UidServer::init() {
    //初始化日志
    FLAGS_logbuflevel = -1;
    google::InitGoogleLogging("uis");
    return 0;
}

int UidServer::run() {
    // processor
    shared_ptr<UidServerHandler> handler(new UidServerHandler());
    shared_ptr<TProcessor> processor(new UidServerProcessor(handler));

    // protocol
    shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());

    // thread pool
    shared_ptr<ThreadManager> threadManager = ThreadManager::newSimpleThreadManager(FLAGS_server_thread_num);
    shared_ptr<PosixThreadFactory> threadFactory(new PosixThreadFactory());
    threadManager->threadFactory(threadFactory);
    threadManager->start();

    // server
    _server = new(std::nothrow) TNonblockingServer(processor,
                                                   protocolFactory,
                                                   FLAGS_server_port,
                                                   threadManager);
    if (NULL == _server) {
        LOG(ERROR) << "uis create nonblocking server error";
        return 1;
    }
    _server->serve();

    LOG(INFO) << "uis start ok!\n";
    return 0;
}

void UidServer::stop() {
    if (_server) {
        _server->stop();
    }
}

}
