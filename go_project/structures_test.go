package lab3_go

import (
	"os"
	"testing"
)

func TestDynamicArray(t *testing.T) {
	arr := NewDynamicArray()
	if _, err := arr.Get(0); err == nil {
		t.Fatal("expected error on empty get")
	}
	if err := arr.Set(0, "x"); err == nil {
		t.Fatal("expected error on empty set")
	}
	if err := arr.RemoveAt(0); err == nil {
		t.Fatal("expected error on empty remove")
	}
	if err := arr.InsertAt(5, "x"); err == nil {
		t.Fatal("expected error on invalid insert")
	}

	arr.PushBack("One")
	arr.PushBack("Three")
	_ = arr.InsertAt(1, "Two")
	_ = arr.Set(1, "TwoUpdated")
	if v, _ := arr.Get(1); v != "TwoUpdated" {
		t.Fatalf("expected TwoUpdated, got %s", v)
	}

	_ = arr.SerializeText("go_arr.json")
	arrTxt := NewDynamicArray()
	_ = arrTxt.DeserializeText("go_arr.json")
	if arrTxt.Size() != 3 {
		t.Fatal("json deserialize failed")
	}

	_ = arr.SerializeBinary("go_arr.gob")
	arrBin := NewDynamicArray()
	_ = arrBin.DeserializeBinary("go_arr.gob")
	if arrBin.Size() != 3 {
		t.Fatal("gob deserialize failed")
	}

	_ = arr.RemoveAt(1)
	if arr.Size() != 2 {
		t.Fatal("remove failed")
	}
	os.Remove("go_arr.json")
	os.Remove("go_arr.gob")
}

func TestSinglyList(t *testing.T) {
	l := NewSinglyList()
	if err := l.PopFront(); err == nil {
		t.Fatal("expected empty error")
	}
	if err := l.PopBack(); err == nil {
		t.Fatal("expected empty error")
	}
	if l.RemoveByValue("none") {
		t.Fatal("expected false on empty remove")
	}

	l.PushFront("B")
	l.PushFront("A")
	l.PushBack("C")
	l.PushBack("D")

	if !l.Find("C") || l.Find("Z") {
		t.Fatal("find failed")
	}
	if !l.RemoveByValue("B") || l.RemoveByValue("Z") {
		t.Fatal("remove by value failed")
	}

	_ = l.SerializeText("go_sl.json")
	lTxt := NewSinglyList()
	_ = lTxt.DeserializeText("go_sl.json")
	if lTxt.Size() != 3 {
		t.Fatal("json failed")
	}

	_ = l.SerializeBinary("go_sl.gob")
	lBin := NewSinglyList()
	_ = lBin.DeserializeBinary("go_sl.gob")
	if lBin.Size() != 3 {
		t.Fatal("gob failed")
	}

	_ = l.PopBack()
	_ = l.PopFront()
	_ = l.PopBack()
	if l.Size() != 0 {
		t.Fatal("expected size 0")
	}
	os.Remove("go_sl.json")
	os.Remove("go_sl.gob")
}

func TestDoublyList(t *testing.T) {
	l := NewDoublyList()
	if err := l.PopFront(); err == nil {
		t.Fatal("expected empty error")
	}
	if err := l.PopBack(); err == nil {
		t.Fatal("expected empty error")
	}

	l.PushFront("2")
	l.PushFront("1")
	l.PushBack("3")
	if !l.Find("2") || l.Find("9") {
		t.Fatal("find failed")
	}

	_ = l.SerializeText("go_dl.json")
	lTxt := NewDoublyList()
	_ = lTxt.DeserializeText("go_dl.json")
	if lTxt.Size() != 3 {
		t.Fatal("json failed")
	}

	_ = l.SerializeBinary("go_dl.gob")
	lBin := NewDoublyList()
	_ = lBin.DeserializeBinary("go_dl.gob")
	if lBin.Size() != 3 {
		t.Fatal("gob failed")
	}

	_ = l.PopFront()
	_ = l.PopBack()
	_ = l.PopFront()
	if l.Size() != 0 {
		t.Fatal("expected size 0")
	}
	os.Remove("go_dl.json")
	os.Remove("go_dl.gob")
}

