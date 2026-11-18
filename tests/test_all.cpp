#include <gtest/gtest.h>
#include <stdexcept>

#include "../h_files/DynamicArray.h"
#include "../h_files/SinglyList.h"
#include "../h_files/DoublyList.h"
#include "../h_files/Stack.h"
#include "../h_files/Queue.h"
#include "../h_files/HashTable.h"
#include "../h_files/CompleteTree.h"

// Динамический массив
TEST(StructuresSuite, DynamicArrayTest) {
    DynamicArray arr;
    EXPECT_EQ(arr.get_size(), 0);
    EXPECT_THROW(arr.get(0), std::out_of_range);
    EXPECT_THROW(arr.remove_at(0), std::out_of_range);

    arr.push_back("One");
    arr.push_back("Three");
    arr.insert_at("Two", 1);
    arr.push_back("Four");
    arr.push_back("Five");

    EXPECT_EQ(arr.get_size(), 5);
    arr.set(1, "TwoNew");
    EXPECT_EQ(arr.get(1), "TwoNew");
    arr.remove_at(1);
    arr.print();

    DynamicArray copy = arr;
    DynamicArray assign;
    assign = arr;
    EXPECT_EQ(copy.get_size(), 4);
    EXPECT_EQ(assign.get_size(), 4);

    arr.serialize_text("test_arr.txt");
    DynamicArray arr_txt;
    arr_txt.deserialize_text("test_arr.txt");
    EXPECT_EQ(arr_txt.get_size(), 4);

    arr.serialize_binary("test_arr.bin");
    DynamicArray arr_bin;
    arr_bin.deserialize_binary("test_arr.bin");
    EXPECT_EQ(arr_bin.get_size(), 4);
}

// Односвязный список
TEST(StructuresSuite, SinglyListTest) {
    SinglyList list;
    EXPECT_EQ(list.get_size(), 0);
    EXPECT_THROW(list.pop_front(), std::underflow_error);
    EXPECT_THROW(list.pop_back(), std::underflow_error);

    list.push_front("B");
    list.push_front("A");
    list.push_back("C");
    list.print();

    EXPECT_EQ(list.get_size(), 3);
    EXPECT_TRUE(list.find("A"));
    EXPECT_FALSE(list.find("Z"));

    list.remove_by_value("B");
    EXPECT_EQ(list.get_size(), 2);

    SinglyList copy = list;
    SinglyList assign;
    assign = list;
    list.pop_front();
    EXPECT_EQ(list.get_size(), 1);
    EXPECT_EQ(copy.get_size(), 2);
    EXPECT_EQ(assign.get_size(), 2);

    copy.serialize_text("test_slist.txt");
    SinglyList from_txt;
    from_txt.deserialize_text("test_slist.txt");
    EXPECT_EQ(from_txt.get_size(), 2);
    EXPECT_TRUE(from_txt.find("C"));

    copy.serialize_binary("test_slist.bin");
    SinglyList from_bin;
    from_bin.deserialize_binary("test_slist.bin");
    EXPECT_EQ(from_bin.get_size(), 2);
    EXPECT_TRUE(from_bin.find("A"));
}

// Двусвязный список
TEST(StructuresSuite, DoublyListTest) {
    DoublyList dlist;
    EXPECT_THROW(dlist.pop_front(), std::underflow_error);
    EXPECT_THROW(dlist.pop_back(), std::underflow_error);

    dlist.push_front("20");
    dlist.push_front("10");
    dlist.push_back("30");
    dlist.print();

    EXPECT_EQ(dlist.get_size(), 3);
    EXPECT_TRUE(dlist.find("20"));

    dlist.serialize_text("test_dlist.txt");
    DoublyList d_txt;
    d_txt.deserialize_text("test_dlist.txt");
    EXPECT_EQ(d_txt.get_size(), 3);

    dlist.serialize_binary("test_dlist.bin");
    DoublyList d_bin;
    d_bin.deserialize_binary("test_dlist.bin");
    EXPECT_EQ(d_bin.get_size(), 3);

    DoublyList copy = dlist;
    DoublyList assign;
    assign = dlist;
    dlist.pop_front();
    dlist.pop_back();
    EXPECT_EQ(dlist.get_size(), 1);
    EXPECT_EQ(copy.get_size(), 3);
    EXPECT_EQ(assign.get_size(), 3);
}

