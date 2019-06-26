#ifndef __ARREGLOS_HPP
#define __ARREGLOS_HPP
#endif

template<class T>  //esta clase almacena apuntadores
class Arreglo
{
public:
	Arreglo(int sz=10)
	{
		init(sz);
	}
	virtual ~Arreglo(void)
	{
		FlushDestroy();
		delete [] data;
	}
	void Add(T* dato);
	void AddAt(int loc,T* dato);
	int size(void) const;
	int GetItemsInContainer(void) const;
	T **ptrdato(int index);
	T *dato(int index);
	T *datoConst(int index) const;
	int buscar(T *d);
	int buscarPtr(T *d);
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
T **Arreglo<T>::ptrdato (int index)
{
	if (index <0){index=0;}
	if (index >items){index=items;}
	return (&data[index]);
}

template <class T>
T *Arreglo<T>::dato (int index)
{
	if (index <0){index=0;}
	if (index >items){index=items;}
	return (data[index]);
}

template <class T>
T *Arreglo<T>::datoConst (int index) const
{
	if (index <0){index=0;}
	if (index >items){index=items;}
	return (data[index]);
}

template <class T>
int Arreglo<T>::buscar (T *d)
{
	int ind=-1;
	int i;
	for(i=0;i<items;i++)
	{
		if(*data[i]==*d)
		{
			ind=i;
			i=items;
		}
	}
	return (ind);
}

template <class T>
int Arreglo<T>::buscarPtr (T *d)
{
	if(d==NULL){return -1;}
	int ind=-1;
	int i;
	for(i=0;i<items;i++)
	{
		if(data[i]==d)
		{
			ind=i;
			i=items;
		}
	}
	return (ind);
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
void Arreglo<T>::AddAt(int loc,T* dato)
{
	if(loc<0){loc=0;}
	if(loc>items){loc=items;}
	items++;

	T** temp;
	temp=new T*[items];
	n=items;
	int i;
	for(i=0;i<loc;i++)
	{
		temp[i]=data[i];
	}
	temp[loc]=dato;
	for(i=loc+1;i<items;i++)
	{
		temp[i]=data[i-1];
	}
	delete [] data;
	data=temp;
}

template <class T>
void Arreglo<T>::Destroy (int index)
{
	T* temp;
	temp=dato(index);
	int i;
	for(i=index;i<items-1;i++)
	{
		data[i]=data[i+1];
	}
	items--;
	delete temp;
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
	int i,tam;
	tam=items;
	for(i=0;i<tam;i++)
	{
		Destroy(0);
	}
}

template <class T>
void Arreglo<T>::FlushDetach()
{
	int i,tam;
	tam=items;
	for(i=0;i<tam;i++)
	{
		Detach(0);
	}
}

