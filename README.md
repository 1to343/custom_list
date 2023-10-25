# "Лист"

Реализовал шаблонный класс List\<T, Allocator\> аналог std::list из stl.

Реализация основана на нодах (класс Node) которые содержат указатели на лево и на право.

Класс List:

* Имеет базовую функциональность + итераторы
* Имеет поддержку аллокаторов
* Является exception-safety
* Все взаимодействия с памятью реализованы через аллокаторы
* Поддерживает propagate_on_container_copy в соответствующих методах
## Методы:

### Using'и

* value_type
* allocator_type
* iterator (удовлетворяет https://en.cppreference.com/w/cpp/named_req/BidirectionalIterator)

### Конструкторы

В моем классе реализованы следующие конструкторы:

* List()
* List(size_t count, const T& value = T(), const Allocator& alloc = Allocator())
* explicit List(size_t count, const Allocator& alloc = Allocator())
* List(const list& other);
* List(std::initializer_list\<T\> init, const Allocator& alloc = Allocator())


### Iterators (с поддержкой константных)

* begin()
* end()
* cbegin()
* cend()

### operator=

* List& operator=(const List& other)


### element access methods

* T& front()
* const T& front() const
* T& back()
* const T& back() const


### Capacity

* bool empty()
* size_t size()

### Modifiers

* push_back(front)(const T&)
* push_back(front)(T&&)
* pop_back(front)();


