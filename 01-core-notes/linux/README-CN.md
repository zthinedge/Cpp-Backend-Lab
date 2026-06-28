# Linux 核心笔记

这个目录用于记录 Linux 相关的后端基础知识。包括后端组件里经常用到的机制拆开：线程、锁、睡眠唤醒、IO、网络、进程资源等。



```text
thread-concurrency/
├── README-CN.md
├── 01-thread-lifecycle/
│   ├── README-CN.md
│   ├── thread-process-task.md
│   ├── join-detach.md
│   └── context-switch.md
├── 02-mutual-exclusion/
│   ├── README-CN.md
│   ├── mutex/
│   │   ├── README-CN.md
│   │   └── demo_mutex.cpp
│   ├── spinlock/
│   │   ├── README-CN.md
│   │   └── demo_spinlock.cpp
│   ├── rwlock/
│   │   └── README-CN.md
│   └── semaphore/
│       └── README-CN.md
├── 03-sleep-wakeup/
│   ├── README-CN.md
│   ├── condition-variable/
│   │   ├── README-CN.md
│   │   ├── demo_wait_notify.cpp
│   │   ├── demo_lost_wakeup.cpp
│   │   └── demo_producer_consumer.cpp
│   ├── futex/
│   │   ├── README-CN.md
│   │   └── demo_futex.c
│   └── wait-queue/
│       └── README-CN.md
├── 04-atomic-memory-order/
│   ├── README-CN.md
│   ├── atomic.md
│   ├── memory-order.md
│   └── demo_atomic.cpp
├── 05-concurrency-patterns/
│   ├── README-CN.md
│   ├── producer-consumer/
│   │   └── README-CN.md
│   ├── blocking-queue/
│   │   ├── README-CN.md
│   │   └── BlockingQueue.h
│   └── pool-components/
│       ├── threadpool-analysis.md
│       └── connectionpool-analysis.md
└── 06-debugging-tools/
    ├── README-CN.md
    ├── deadlock.md
    ├── race-condition.md
    └── tools.md
```

