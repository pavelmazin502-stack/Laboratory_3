package lab3_go

import (
	"fmt"
	"os"
	"testing"
)

// 1. Тест динамического массива (DynamicArray)
func TestDynamicArray(t *testing.T) {
	arr := NewDynamicArray()
	if _, err := arr.Get(0); err == nil {
		t.Fatal("expected out of range error on Get")
	}
	if err := arr.Set(0, "X"); err == nil {
		t.Fatal("expected out of range error on Set")
	}
	if err := arr.RemoveAt(0); err == nil {
		t.Fatal("expected out of range error on RemoveAt")
	}
	if err := arr.InsertAt(-1, "X"); err == nil {
		t.Fatal("expected out of range error on InsertAt")
	}

	arr.PushBack("One")
	arr.PushBack("Three")
	arr.PushBack("Four")
	arr.PushBack("Five")
	arr.PushBack("Six") // проверка расширения емкости > 4
	if err := arr.InsertAt(1, "Two"); err != nil {
		t.Fatalf("unexpected InsertAt error: %v", err)
	}
	if err := arr.Set(1, "TwoUpdated"); err != nil {
		t.Fatalf("unexpected Set error: %v", err)
	}
	if val, err := arr.Get(1); err != nil || val != "TwoUpdated" || arr.Size() != 6 {
		t.Fatalf("expected TwoUpdated, got %s", val)
	}

	_ = arr.SerializeText("go_arr.json")
	arrTxt := NewDynamicArray()
	_ = arrTxt.DeserializeText("go_arr.json")
	if arrTxt.Size() != 6 {
		t.Fatal("json deserialize failed")
	}

	_ = arr.SerializeBinary("go_arr.gob")
	arrBin := NewDynamicArray()
	_ = arrBin.DeserializeBinary("go_arr.gob")
	if arrBin.Size() != 6 {
		t.Fatal("gob deserialize failed")
	}

	if err := arr.RemoveAt(0); err != nil || arr.Size() != 5 {
		t.Fatal("RemoveAt failed")
	}

	// Проверка ошибок чтения несуществующих файлов
	if err := arrTxt.DeserializeText("no_such_file.json"); err == nil {
		t.Fatal("expected file open error")
	}
	if err := arrBin.DeserializeBinary("no_such_file.gob"); err == nil {
		t.Fatal("expected file open error")
	}

	os.Remove("go_arr.json")
	os.Remove("go_arr.gob")
}

// 2. Тест односвязного списка (SinglyList) — все 11+ операций
func TestSinglyList(t *testing.T) {
	l := NewSinglyList()
	if err := l.PopFront(); err == nil {
		t.Fatal("expected empty error on PopFront")
	}
	if err := l.PopBack(); err == nil {
		t.Fatal("expected empty error on PopBack")
	}
	if _, err := l.Front(); err == nil {
		t.Fatal("expected empty error on Front")
	}
	if _, err := l.Back(); err == nil {
		t.Fatal("expected empty error on Back")
	}
	if _, err := l.Get(0); err == nil {
		t.Fatal("expected out of range error on Get")
	}
	if l.RemoveByValue("none") || l.InsertBefore("A", "X") || l.InsertAfter("A", "X") || l.RemoveBefore("A") || l.RemoveAfter("A") {
		t.Fatal("expected false on empty list")
	}

	l.PushFront("B")
	l.PushFront("A")
	l.PushBack("D")

	// Вставка до и после (у головы, в середине, у хвоста, несуществующий target)
	if !l.InsertBefore("A", "0") || !l.InsertBefore("D", "C") || l.InsertBefore("Z", "X") {
		t.Fatal("InsertBefore failed")
	}
	if !l.InsertAfter("D", "E") || !l.InsertAfter("A", "A1") || l.InsertAfter("Z", "X") {
		t.Fatal("InsertAfter failed")
	}

	// Чтение и вывод
	if v, _ := l.Front(); v != "0" {
		t.Fatalf("expected Front=0, got %s", v)
	}
	if v, _ := l.Back(); v != "E" {
		t.Fatalf("expected Back=E, got %s", v)
	}
	if v, _ := l.Get(2); v != "A1" {
		t.Fatalf("expected Get(2)=A1, got %s", v)
	}
	l.Print()

	// Удаление до и после
	if l.RemoveBefore("0") || !l.RemoveBefore("A") || !l.RemoveBefore("B") || l.RemoveBefore("Z") {
		t.Fatal("RemoveBefore failed")
	}
	if l.RemoveAfter("E") || !l.RemoveAfter("D") || !l.RemoveAfter("A") || l.RemoveAfter("Z") {
		t.Fatal("RemoveAfter failed")
	}

	// Поиск и удаление по значению
	if !l.Find("C") || l.Find("Z") {
		t.Fatal("Find failed")
	}
	if !l.RemoveByValue("C") || l.RemoveByValue("Z") {
		t.Fatal("RemoveByValue failed")
	}

	// Текстовая и бинарная сериализация
	_ = l.SerializeText("go_sl.json")
	lTxt := NewSinglyList()
	_ = lTxt.DeserializeText("go_sl.json")
	if lTxt.Size() != 2 {
		t.Fatal("json deserialize failed")
	}

	_ = l.SerializeBinary("go_sl.gob")
	lBin := NewSinglyList()
	_ = lBin.DeserializeBinary("go_sl.gob")
	if lBin.Size() != 2 {
		t.Fatal("gob deserialize failed")
	}

	_ = l.PopBack()
	_ = l.PopFront()
	if l.Size() != 0 {
		t.Fatal("expected size 0")
	}
	l.PushBack("X")
	l.PushBack("Y")
	if !l.RemoveByValue("Y") || !l.RemoveByValue("X") {
		t.Fatal("remove head/tail by value failed")
	}
	l.Clear()

	os.Remove("go_sl.json")
	os.Remove("go_sl.gob")
}

