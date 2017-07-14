/********************************************

  Numeric format

  Implementation file

  Taylor Shin

  July 13th 2017

*********************************************/

/* Read Data */
template<class T>
T& Num<T>::Data()
{
  T* p_data = data.get();
  return *p_data;
}

/* Get size */
template<class T>
size_t Num<T>::Size()
{
  if (data == nullptr) return 0;
  return sizeof(T);
}

/* Set data */
template<class T>
void Num<T>::Set(const T& new_data)
{
  data = std::make_unique<T>(new_data);
}


/* Constructors and Destructors */
template<class T>
Num<T>::Num() : data(std::make_unique<T>(nullptr)) {;}

template<class T>
Num<T>::Num(const T& given_data) : Num()
{
  Set(given_data);
}

template<class T>
Num<T>::Num(const Num& other) : Num()
{
  Set(other.Data());
}

template<class T>
Num<T>::~Num() noexcept {;}
