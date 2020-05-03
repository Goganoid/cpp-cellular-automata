//#ifndef CONCURRENT_THREADPOOL_H
//#define CONCURRENT_THREADPOOL_H
//
//#include <atomic>
//#include <thread>
//#include <mutex>
//#include <array>
//#include <list>
//#include <functional>
//#include <condition_variable>
//namespace nbsdx {
//    namespace concurrent {
//
///**
// *  Simple ThreadPool that creates `ThreadCount` threads upon its creation,
// *  and pulls from a queue to get new jobs. The default is 10 threads.
// *
// *  This class requires a number of c++11 features be present in your compiler.
// */
//        template <unsigned ThreadCount = 10>
//        class ThreadPool {
//
//            std::array<std::thread, ThreadCount> threads;
//            std::list<std::function<void(void)>> queue;
//
//            std::atomic_int         jobs_left;
//            std::atomic_bool        bailout;
//            std::atomic_bool        finished;
//            std::condition_variable job_available_var;
//            std::condition_variable wait_var;
//            std::mutex              wait_mutex;
//            std::mutex              queue_mutex;
//
//            /**
//             *  Take the next job in the queue and run it.
//             *  Notify the main thread that a job has completed.
//             */
//            void Task() {
//                while( !bailout ) {
//                    next_job()();
//                    --jobs_left;
//                    wait_var.notify_one();
//                }
//            }
//
//            /**
//             *  Get the next job; pop the first item in the queue,
//             *  otherwise wait for a signal from the main thread.
//             */
//            std::function<void(void)> next_job() {
//                std::function<void(void)> res;
//                std::unique_lock<std::mutex> job_lock( queue_mutex );
//
//                // Wait for a job if we don't have any.
//                job_available_var.wait( job_lock, [this]() ->bool { return queue.size() || bailout; } );
//
//                // Get job from the queue
//                if( !bailout ) {
//                    res = queue.front();
//                    queue.pop_front();
//                }
//                else { // If we're bailing out, 'inject' a job into the queue to keep jobs_left accurate.
//                    res = []{};
//                    ++jobs_left;
//                }
//                return res;
//            }
//
//        public:
//            ThreadPool()
//                    : jobs_left( 0 )
//                    , bailout( false )
//                    , finished( false )
//            {
//                for( unsigned i = 0; i < ThreadCount; ++i )
//                    threads[ i ] = std::thread( [this]{ this->Task(); } );
//            }
//
//            /**
//             *  JoinAll on deconstruction
//             */
//            ~ThreadPool() {
//                JoinAll();
//            }
//
//            /**
//             *  Get the number of threads in this pool
//             */
//            inline unsigned Size() const {
//                return ThreadCount;
//            }
//
//            /**
//             *  Get the number of jobs left in the queue.
//             */
//            inline unsigned JobsRemaining() {
//                std::lock_guard<std::mutex> guard( queue_mutex );
//                return queue.size();
//            }
//
//            /**
//             *  Add a new job to the pool. If there are no jobs in the queue,
//             *  a thread is woken up to take the job. If all threads are busy,
//             *  the job is added to the end of the queue.
//             */
//            void AddJob( std::function<void(void)> job ) {
//                std::lock_guard<std::mutex> guard( queue_mutex );
//                queue.emplace_back( job );
//                ++jobs_left;
//                job_available_var.notify_one();
//            }
//
//            /**
//             *  Join with all threads. Block until all threads have completed.
//             *  Params: WaitForAll: If true, will wait for the queue to empty
//             *          before joining with threads. If false, will complete
//             *          current jobs, then inform the threads to exit.
//             *  The queue will be empty after this call, and the threads will
//             *  be done. After invoking `ThreadPool::JoinAll`, the pool can no
//             *  longer be used. If you need the pool to exist past completion
//             *  of jobs, look to use `ThreadPool::WaitAll`.
//             */
//            void JoinAll( bool WaitForAll = true ) {
//                if( !finished ) {
//                    if( WaitForAll ) {
//                        WaitAll();
//                    }
//
//                    // note that we're done, and wake up any thread that's
//                    // waiting for a new job
//                    bailout = true;
//                    job_available_var.notify_all();
//
//                    for( auto &x : threads )
//                        if( x.joinable() )
//                            x.join();
//                    finished = true;
//                }
//            }
//
//            /**
//             *  Wait for the pool to empty before continuing.
//             *  This does not call `std::thread::join`, it only waits until
//             *  all jobs have finshed executing.
//             */
//            void WaitAll() {
//                if( jobs_left > 0 ) {
//                    std::unique_lock<std::mutex> lk( wait_mutex );
//                    wait_var.wait( lk, [this]{ return this->jobs_left == 0; } );
//                    lk.unlock();
//                }
//            }
//        };
//
//    } // namespace concurrent
//} // namespace nbsdx
//
//#endif //CONCURRENT_THREADPOOL_H
#ifndef CONCURRENT_THREADPOOL_H
#define CONCURRENT_THREADPOOL_H

