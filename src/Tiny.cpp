#include "NetConnection.h"
#include "RequestManager.h"
#include "Thread.h"
#include "Task.h"
#include "ThreadPool.h"
#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <memory>
#include <mutex>
#include <condition_variable>
using namespace std;

//#define _THREAD_POOL

static const int kItemRepositorySize=10;


namespace
{
int getPortFromCommandLine(char **argv)
{
    return atoi(argv[1]);
}

int getDefalutPort()
{
    return 8080;
}

int getStartPort(int argc, char **argv)
{
    if (argc == 2)
        return getPortFromCommandLine(argv);
    else
        cout<<"Listen 8080 port."<<endl;
    return getDefalutPort();
}

void* threadRun(void *fd)
{
    int connfd=*((int*)fd);
    //pthread_t tid=pthread_self();
    RequestManager(connfd).run();
    close(connfd);
    return 0;
}
}

////�������ߵ�������
//struct ItemRepository {
//    int item_buffer[kItemRepositorySize]; // ��Ʒ������, ��� read_position �� write_position ģ�ͻ��ζ���.
//    size_t read_position; // �����߶�ȡ��Ʒλ��.
//    size_t write_position; // ������д���Ʒλ��.
//    std::mutex mtx; // ������,������Ʒ������
//    std::condition_variable repo_not_full; // ��������, ָʾ��Ʒ��������Ϊ��.
//    std::condition_variable repo_not_empty; // ��������, ָʾ��Ʒ��������Ϊ��.
//} gItemRepository; // ��Ʒ��ȫ�ֱ���, �����ߺ������߲����ñ���.
//
//typedef struct ItemRepository ItemRepository;
//
//
//void ProduceItem(ItemRepository *ir, int item)
//{
//    std::unique_lock<std::mutex> lock(ir->mtx);
//    while(((ir->write_position + 1) % kItemRepositorySize)
//        == ir->read_position) { // item buffer is full, just wait here.
//        std::cout << "Producer is waiting for an empty slot...\n";
//        (ir->repo_not_full).wait(lock); // �����ߵȴ�"��Ʒ�⻺������Ϊ��"��һ��������.
//    }
//
//    (ir->item_buffer)[ir->write_position] = item; // д���Ʒ.
//    (ir->write_position)++; // д��λ�ú���.
//
//    if (ir->write_position == kItemRepositorySize) // д��λ�������ڶ����������������Ϊ��ʼλ��.
//        ir->write_position = 0;
//
//    (ir->repo_not_empty).notify_all(); // ֪ͨ�����߲�Ʒ�ⲻΪ��.
//    lock.unlock(); // ����.
//}
//
//int ConsumeItem(ItemRepository *ir)
//{
//    int data;
//    std::unique_lock<std::mutex> lock(ir->mtx);
//    // item buffer is empty, just wait here.
//    while(ir->write_position == ir->read_position) {
//        std::cout << "Consumer is waiting for items...\n";
//        (ir->repo_not_empty).wait(lock); // �����ߵȴ�"��Ʒ�⻺������Ϊ��"��һ��������.
//    }
//
//    data = (ir->item_buffer)[ir->read_position]; // ��ȡĳһ��Ʒ
//    (ir->read_position)++; // ��ȡλ�ú���
//
//    if (ir->read_position >= kItemRepositorySize) // ��ȡλ�����Ƶ������������λ.
//        ir->read_position = 0;
//
//    (ir->repo_not_full).notify_all(); // ֪ͨ�����߲�Ʒ�ⲻΪ��.
//    lock.unlock(); // ����.
//
//    return data; // ���ز�Ʒ.
//}
//
//
//void ProducerTask() // ����������
//{
//    NetConnection connection;
//    connection.lisen(8080);
//    int connfd;
//    while(1){
//        connfd=connection.accept();
//        ProduceItem(&gItemRepository,connfd);
//    }
//}
//
//void ConsumerTask() // ����������
//{
//    while(1){
//        int item=ConsumeItem(&gItemRepository);
//        RequestManager(item).run();
//        close(item);
//    }
//}
//
//void InitItemRepository(ItemRepository *ir)
//{
//    ir->write_position = 0; // ��ʼ����Ʒд��λ��.
//    ir->read_position = 0; // ��ʼ����Ʒ��ȡλ��.
//}

