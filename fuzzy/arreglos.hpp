#ifndef __ARREGLOS_HPP
#define __ARREGLOS_HPP
#endif
template<class T>
class Arreglo
{
public:
	Arreglo(int sz=10)
	{
		init(sz);
	}
	virtual ~Arreglo(void)
	{
		delete [] data;
	}
	void Add(T* dato);
	void AddAt(int pos,T* dato);
	int size(void) const;
	int GetItemsInContainer(void) const;
	T *dato(int index);
	void Destroy(int index);
	void Detach(int index);
	void FlushDestroy();
	void FlushDetach();
private:
	int n;
	int items;
	T **data;
	void init(int sz);
};

template <class T>
void Arreglo<T>::init (int sz)
{
	if(sz<1)
	{
		sz=1;
	}
	n=sz;
	items=0;
	data=new T*[n];
}

template <class T>
int Arreglo<T>::size (void ) const
{
	return (n);
}

template <class T>
int Arreglo<T>::GetItemsInContainer (void ) const
{
	return (items);
}

template <class T>
T *Arreglo<T>::dato (int index)
{
	if (index <0){index=0;}
	if (index >=n){index=n-1;}
	return (data[index]);
}

template <class T>
void Arreglo<T>::Add(T* dato)
{
	items++;
	if(items>n)
	{
		T** temp;
		temp=new T*[items];
		n=items;
		int i;
		for(i=0;i<items-1;i++)
		{
			temp[i]=data[i];
		}
		delete [] data;
		data=temp;
	}
	data[items-1]=dato;
}

template <class T>
void Arreglo<T>::AddAt(int pos, T* dato)
{
  if(pos<items)
  {
    data[pos]=dato;
  }else
  {
    Add(dato);
  }
}

template <class T>
void Arreglo<T>::Destroy (int index)
{
	T* temp;
	temp=dato(index);
	delete temp;
  Detach(index);
}

template <class T>
void Arreglo<T>::Detach (int index)
{
	int i;
	for(i=index;i<items-1;i++)
	{
		data[i]=data[i+1];
	}
	items--;
}

template <class T>
void Arreglo<T>::FlushDestroy()
{
  while(items>0)
  {
    Destroy(0);
  };
}

template <class T>
void Arreglo<T>::FlushDetach()
{
  while(items>0)
  {
    Detach(0);
  };
}


