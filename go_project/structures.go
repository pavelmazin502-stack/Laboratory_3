package lab3_go

import (
	"encoding/gob"
	"encoding/json"
	"errors"
	"os"
)

// ==================== 1. DYNAMIC ARRAY ====================
type DynamicArray struct {
	data []string // приватное поле (инкапсуляция)
}

func NewDynamicArray() *DynamicArray {
	return &DynamicArray{data: make([]string, 0, 4)}
}

func (a *DynamicArray) PushBack(val string) {
	a.data = append(a.data, val)
}

func (a *DynamicArray) InsertAt(idx int, val string) error {
	if idx < 0 || idx > len(a.data) {
		return errors.New("index out of range")
	}
	a.data = append(a.data[:idx], append([]string{val}, a.data[idx:]...)...)
	return nil
}

func (a *DynamicArray) Get(idx int) (string, error) {
	if idx < 0 || idx >= len(a.data) {
		return "", errors.New("index out of range")
	}
	return a.data[idx], nil
}

func (a *DynamicArray) Set(idx int, val string) error {
	if idx < 0 || idx >= len(a.data) {
		return errors.New("index out of range")
	}
	a.data[idx] = val
	return nil
}

func (a *DynamicArray) RemoveAt(idx int) error {
	if idx < 0 || idx >= len(a.data) {
		return errors.New("index out of range")
	}
	a.data = append(a.data[:idx], a.data[idx+1:]...)
	return nil
}

func (a *DynamicArray) Size() int { return len(a.data) }

func (a *DynamicArray) SerializeText(filename string) error {
	bytes, err := json.MarshalIndent(a.data, "", "  ")
	if err != nil {
		return err
	}
	return os.WriteFile(filename, bytes, 0644)
}

func (a *DynamicArray) DeserializeText(filename string) error {
	bytes, err := os.ReadFile(filename)
	if err != nil {
		return err
	}
	return json.Unmarshal(bytes, &a.data)
}

func (a *DynamicArray) SerializeBinary(filename string) error {
	f, err := os.Create(filename)
	if err != nil {
		return err
	}
	defer f.Close()
	return gob.NewEncoder(f).Encode(a.data)
}

func (a *DynamicArray) DeserializeBinary(filename string) error {
	f, err := os.Open(filename)
	if err != nil {
		return err
	}
	defer f.Close()
	return gob.NewDecoder(f).Decode(&a.data)
}

// ==================== 2. SINGLY LINKED LIST ====================
type sNode struct {
	data string
	next *sNode
}

type SinglyList struct {
	head *sNode
	tail *sNode
	size int
}

func NewSinglyList() *SinglyList { return &SinglyList{} }

func (l *SinglyList) PushFront(val string) {
	node := &sNode{data: val, next: l.head}
	l.head = node
	if l.tail == nil {
		l.tail = node
	}
	l.size++
}

func (l *SinglyList) PushBack(val string) {
	node := &sNode{data: val}
	if l.tail == nil {
		l.head = node
		l.tail = node
	} else {
		l.tail.next = node
		l.tail = node
	}
	l.size++
}

func (l *SinglyList) PopFront() error {
	if l.head == nil {
		return errors.New("list is empty")
	}
	l.head = l.head.next
	if l.head == nil {
		l.tail = nil
	}
	l.size--
	return nil
}

func (l *SinglyList) PopBack() error {
	if l.head == nil {
		return errors.New("list is empty")
	}
	if l.head == l.tail {
		return l.PopFront()
	}
	curr := l.head
	for curr.next != l.tail {
		curr = curr.next
	}
	l.tail = curr
	l.tail.next = nil
	l.size--
	return nil
}

func (l *SinglyList) RemoveByValue(val string) bool {
	if l.head == nil {
		return false
	}
	if l.head.data == val {
		_ = l.PopFront()
		return true
	}
	curr := l.head
	for curr.next != nil && curr.next.data != val {
		curr = curr.next
	}
	if curr.next == nil {
		return false
	}
	if curr.next == l.tail {
		l.tail = curr
	}
	curr.next = curr.next.next
	l.size--
	return true
}

func (l *SinglyList) Find(val string) bool {
	for curr := l.head; curr != nil; curr = curr.next {
		if curr.data == val {
			return true
		}
	}
	return false
}

func (l *SinglyList) Size() int { return l.size }

func (l *SinglyList) toSlice() []string {
	res := make([]string, 0, l.size)
	for curr := l.head; curr != nil; curr = curr.next {
		res = append(res, curr.data)
	}
	return res
}

func (l *SinglyList) SerializeText(filename string) error {
	bytes, err := json.MarshalIndent(l.toSlice(), "", "  ")
	if err != nil {
		return err
	}
	return os.WriteFile(filename, bytes, 0644)
}

