template <typename T> struct ListNode {
  T data;
  ListNode<T>* next;
};

template <typename T> class Linked_List
{
  protected:
    int _size;
    ListNode<T>* root;
    ListNode<T>* last;
    SemaphoreHandle_t mutex;

  public:
    Linked_List();
    ~Linked_List();
    virtual int size()const;
    virtual bool add(T);
    virtual int find(T);
    virtual bool remove(int index);
    virtual bool remove(T);
    virtual T get(int index)const;
    virtual void print();
    virtual void clear();
    virtual void operator=(const Linked_List<T>& rhs);
    Linked_List(const Linked_List<T>& rhs);
};

template <typename T> Linked_List<T>::Linked_List()
{
  //Serial.println("\nIn the Linked List Constructor");
  root = NULL;
  last = NULL;
  mutex = xSemaphoreCreateMutex();
  _size = 0;
};

template <typename T> Linked_List<T>::~Linked_List()
{
  //Serial.println("\nIn the Linked List Destructor");
  if (_size > 0) {
    ListNode<T>* tmp = root;
    ListNode<T>* toDelete = tmp;
    for (int i = 0; i < _size; i++) {
      tmp = tmp->next;
      delete(toDelete);
      toDelete = tmp;
    }
    root = NULL;
    last = NULL;
    _size = 0;
  }
}

template <typename T> int Linked_List<T>::size() const
{
  return _size;
}

template <typename T> bool Linked_List<T>::add(T obj)
{
  //Serial.println("\nIn the Linked List add");
  ListNode<T>* tmp = new ListNode<T>();
  tmp->data = obj;
  tmp->next = NULL;
  while (xSemaphoreTake(mutex, 0) != true) {
    delay(1);
  }
  if (_size == 0) {
    root = tmp;
    last = tmp;
  } else {
    last->next = tmp;
    last = tmp;
  }
  _size++;
  xSemaphoreGive(mutex);
  return true;
}

template <typename T> int Linked_List<T>::find(T obj)
{
  //Serial.println("\nIn the Linked List find");
  if (_size == 0) {
    return -1;
  } else {
    ListNode<T>* tmp = root;
    while (xSemaphoreTake(mutex, 0) != true) {
      delay(1);
    }
    for (int i = 0; i < _size; i++) {
      if (tmp->data == obj) {
        xSemaphoreGive(mutex);
        return i;
      }
      else {
        tmp = tmp->next;
      }
    }
    xSemaphoreGive(mutex);
    return -1;
  }
}

template <typename T> bool Linked_List<T>::remove(int index)
{
  //Serial.println("\nIn the Linked List remove");
  if (index > _size || index < 0 || _size == 0) {
    return false;
  } else {
    ListNode<T>* tmp = root;
    ListNode<T>* toDelete = root;
    while (xSemaphoreTake(mutex, 0) != true) {
      delay(1);
    }
    if (index == 0) {
      root = tmp->next;
      delete(toDelete);
    } else {
      for (int i = 0; i < index - 1; i++) {
        tmp = tmp->next;
      }
      toDelete = tmp->next;
      tmp->next = toDelete->next;
      delete(toDelete);
      if ((index + 1) == _size) {
        last = tmp;
      }
    }
    _size -= 1;
    xSemaphoreGive(mutex);
    return true;
  }
}

template <typename T> bool Linked_List<T>::remove(T obj)
{
  int index = find(obj);
  return remove(index);
}

template <typename T> T Linked_List<T>::get(int index) const
{
  //Serial.println("\nIn the Linked List get");
  T out;
  while (xSemaphoreTake(mutex, 0) != true) {
    delay(1);
  }
  if ((_size > 0) && (index < _size) && (index >= 0)) {
    ListNode<T>* tmp = root;
    for (int i = 0; i < _size; i++) {
      if (index == i) {
        out = tmp->data;
      }
      tmp = tmp->next;
    }
  }
  xSemaphoreGive(mutex);
  return out;
}
template <typename T> void Linked_List<T>::print()
{
  Serial.println("\n=================Contents of Linked List:=================");
  if (_size > 0) {
    while (xSemaphoreTake(mutex, 0) != true) {
      delay(1);
    }
    ListNode<T>* tmp = root;
    for (int i = 0; i < _size; i++) {
      tmp->data.print();
      tmp = tmp->next;
    }
    xSemaphoreGive(mutex);
  }
  else {
    Serial.println("\n                      List is Empty                       \n");
  }
}

template <typename T> void Linked_List<T>::clear()
{
  //Serial.println("\nIn the Linked List clear");
  if (_size > 0) {
    ListNode<T>* tmp = root;
    ListNode<T>* toDelete = tmp;
    while (xSemaphoreTake(mutex, 0) != true) {
      delay(1);
    }
    for (int i = 0; i < _size; i++) {
      tmp = tmp->next;
      //toDelete->data.~BLE_DEVICE();
      delete(toDelete);
      toDelete = tmp;
    }
    xSemaphoreGive(mutex);
    root = NULL;
    last = NULL;
    _size = 0;
  }
}

template <typename T> void Linked_List<T>::operator=(const Linked_List<T>& rhs)
{
  if (_size != 0) {
    clear();
    for (int i = 0; i < rhs.size(); i++) {
      this->add(rhs.get(i));
    }
  }
}
template <typename T> Linked_List<T>::Linked_List(const Linked_List<T>& rhs)
{
  this->_size = 0;
  this->root = NULL;
  this->last = NULL;
  for (int i = 0; i < rhs.size(); i++) {
    this->add(rhs.get(i));
  }
}