//�������߶�������
struct ItemRepository {
    int item_buffer[kItemRepositorySize];
    size_t read_position;
    size_t write_position;
    size_t item_counter;
    std::mutex mtx;
    std::mutex item_counter_mtx;
    std::condition_variable repo_not_full;
    std::condition_variable repo_not_empty;
} gItemRepository;

typedef struct ItemRepository ItemRepository;


void ProduceItem(ItemRepository *ir, int item)
{
    std::unique_lock<std::mutex> lock(ir->mtx);
    while(((ir->write_position + 1) % kItemRepositorySize)
        == ir->read_position) { // item buffer is full, just wait here.
        std::cout << "Producer is waiting for an empty slot...\n";
        (ir->repo_not_full).wait(lock);
    }

    (ir->item_buffer)[ir->write_position] = item;
    (ir->write_position)++;

    if (ir->write_position == kItemRepositorySize)
        ir->write_position = 0;

    (ir->repo_not_empty).notify_all();
    lock.unlock();
}

int ConsumeItem(ItemRepository *ir)
{
    int data;
    std::unique_lock<std::mutex> lock(ir->mtx);
    // item buffer is empty, just wait here.
    while(ir->write_position == ir->read_position) {
        std::cout << "Consumer is waiting for items...\n";
        (ir->repo_not_empty).wait(lock);
    }

    data = (ir->item_buffer)[ir->read_position];
    (ir->read_position)++;

    if (ir->read_position >= kItemRepositorySize)
        ir->read_position = 0;

    (ir->repo_not_full).notify_all();
    lock.unlock();

    return data;
}


void ProducerTask()
{
    NetConnection connection;
    connection.lisen(8080);
    int connfd;
    while(1){
        connfd=connection.accept();
        ProduceItem(&gItemRepository,connfd);
    }
}

void ConsumerTask()
{
    while(1) {
        //sleep(1);
        std::unique_lock<std::mutex> lock(gItemRepository.item_counter_mtx);
        int item = ConsumeItem(&gItemRepository);
        RequestManager(item).run();
        lock.unlock();
        close(item);
    }
}

void InitItemRepository(ItemRepository *ir)
{
    ir->write_position = 0;
    ir->read_position = 0;
    ir->item_counter = 0;
}





int main(int argc, char **argv)
{
//    NetConnection connection;
//    //10���̵߳��̳߳�
//    CThreadPool threadPool(10);
//
//    connection.lisen(getStartPort(argc, argv));

//    google::InitGoogleLogging(argv[0]);
//    FLAGS_log_dir="./myLog";
//    FLAGS_logtostderr = false;


    //���߳�
//    #ifndef _THREAD_POOL
//    while (1)
//    {
//        int connfd = connection.accept();
//        RequestManager(connfd).run();
//        connection.close();
//    }
//
//
//    #else
//
//
//    //�̳߳�
////    ThreadPool tp(10);
////    while(1)
////    {
////        CMyTask taskObj;
////        int connfd=connection.accept();
////        LOG(INFO)<<"Connect "<<connfd<<endl;
////        taskObj.SetData(&connfd);
////        threadPool.AddTask(&taskObj);
////    }
//
////    while(1)
////    {
////        cout<<"there are still "<<threadPool.getTaskSize()<<" tasks need to handle"<<endl;
////        if(threadPool.getTaskSize()==0)
////        {
////            if(threadPool.StopAll()==-1)
////            {
////                cout<<"Now I will exit from main"<<endl;
////                exit(4);
////            }
////        }
////        sleep(2);
////    }
//    //�̳߳أ�
//    std::mutex _locker;
//    while(1){
//        int connfd=connection.accept();
//        Task t1(connfd);
//        tp.add_task(&t1);
//    }
//
//    #endif // _THREAD_POOL

//    //�����������ߡ�
//    InitItemRepository(&gItemRepository);
//    thread producer(ProducerTask);
//    thread consumer(ConsumerTask);
//    producer.join();
//    consumer.join();

    //�������߶�������
    InitItemRepository(&gItemRepository);
    std::thread producer(ProducerTask);
    std::thread consumer1(ConsumerTask);
    std::thread consumer2(ConsumerTask);
    std::thread consumer3(ConsumerTask);
    std::thread consumer4(ConsumerTask);

    producer.join();
    consumer1.join();
    consumer2.join();
    consumer3.join();
    consumer4.join();

    return 1;
}