func (l *SinglyList) DeserializeText(filename string) error {
	bytes, err := os.ReadFile(filename)
	if err != nil {
		return err
	}
	var items []string
	if err := json.Unmarshal(bytes, &items); err != nil {
		return err
	}
	l.head, l.tail, l.size = nil, nil, 0
	for _, v := range items {
		l.PushBack(v)
	}
	return nil
}

func (l *SinglyList) SerializeBinary(filename string) error {
	f, err := os.Create(filename)
	if err != nil {
		return err
	}
	defer f.Close()
	return gob.NewEncoder(f).Encode(l.toSlice())
}

func (l *SinglyList) DeserializeBinary(filename string) error {
	f, err := os.Open(filename)
	if err != nil {
		return err
	}
	defer f.Close()
	var items []string
	if err := gob.NewDecoder(f).Decode(&items); err != nil {
		return err
	}
	l.head, l.tail, l.size = nil, nil, 0
	for _, v := range items {
		l.PushBack(v)
	}
	return nil
}

// ==================== 3. DOUBLY LINKED LIST ====================
type dNode struct {
	data string
	prev *dNode
	next *dNode
}

type DoublyList struct {
	head *dNode
	tail *dNode
	size int
}

func NewDoublyList() *DoublyList { return &DoublyList{} }

func (l *DoublyList) PushFront(val string) {
	node := &dNode{data: val, next: l.head}
	if l.head != nil {
		l.head.prev = node
	} else {
		l.tail = node
	}
	l.head = node
	l.size++
}

func (l *DoublyList) PushBack(val string) {
	node := &dNode{data: val, prev: l.tail}
	if l.tail != nil {
		l.tail.next = node
	} else {
		l.head = node
	}
	l.tail = node
	l.size++
}

func (l *DoublyList) PopFront() error {
	if l.head == nil {
		return errors.New("list is empty")
	}
	l.head = l.head.next
	if l.head != nil {
		l.head.prev = nil
	} else {
		l.tail = nil
	}
	l.size--
	return nil
}

func (l *DoublyList) PopBack() error {
	if l.tail == nil {
		return errors.New("list is empty")
	}
	l.tail = l.tail.prev
	if l.tail != nil {
		l.tail.next = nil
	} else {
		l.head = nil
	}
	l.size--
	return nil
}

func (l *DoublyList) Find(val string) bool {
	for curr := l.head; curr != nil; curr = curr.next {
		if curr.data == val {
			return true
		}
	}
	return false
}

func (l *DoublyList) Size() int { return l.size }

func (l *DoublyList) toSlice() []string {
	res := make([]string, 0, l.size)
	for curr := l.head; curr != nil; curr = curr.next {
		res = append(res, curr.data)
	}
	return res
}

func (l *DoublyList) SerializeText(filename string) error {
	bytes, err := json.MarshalIndent(l.toSlice(), "", "  ")
	if err != nil {
		return err
	}
	return os.WriteFile(filename, bytes, 0644)
}

func (l *DoublyList) DeserializeText(filename string) error {
	bytes, err := os.ReadFile(filename)
	if err != nil {
		return err
	}
	var items []string
	if err := json.Unmarshal(bytes, &items); err != nil {
		return err
	}
	l.head, l.tail, l.size = nil, nil, 0
	for _, v := range items {
		l.PushBack(v)
	}
	return nil
}

func (l *DoublyList) SerializeBinary(filename string) error {
	f, err := os.Create(filename)
	if err != nil {
		return err
	}
	defer f.Close()
	return gob.NewEncoder(f).Encode(l.toSlice())
}

func (l *DoublyList) DeserializeBinary(filename string) error {
	f, err := os.Open(filename)
	if err != nil {
		return err
	}
	defer f.Close()
	var items []string
	if err := gob.NewDecoder(f).Decode(&items); err != nil {
		return err
	}
	l.head, l.tail, l.size = nil, nil, 0
	for _, v := range items {
		l.PushBack(v)
	}
	return nil
}

// ==================== 4. STACK (Паттерн Адаптер) ====================
type Stack struct {
	list SinglyList
}

func NewStack() *Stack               { return &Stack{} }
func (s *Stack) Push(val string)     { s.list.PushFront(val) }
func (s *Stack) Pop() error          { return s.list.PopFront() }
func (s *Stack) Size() int           { return s.list.Size() }
func (s *Stack) Peek() (string, error) {
	if s.list.head == nil {
		return "", errors.New("stack is empty")
	}
	return s.list.head.data, nil
}

func (s *Stack) SerializeText(fn string) error { return s.list.SerializeText(fn) }
func (s *Stack) DeserializeText(fn string) error {
	bytes, err := os.ReadFile(fn)
	if err != nil {
		return err
	}
	var items []string
	if err := json.Unmarshal(bytes, &items); err != nil {
		return err
	}
	s.list = SinglyList{}
	for i := len(items) - 1; i >= 0; i-- {
		s.Push(items[i])
	}
	return nil
}

