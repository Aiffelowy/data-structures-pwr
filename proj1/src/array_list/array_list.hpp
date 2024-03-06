#ifndef MY_ARRAY_LIST
#define MY_ARRAY_LIST


template <typename T>
struct ArrayList {
private:
  T* array;
  int length;
  int size;

public:
  ArrayList();
  ArrayList(int size);
  ~ArrayList();

  void push(T item);
  void insert(T item, int place);
  T pop();
  void expand();
  T* operator[](int i);
};



#endif
