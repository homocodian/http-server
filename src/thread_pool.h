#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <atomic>
#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <vector>

class ThreadPool {
private:
  std::vector<std::thread> workers;
  std::queue<std::function<void()>> tasks;

  std::mutex queueMutex;
  std::atomic<bool> stop;
  std::condition_variable condition;

public:
  ThreadPool(size_t threads);

  template <class F, class... Args>
  auto enqueue(F &&f, Args &&...args)
      -> std::future<std::invoke_result_t<F, Args...>> {
    using returnType = std::invoke_result_t<F, Args...>;

    auto task = std::make_shared<std::packaged_task<returnType()>>(
        std::bind(std::forward<F>(f), std::forward<Args>(args)...));

    std::future<returnType> res = task->get_future();

    {
      std::unique_lock<std::mutex> lock(queueMutex);
      if (stop) {
        throw std::runtime_error("enqueue in stopped ThreadPool");
      }

      tasks.emplace([task]() { (*task)(); });
    }

    condition.notify_one();
    return res;
  }

  ~ThreadPool();
};

ThreadPool::ThreadPool(size_t threads) : stop(false) {
  for (size_t i = 0; i < threads; i++) {
    workers.emplace_back([this] {
      while (true) {
        std::function<void()> task;

        {
          std::unique_lock<std::mutex> lock(queueMutex);
          condition.wait(lock, [this] { return stop || !tasks.empty(); });

          if (stop && tasks.empty()) {
            return;
          }

          task = std::move(tasks.front());
          tasks.pop();
        }

        task();
      }
    });
  }
}

ThreadPool::~ThreadPool() {
  {
    std::unique_lock<std::mutex> lock(queueMutex);
    stop = true;
  }

  condition.notify_all();
  for (auto &&worker : workers) {
    worker.join();
  }
}

#endif