func (s *Stack) SerializeBinary(fn string) error { return s.list.SerializeBinary(fn) }
func (s *Stack) DeserializeBinary(fn string) error {
	f, err := os.Open(fn)
	if err != nil {
		return err
	}
	defer f.Close()
	var items []string
	if err := gob.NewDecoder(f).Decode(&items); err != nil {
		return err
	}
	s.list = SinglyList{}
	for i := len(items) - 1; i >= 0; i-- {
		s.Push(items[i])
	}
	return nil
}

// ==================== 5. QUEUE (Паттерн Адаптер) ====================
type Queue struct {
	list DoublyList
}

func NewQueue() *Queue               { return &Queue{} }
func (q *Queue) Push(val string)     { q.list.PushBack(val) }
func (q *Queue) Pop() error          { return q.list.PopFront() }
func (q *Queue) Size() int           { return q.list.Size() }
func (q *Queue) Peek() (string, error) {
	if q.list.head == nil {
		return "", errors.New("queue is empty")
	}
	return q.list.head.data, nil
}

func (q *Queue) SerializeText(fn string) error     { return q.list.SerializeText(fn) }
func (q *Queue) DeserializeText(fn string) error   { return q.list.DeserializeText(fn) }
func (q *Queue) SerializeBinary(fn string) error   { return q.list.SerializeBinary(fn) }
func (q *Queue) DeserializeBinary(fn string) error { return q.list.DeserializeBinary(fn) }

// ==================== 6. HASH TABLE (Метод цепочек) ====================
type hashNode struct {
	key   string
	value string
	next  *hashNode
}

type HashTable struct {
	buckets  []*hashNode
	capacity int
	size     int
}

func NewHashTable() *HashTable {
	cap := 128
	return &HashTable{
		buckets:  make([]*hashNode, cap),
		capacity: cap,
	}
}

func (h *HashTable) hashFunction(key string) int {
	var hash uint64 = 0
	for i := 0; i < len(key); i++ {
		hash = hash*31 + uint64(key[i])
	}
	return int(hash % uint64(h.capacity))
}

func (h *HashTable) Insert(key, value string) {
	idx := h.hashFunction(key)
	curr := h.buckets[idx]
	for curr != nil {
		if curr.key == key {
			curr.value = value
			return
		}
		curr = curr.next
	}
	h.buckets[idx] = &hashNode{key: key, value: value, next: h.buckets[idx]}
	h.size++
}

func (h *HashTable) Get(key string) (string, error) {
	idx := h.hashFunction(key)
	curr := h.buckets[idx]
	for curr != nil {
		if curr.key == key {
			return curr.value, nil
		}
		curr = curr.next
	}
	return "", errors.New("key not found")
}

func (h *HashTable) Contains(key string) bool {
	_, err := h.Get(key)
	return err == nil
}

func (h *HashTable) Remove(key string) bool {
	idx := h.hashFunction(key)
	curr := h.buckets[idx]
	var prev *hashNode
	for curr != nil {
		if curr.key == key {
			if prev == nil {
				h.buckets[idx] = curr.next
			} else {
				prev.next = curr.next
			}
			h.size--
			return true
		}
		prev = curr
		curr = curr.next
	}
	return false
}

func (h *HashTable) Size() int { return h.size }

func (h *HashTable) toMap() map[string]string {
	m := make(map[string]string, h.size)
	for i := 0; i < h.capacity; i++ {
		for curr := h.buckets[i]; curr != nil; curr = curr.next {
			m[curr.key] = curr.value
		}
	}
	return m
}

func (h *HashTable) SerializeText(filename string) error {
	bytes, err := json.MarshalIndent(h.toMap(), "", "  ")
	if err != nil {
		return err
	}
	return os.WriteFile(filename, bytes, 0644)
}

func (h *HashTable) DeserializeText(filename string) error {
	bytes, err := os.ReadFile(filename)
	if err != nil {
		return err
	}
	var m map[string]string
	if err := json.Unmarshal(bytes, &m); err != nil {
		return err
	}
	h.buckets = make([]*hashNode, h.capacity)
	h.size = 0
	for k, v := range m {
		h.Insert(k, v)
	}
	return nil
}

func (h *HashTable) SerializeBinary(filename string) error {
	f, err := os.Create(filename)
	if err != nil {
		return err
	}
	defer f.Close()
	return gob.NewEncoder(f).Encode(h.toMap())
}

func (h *HashTable) DeserializeBinary(filename string) error {
	f, err := os.Open(filename)
	if err != nil {
		return err
	}
	defer f.Close()
	var m map[string]string
	if err := gob.NewDecoder(f).Decode(&m); err != nil {
		return err
	}
	h.buckets = make([]*hashNode, h.capacity)
	h.size = 0
	for k, v := range m {
		h.Insert(k, v)
	}
	return nil
}