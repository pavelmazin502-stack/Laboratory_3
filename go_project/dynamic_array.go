package lab3_go

import (
	"encoding/gob"
	"encoding/json"
	"errors"
	"os"
)

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