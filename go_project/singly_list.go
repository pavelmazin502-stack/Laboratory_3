package lab3_go

import (
	"encoding/gob"
	"encoding/json"
	"errors"
	"fmt" 
	"os"
)

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

// вставка ДО целевого элемента 
func (l *SinglyList) InsertBefore(target, val string) bool {
	if l.head == nil {
		return false
	}
	if l.head.data == target {
		l.PushFront(val)
		return true
	}
	curr := l.head
	for curr.next != nil && curr.next.data != target {
		curr = curr.next
	}
	if curr.next == nil {
		return false
	}
	curr.next = &sNode{data: val, next: curr.next}
	l.size++
	return true
}

// вставка ПОСЛЕ целевого элемента 
func (l *SinglyList) InsertAfter(target, val string) bool {
	curr := l.head
	for curr != nil && curr.data != target {
		curr = curr.next
	}
	if curr == nil {
		return false
	}
	node := &sNode{data: val, next: curr.next}
	curr.next = node
	if curr == l.tail {
		l.tail = node
	}
	l.size++
	return true
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

//  удаление ДО целевого элемента 
func (l *SinglyList) RemoveBefore(target string) bool {
	if l.head == nil || l.head.next == nil || l.head.data == target {
		return false
	}
	if l.head.next.data == target {
		_ = l.PopFront()
		return true
	}
	prev := l.head
	for prev.next.next != nil && prev.next.next.data != target {
		prev = prev.next
	}
	if prev.next.next == nil {
		return false
	}
	prev.next = prev.next.next
	l.size--
	return true
}

// удаление ПОСЛЕ целевого элемента 
func (l *SinglyList) RemoveAfter(target string) bool {
	curr := l.head
	for curr != nil && curr.data != target {
		curr = curr.next
	}
	if curr == nil || curr.next == nil {
		return false
	}
	toDel := curr.next
	curr.next = toDel.next
	if toDel == l.tail {
		l.tail = curr
	}
	l.size--
	return true
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

// чтение по индексу, голова, хвост, очистка, печать 
func (l *SinglyList) Get(idx int) (string, error) {
	if idx < 0 || idx >= l.size {
		return "", errors.New("index out of range")
	}
	curr := l.head
	for i := 0; i < idx; i++ {
		curr = curr.next
	}
	return curr.data, nil
}

func (l *SinglyList) Front() (string, error) {
	if l.head == nil {
		return "", errors.New("list is empty")
	}
	return l.head.data, nil
}

func (l *SinglyList) Back() (string, error) {
	if l.tail == nil {
		return "", errors.New("list is empty")
	}
	return l.tail.data, nil
}

func (l *SinglyList) Clear() {
	l.head, l.tail, l.size = nil, nil, 0
}

func (l *SinglyList) Print() {
	fmt.Print("[")
	for curr := l.head; curr != nil; curr = curr.next {
		fmt.Print(curr.data)
		if curr.next != nil {
			fmt.Print(" -> ")
		}
	}
	fmt.Println("]")
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
	l.Clear()
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
	l.Clear()
	for _, v := range items {
		l.PushBack(v)
	}
	return nil
}