// Стек (LIFO)
TEST(StructuresSuite, StackTest) {
    Stack st;
    EXPECT_THROW(st.pop(), std::underflow_error);
    EXPECT_THROW(st.peek(), std::underflow_error);

    st.push("Bottom");
    st.push("Top");
    EXPECT_EQ(st.peek(), "Top");
    st.print();

    st.serialize_text("test_stack.txt");
    Stack st_txt;
    st_txt.deserialize_text("test_stack.txt");
    EXPECT_EQ(st_txt.peek(), "Top");
    EXPECT_EQ(st_txt.get_size(), 2);

    st.serialize_binary("test_stack.bin");
    Stack st_bin;
    st_bin.deserialize_binary("test_stack.bin");
    EXPECT_EQ(st_bin.peek(), "Top");

    Stack copy = st;
    Stack assign;
    assign = st;
    st.pop();
    EXPECT_EQ(st.peek(), "Bottom");
    EXPECT_EQ(copy.peek(), "Top");
    EXPECT_EQ(assign.peek(), "Top");
}

// Очередь (FIFO)
TEST(StructuresSuite, QueueTest) {
    Queue q;
    EXPECT_THROW(q.pop(), std::underflow_error);
    EXPECT_THROW(q.peek(), std::underflow_error);

    q.push("First");
    q.push("Second");
    EXPECT_EQ(q.peek(), "First");
    q.print();

    q.serialize_text("test_queue.txt");
    Queue q_txt;
    q_txt.deserialize_text("test_queue.txt");
    EXPECT_EQ(q_txt.peek(), "First");

    q.serialize_binary("test_queue.bin");
    Queue q_bin;
    q_bin.deserialize_binary("test_queue.bin");
    EXPECT_EQ(q_bin.peek(), "First");

    Queue copy = q;
    Queue assign;
    assign = q;
    q.pop();
    EXPECT_EQ(q.peek(), "Second");
    EXPECT_EQ(copy.peek(), "First");
    EXPECT_EQ(assign.peek(), "First");
}

// Хеш-таблица
TEST(StructuresSuite, HashTableTest) {
    HashTable ht;
    ht.insert("k1", "v1");
    ht.insert("k2", "v2");
    ht.insert("k1", "v1_new");

    EXPECT_EQ(ht.get_size(), 2);
    EXPECT_TRUE(ht.contains("k1"));
    EXPECT_EQ(ht.get("k1"), "v1_new");
    EXPECT_THROW(ht.get("missing"), std::runtime_error);
    ht.print();

    ht.serialize_text("test_ht.txt");
    HashTable ht_txt;
    ht_txt.deserialize_text("test_ht.txt");
    EXPECT_EQ(ht_txt.get("k2"), "v2");

    ht.serialize_binary("test_ht.bin");
    HashTable ht_bin;
    ht_bin.deserialize_binary("test_ht.bin");
    EXPECT_EQ(ht_bin.get("k1"), "v1_new");

    HashTable copy = ht;
    HashTable assign;
    assign = ht;
    ht.remove("k1");
    EXPECT_FALSE(ht.contains("k1"));
    EXPECT_TRUE(copy.contains("k1"));
    EXPECT_TRUE(assign.contains("k1"));
}

// Complete Binary Tree
TEST(StructuresSuite, CompleteTreeTest) {
    CompleteTree tree;
    EXPECT_TRUE(tree.is_complete());

    tree.insert("Root");
    tree.insert("Left");
    tree.insert("Right");
    EXPECT_TRUE(tree.find("Left"));
    EXPECT_FALSE(tree.find("None"));
    EXPECT_TRUE(tree.is_complete());
    tree.print();

    CompleteTree copy = tree;
    CompleteTree assign;
    assign = tree;

    tree.serialize_text("test_tree.txt");
    CompleteTree t_txt;
    t_txt.deserialize_text("test_tree.txt");
    EXPECT_TRUE(t_txt.find("Right"));
    EXPECT_TRUE(t_txt.is_complete());

    tree.serialize_binary("test_tree.bin");
    CompleteTree t_bin;
    t_bin.deserialize_binary("test_tree.bin");
    EXPECT_TRUE(t_bin.find("Root"));
}