#include <thread>
#include <mutex>
#include <functional>
#include <condition_variable>
#include <queue>
#include <vector>

/**
 *  Simple ThreadPool that creates `threadCount` threads upon its creation,
 *  and pulls from a queue to get new jobs.
 *
 *  This class requires a number of c++11 features be present in your compiler.
 */
class ThreadPool final
{
public:
    explicit ThreadPool(const int threadCount) :
            _jobsLeft(0),
            _isRunning(true)
    {
        _threads.reserve(threadCount);
        for (int index = 0; index < threadCount; ++index)
        {
            _threads.emplace_back([&]
                                  {
                                      /**
                                      *  Take the next job in the queue and run it.
                                      *  Notify the main thread that a job has completed.
                                      */
                                      do
                                      {
                                          std::function<void()> job;

                                          // scoped lock
                                          {
                                              std::unique_lock<std::mutex> lock(_queueMutex);

                                              // Wait for a job if we don't have any.
                                              _jobAvailableVar.wait(lock, [&]
                                              {
                                                  return !_queue.empty();
                                              });

                                              // Get job from the queue
                                              job = _queue.front();
                                              _queue.pop();
                                          }

                                          job();

                                          // scoped lock
                                          {
                                              std::lock_guard<std::mutex> lock(_jobsLeftMutex);
                                              --_jobsLeft;
                                          }

                                          _waitVar.notify_one();
                                      }
                                      while (_isRunning);
                                  });
        }
    }

    /**
     *  JoinAll on deconstruction
     */
    ~ThreadPool()
    {
        JoinAll();
    }

    /**
     *  Add a new job to the pool. If there are no jobs in the queue,
     *  a thread is woken up to take the job. If all threads are busy,
     *  the job is added to the end of the queue.
     */
    void AddJob(const std::function<void()>& job)
    {
        // scoped lock
        {
            std::lock_guard<std::mutex> lock(_queueMutex);
            _queue.push(job);
        }
        // scoped lock
        {
            std::lock_guard<std::mutex> lock(_jobsLeftMutex);
            ++_jobsLeft;
        }
        _jobAvailableVar.notify_one();
    }

    /**
     *  Join with all threads. Block until all threads have completed.
     *  The queue may be filled after this call, but the threads will
     *  be done. After invoking `ThreadPool::JoinAll`, the pool can no
     *  longer be used.
     */
    void JoinAll()
    {
        if (!_isRunning)
        {
            return;
        }
        _isRunning = false;

        // add empty jobs to wake up threads
        const int threadCount = _threads.size();
        for (int index = 0; index < threadCount; ++index)
        {
            AddJob([]
                   {
                   });
        }

        // note that we're done, and wake up any thread that's
        // waiting for a new job
        _jobAvailableVar.notify_all();

        for (std::thread& thread : _threads)
        {
            if (thread.joinable())
            {
                thread.join();
            }
        }
    }

    /**
     *  Wait for the pool to empty before continuing.
     *  This does not call `std::thread::join`, it only waits until
     *  all jobs have finished executing.
     */
    void WaitAll()
    {
        std::unique_lock<std::mutex> lock(_jobsLeftMutex);
        if (_jobsLeft > 0)
        {
            _waitVar.wait(lock, [&]
            {
                return _jobsLeft == 0;
            });
        }
    }

    /**
     *  Get the vector of threads themselves, in order to set the
     *  affinity, or anything else you might want to do
     */
    std::vector<std::thread>& GetThreads()
    {
        return _threads;
    }

    /**
     *  Return the next job in the queue to run it in the main thread
     */
    std::function<void()> GetNextJob()
    {
        std::function<void()> job;

        // scoped lock
        {
            std::lock_guard<std::mutex> lock(_queueMutex);

            if (_queue.empty())
            {
                return nullptr;
            }

            // Get job from the queue
            job = _queue.front();
            _queue.pop();
        }

        // scoped lock
        {
            std::lock_guard<std::mutex> lock(_jobsLeftMutex);
            --_jobsLeft;
        }

        _waitVar.notify_one();

        return job;
    }

private:
    std::vector<std::thread> _threads;
    std::queue<std::function<void()>> _queue;

    int _jobsLeft;
    bool _isRunning;
    std::condition_variable _jobAvailableVar;
    std::condition_variable _waitVar;
    std::mutex _jobsLeftMutex;
    std::mutex _queueMutex;
};

#endif //CONCURRENT_THREADPOOL_H