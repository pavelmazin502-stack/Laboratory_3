package lab3_go

import (
	"encoding/gob"
	"encoding/json"
	"errors"
	"fmt" 
	"os"
)

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

// вспомогательный поиск узла 
func (l *DoublyList) findNode(val string) *dNode {
	for curr := l.head; curr != nil; curr = curr.next {
		if curr.data == val {
			return curr
		}
	}
	return nil
}

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

// вставка ДО целевого элемента 
func (l *DoublyList) InsertBefore(target, val string) bool {
	curr := l.findNode(target)
	if curr == nil {
		return false
	}
	if curr == l.head {
		l.PushFront(val)
		return true
	}
	node := &dNode{data: val, prev: curr.prev, next: curr}
	curr.prev.next = node
	curr.prev = node
	l.size++
	return true
}

// вставка ПОСЛЕ целевого элемента 
func (l *DoublyList) InsertAfter(target, val string) bool {
	curr := l.findNode(target)
	if curr == nil {
		return false
	}
	if curr == l.tail {
		l.PushBack(val)
		return true
	}
	node := &dNode{data: val, prev: curr, next: curr.next}
	curr.next.prev = node
	curr.next = node
	l.size++
	return true
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

// удаление ДО целевого элемента 
func (l *DoublyList) RemoveBefore(target string) bool {
	curr := l.findNode(target)
	if curr == nil || curr.prev == nil {
		return false
	}
	toDel := curr.prev
	if toDel == l.head {
		_ = l.PopFront()
		return true
	}
	toDel.prev.next = curr
	curr.prev = toDel.prev
	l.size--
	return true
}

// удаление ПОСЛЕ целевого элемента 
func (l *DoublyList) RemoveAfter(target string) bool {
	curr := l.findNode(target)
	if curr == nil || curr.next == nil {
		return false
	}
	toDel := curr.next
	if toDel == l.tail {
		_ = l.PopBack()
		return true
	}
	curr.next = toDel.next
	toDel.next.prev = curr
	l.size--
	return true
}

// удаление ПО ЗНАЧЕНИЮ 
func (l *DoublyList) RemoveByValue(val string) bool {
	curr := l.findNode(val)
	if curr == nil {
		return false
	}
	if curr == l.head {
		_ = l.PopFront()
		return true
	}
	if curr == l.tail {
		_ = l.PopBack()
		return true
	}
	curr.prev.next = curr.next
	curr.next.prev = curr.prev
	l.size--
	return true
}

func (l *DoublyList) Find(val string) bool {
	return l.findNode(val) != nil
}

// чтение по индексу, голова, хвост, очистка, печать 
func (l *DoublyList) Get(idx int) (string, error) {
	if idx < 0 || idx >= l.size {
		return "", errors.New("index out of range")
	}
	curr := l.head
	for i := 0; i < idx; i++ {
		curr = curr.next
	}
	return curr.data, nil
}

func (l *DoublyList) Front() (string, error) {
	if l.head == nil {
		return "", errors.New("list is empty")
	}
	return l.head.data, nil
}

func (l *DoublyList) Back() (string, error) {
	if l.tail == nil {
		return "", errors.New("list is empty")
	}
	return l.tail.data, nil
}

func (l *DoublyList) Clear() {
	l.head, l.tail, l.size = nil, nil, 0
}

func (l *DoublyList) Print() {
	fmt.Print("[")
	for curr := l.head; curr != nil; curr = curr.next {
		fmt.Print(curr.data)
		if curr.next != nil {
			fmt.Print(" <-> ")
		}
	}
	fmt.Println("]")
}

func (l *DoublyList) PrintReverse() {
	fmt.Print("[")
	for curr := l.tail; curr != nil; curr = curr.prev {
		fmt.Print(curr.data)
		if curr.prev != nil {
			fmt.Print(" <-> ")
		}
	}
	fmt.Println("]")
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
	l.Clear()
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
	l.Clear()
	for _, v := range items {
		l.PushBack(v)
	}
	return nil
}