func TestStackAndQueue(t *testing.T) {
	st := NewStack()
	if _, err := st.Peek(); err == nil {
		t.Fatal("expected empty stack error")
	}
	st.Push("Bottom")
	st.Push("Top")

	_ = st.SerializeText("go_st.json")
	stTxt := NewStack()
	_ = stTxt.DeserializeText("go_st.json")
	if top, _ := stTxt.Peek(); top != "Top" || stTxt.Size() != 2 {
		t.Fatal("stack json order broken")
	}

	_ = st.SerializeBinary("go_st.gob")
	stBin := NewStack()
	_ = stBin.DeserializeBinary("go_st.gob")
	if top, _ := stBin.Peek(); top != "Top" {
		t.Fatal("stack gob order broken")
	}
	_ = st.Pop()
	os.Remove("go_st.json")
	os.Remove("go_st.gob")

	q := NewQueue()
	if _, err := q.Peek(); err == nil {
		t.Fatal("expected empty queue error")
	}
	q.Push("First")
	q.Push("Second")

	_ = q.SerializeText("go_q.json")
	qTxt := NewQueue()
	_ = qTxt.DeserializeText("go_q.json")
	if front, _ := qTxt.Peek(); front != "First" || qTxt.Size() != 2 {
		t.Fatal("queue json order broken")
	}

	_ = q.SerializeBinary("go_q.gob")
	qBin := NewQueue()
	_ = qBin.DeserializeBinary("go_q.gob")
	if front, _ := qBin.Peek(); front != "First" {
		t.Fatal("queue gob order broken")
	}
	_ = q.Pop()
	os.Remove("go_q.json")
	os.Remove("go_q.gob")
}

func TestHashTable(t *testing.T) {
	ht := NewHashTable()
	if _, err := ht.Get("missing"); err == nil {
		t.Fatal("expected missing key error")
	}
	if ht.Remove("missing") {
		t.Fatal("expected false on missing remove")
	}

	ht.Insert("k1", "v1")
	ht.Insert("k2", "v2")
	ht.Insert("k1", "v1_updated")
	if !ht.Contains("k1") || ht.Size() != 2 {
		t.Fatal("insert/update failed")
	}

	_ = ht.SerializeText("go_ht.json")
	htTxt := NewHashTable()
	_ = htTxt.DeserializeText("go_ht.json")
	if v, _ := htTxt.Get("k1"); v != "v1_updated" || htTxt.Size() != 2 {
		t.Fatal("ht json failed")
	}

	_ = ht.SerializeBinary("go_ht.gob")
	htBin := NewHashTable()
	_ = htBin.DeserializeBinary("go_ht.gob")
	if v, _ := htBin.Get("k2"); v != "v2" {
		t.Fatal("ht gob failed")
	}

	if !ht.Remove("k1") || ht.Contains("k1") {
		t.Fatal("remove failed")
	}
	os.Remove("go_ht.json")
	os.Remove("go_ht.gob")
}

// ==================== BENCHMARKS (go test -bench=.) ====================
func BenchmarkDynamicArrayPush(b *testing.B) {
	arr := NewDynamicArray()
	for i := 0; i < b.N; i++ {
		arr.PushBack("item")
	}
}

func BenchmarkSinglyListPushFront(b *testing.B) {
	l := NewSinglyList()
	for i := 0; i < b.N; i++ {
		l.PushFront("item")
	}
}

func BenchmarkDoublyListPushBack(b *testing.B) {
	l := NewDoublyList()
	for i := 0; i < b.N; i++ {
		l.PushBack("item")
	}
}

func BenchmarkStackPush(b *testing.B) {
	st := NewStack()
	for i := 0; i < b.N; i++ {
		st.Push("item")
	}
}

func BenchmarkQueuePush(b *testing.B) {
	q := NewQueue()
	for i := 0; i < b.N; i++ {
		q.Push("item")
	}
}

func BenchmarkHashTableInsert(b *testing.B) {
	ht := NewHashTable()
	for i := 0; i < b.N; i++ {
		ht.Insert("key", "value")
	}
}