// 3. Тест двусвязного списка (DoublyList) — все операции + PrintReverse
func TestDoublyList(t *testing.T) {
	l := NewDoublyList()
	if err := l.PopFront(); err == nil {
		t.Fatal("expected empty error on PopFront")
	}
	if err := l.PopBack(); err == nil {
		t.Fatal("expected empty error on PopBack")
	}
	if _, err := l.Front(); err == nil {
		t.Fatal("expected empty error on Front")
	}
	if _, err := l.Back(); err == nil {
		t.Fatal("expected empty error on Back")
	}
	if _, err := l.Get(0); err == nil {
		t.Fatal("expected out of range error on Get")
	}
	if l.RemoveByValue("none") || l.InsertBefore("A", "X") || l.InsertAfter("A", "X") || l.RemoveBefore("A") || l.RemoveAfter("A") {
		t.Fatal("expected false on empty list")
	}

	l.PushFront("B")
	l.PushFront("A")
	l.PushBack("D")

	// Вставка до и после
	if !l.InsertBefore("A", "0") || !l.InsertBefore("D", "C") || l.InsertBefore("Z", "X") {
		t.Fatal("InsertBefore failed")
	}
	if !l.InsertAfter("D", "E") || !l.InsertAfter("A", "A1") || l.InsertAfter("Z", "X") {
		t.Fatal("InsertAfter failed")
	}

	// Чтение и печать в прямом и обратном порядке
	if v, _ := l.Front(); v != "0" {
		t.Fatalf("expected Front=0, got %s", v)
	}
	if v, _ := l.Back(); v != "E" {
		t.Fatalf("expected Back=E, got %s", v)
	}
	if v, _ := l.Get(2); v != "A1" {
		t.Fatalf("expected Get(2)=A1, got %s", v)
	}
	l.Print()
	l.PrintReverse()

	// Удаление до, после и по значению
	if l.RemoveBefore("0") || !l.RemoveBefore("A") || !l.RemoveBefore("B") || l.RemoveBefore("Z") {
		t.Fatal("RemoveBefore failed")
	}
	if l.RemoveAfter("E") || !l.RemoveAfter("D") || !l.RemoveAfter("A") || l.RemoveAfter("Z") {
		t.Fatal("RemoveAfter failed")
	}
	if !l.Find("C") || l.Find("Z") {
		t.Fatal("Find failed")
	}
	if !l.RemoveByValue("C") || l.RemoveByValue("Z") {
		t.Fatal("RemoveByValue failed")
	}

	// Сериализация JSON и Gob
	_ = l.SerializeText("go_dl.json")
	lTxt := NewDoublyList()
	_ = lTxt.DeserializeText("go_dl.json")
	if lTxt.Size() != 2 {
		t.Fatal("json deserialize failed")
	}

	_ = l.SerializeBinary("go_dl.gob")
	lBin := NewDoublyList()
	_ = lBin.DeserializeBinary("go_dl.gob")
	if lBin.Size() != 2 {
		t.Fatal("gob deserialize failed")
	}

	if !l.RemoveByValue("D") || !l.RemoveByValue("A") {
		t.Fatal("remove head/tail by value failed")
	}
	l.PushBack("1")
	l.PushBack("2")
	_ = l.PopFront()
	_ = l.PopBack()
	if l.Size() != 0 {
		t.Fatal("expected size 0")
	}
	l.Clear()

	os.Remove("go_dl.json")
	os.Remove("go_dl.gob")
}

// 4. Тест стека (Stack, LIFO) и 5. Тест очереди (Queue, FIFO)
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
	if top, _ := stBin.Peek(); top != "Top" || stBin.Size() != 2 {
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
	if front, _ := qBin.Peek(); front != "First" || qBin.Size() != 2 {
		t.Fatal("queue gob order broken")
	}

	_ = q.Pop()
	os.Remove("go_q.json")
	os.Remove("go_q.gob")
}

// 6. Тест хеш-таблицы (HashTable)
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

	// Добавляем элементы для проверки разрешения коллизий методом цепочек
	for i := 0; i < 150; i++ {
		ht.Insert(fmt.Sprintf("key_%d", i), fmt.Sprintf("val_%d", i))
	}
	if v, err := ht.Get("key_75"); err != nil || v != "val_75" {
		t.Fatal("collision lookup failed")
	}
	if !ht.Remove("key_75") || ht.Contains("key_75") {
		t.Fatal("collision chain remove failed")
	}

	_ = ht.SerializeText("go_ht.json")
	htTxt := NewHashTable()
	_ = htTxt.DeserializeText("go_ht.json")
	if v, _ := htTxt.Get("k1"); v != "v1_updated" || htTxt.Size() != ht.Size() {
		t.Fatal("ht json deserialize failed")
	}

	_ = ht.SerializeBinary("go_ht.gob")
	htBin := NewHashTable()
	_ = htBin.DeserializeBinary("go_ht.gob")
	if v, _ := htBin.Get("k2"); v != "v2" || htBin.Size() != ht.Size() {
		t.Fatal("ht gob deserialize failed")
	}

	if !ht.Remove("k1") || ht.Contains("k1") {
		t.Fatal("remove failed")
	}
	os.Remove("go_ht.json")
	os.Remove("go_ht.gob")
}

// ==================== БЕНЧМАРКИ (go test -bench=.) ====================

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
		ht.Insert("key", "val")
	}
}