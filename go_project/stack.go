package lab3_go

import (
	"encoding/gob"
	"encoding/json"
	"errors"
	"os"
)

type Stack struct {
	list SinglyList
}

func NewStack() *Stack           { return &Stack{} }
func (s *Stack) Push(val string) { s.list.PushFront(val) }
func (s *Stack) Pop() error      { return s.list.PopFront() }
func (s *Stack) Size() int       { return s.list.Size() }

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