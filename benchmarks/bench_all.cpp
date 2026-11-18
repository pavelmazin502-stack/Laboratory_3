#include <benchmark/benchmark.h>
#include "../h_files/DynamicArray.h"
#include "../h_files/SinglyList.h"
#include "../h_files/DoublyList.h"
#include "../h_files/Stack.h"
#include "../h_files/Queue.h"
#include "../h_files/HashTable.h"
#include "../h_files/CompleteTree.h"

static void BM_DynamicArray_PushBack(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        DynamicArray arr;
        state.ResumeTiming();
        arr.push_back("item");
    }
}
BENCHMARK(BM_DynamicArray_PushBack);

static void BM_SinglyList_PushFront(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        SinglyList list;
        state.ResumeTiming();
        list.push_front("item");
    }
}
BENCHMARK(BM_SinglyList_PushFront);

static void BM_DoublyList_PushBack(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        DoublyList dlist;
        state.ResumeTiming();
        dlist.push_back("item");
    }
}
BENCHMARK(BM_DoublyList_PushBack);

static void BM_Stack_Push(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        Stack st;
        state.ResumeTiming();
        st.push("item");
    }
}
BENCHMARK(BM_Stack_Push);

static void BM_Queue_Push(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        Queue q;
        state.ResumeTiming();
        q.push("item");
    }
}
BENCHMARK(BM_Queue_Push);

static void BM_HashTable_Insert(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        HashTable ht;
        state.ResumeTiming();
        ht.insert("key", "value");
    }
}
BENCHMARK(BM_HashTable_Insert);