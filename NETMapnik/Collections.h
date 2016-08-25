#pragma once

namespace NETMapnik
{
	namespace Collections
	{
		generic <class T> public ref class ListAdapter abstract : public System::Collections::Generic::IList<T>
		{

#pragma region enumerator
			ref class enumerator :System::Collections::Generic::IEnumerator<T>
			{
			public:
				enumerator(ListAdapter^ myList)
				{
					colInst = myList;
					currentIndex = -1;
				}
				virtual bool MoveNext() = System::Collections::Generic::IEnumerator<T>::MoveNext
				{
					if (currentIndex < colInst->Count - 1)
					{
						currentIndex++;
						return true;
					}
				return false;
				}
					property T Current
				{
					virtual T get() = System::Collections::Generic::IEnumerator<T>::Current::get
					{
						return colInst[currentIndex];
					}
				};
				// This is required as IEnumerator<T> also implements IEnumerator
				property Object^ Current2
				{
					virtual Object^ get() = System::Collections::IEnumerator::Current::get
					{
						return colInst[currentIndex];
					}
				};

				virtual void Reset() = System::Collections::Generic::IEnumerator<T>::Reset{}
					~enumerator() {}

			private:
				ListAdapter^ colInst;
				int currentIndex;
			};
#pragma endregion

		public:
			virtual System::Collections::Generic::IEnumerator<T>^ GetEnumerator()
			{
				return gcnew enumerator(this);
			}

			virtual System::Collections::IEnumerator^ GetEnumerator2() = System::Collections::IEnumerable::GetEnumerator
			{
				return gcnew enumerator(this);
			}

				virtual property T default[int]
			{
				T get(int index) abstract;
				void set(int index, T value) abstract;
			}
				virtual 	property bool IsReadOnly
			{
				virtual bool get() abstract;
			}

			virtual property int Count
			{
				virtual int get() abstract;
			}

			virtual int IndexOf(T item)
			{
				auto count = Count;
				if (System::IDisposable::typeid->IsAssignableFrom(T::typeid) && CanDisposeItems)
				{
					for (auto i = 0; i < count; i++)
					{
						auto obj = this[i];
						try
						{
							if (Equals(item, obj))
							{
								return i;
							}
						}
						finally
						{
							obj->~T();
						}
					}
				}
				else
				{
					for (auto i = 0; i < count; i++)
					{
						auto obj = this[i];
						if (Equals(item, obj))
						{
							return i;
						}
					}
				}
				return -1;
			}

			virtual bool Contains(T item)
			{
				return IndexOf(item) >= 0;
			}

			virtual void Insert(int index, T item) abstract;
			virtual void RemoveAt(int index) abstract;
			virtual bool Remove(T) abstract;
			virtual void Add(T) abstract;
			virtual void Clear() abstract;
			virtual void CopyTo(cli::array<T, 1> ^ array, int arrayIndex)
			{
				if (array == nullptr)
				{
					throw gcnew System::ArgumentNullException("array");
				}
				if (arrayIndex < 0)
				{
					throw gcnew System::ArgumentOutOfRangeException("arrayIndex");
				}
				if (array->Length - arrayIndex < Count)
				{
					throw gcnew System::ArgumentException("Destination array is too small", "array");
				}
				for each(auto item in this)
				{
					array[arrayIndex++] = item;
				}
			}
		protected:
			property bool CanDisposeItems
			{
				virtual bool get() abstract;
			}
		};
	}
}