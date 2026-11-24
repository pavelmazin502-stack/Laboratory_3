package lab3_go

import (
	"encoding/gob"
	"encoding/json"
	"errors"
	"os"
)

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