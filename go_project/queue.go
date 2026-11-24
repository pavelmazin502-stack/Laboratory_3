package lab3_go

import "errors"

type Queue struct {
	list DoublyList
}

func NewQueue() *Queue           { return &Queue{} }
func (q *Queue) Push(val string) { q.list.PushBack(val) }
func (q *Queue) Pop() error      { return q.list.PopFront() }
func (q *Queue) Size() int       { return q.list.Size() }

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