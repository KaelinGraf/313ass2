//
// Created by kaelin on 5/16/25.
//

#include "tsQueue.h"

#include <condition_variable>

template<typename T>
tsQueue<T>::tsQueue() {
    //initialise queue
    q = new queue<T>();
}

template<typename T>
tsQueue<T>::~tsQueue() {
    //pass - need to figure out if "q" needs to be deleted, and if so, how
}


template<typename T>
void tsQueue<T>::queue_push(T item) {
    /*
     * This function pushes an item into the queue. It locks the mutex, pushes the item into the queue,
     * and then unlocks the mutex. It also sets the condition variable to indicate that the queue is not empty.
     * ARGS:
     * item: the item to be pushed into the queue
     * cond: the condition variable to be set to notify a new item is available (if waiting)
     */
    //lock the mutex
    mtx.lock();
    //push the item into the queue
    q.push(item);
    //notify the condition variable to indicate that the queue is not empty
    cv.notify_one();
    //unlock the mutex
    mtx.unlock();
}

template<typename T>
T tsQueue<T>::dequeue() {
    //create a unique lock for the mutex. this is used in the wait function to unlock the mutex.
    // this is a RAII style lock, so it will automatically unlock the mutex when it goes out of scope.
    unique_lock<mutex> lock(mtx);
    //pop the item from the queue if it is not empty
    if (q.empty()) {
        //first evaluates if the queue is empty (with mtx locked). Then, if still empty, atomically unlocks
        //mtx and waits for condition variable to be set.
        cv.wait(lock, [this] {return !q.empty();});
    }
    T item = q.front();
    q.pop();
    return item; //lock falls out of scope causing release.
}