#pragma once

// -------------------------------------------------------------------------------------------
// helper function for ntCountedPtr class template to ensure that one pointer type can be
// implicitly converted to another
// -------------------------------------------------------------------------------------------
template <class type>
inline type implicit_cast( type obj )
{
	return obj;
}

// -------------------------------------------------------------------------------------------
// base class for all reference counted objects.  Such objects must never appear on the stack.
// -------------------------------------------------------------------------------------------
class ntCountedObject
{
public:
	ntCountedObject();
	ntCountedObject( const ntCountedObject& arg );
	virtual ~ntCountedObject();

	ntCountedObject& operator=( ntCountedObject const& arg );
	int get_reference_count() const;

protected:
	friend class ntCountedPtrBase;
	void add_reference() const;
	void remove_reference() const;
	mutable int m_reference_count;
};

// -------------------------------------------------------------------------------------------
// inlines for base reference counted object
// -------------------------------------------------------------------------------------------
inline ntCountedObject::ntCountedObject()
{
	m_reference_count = 0;
}

inline ntCountedObject::ntCountedObject( ntCountedObject const& )
{
	m_reference_count = 0;
}

inline ntCountedObject::~ntCountedObject()
{
}

inline ntCountedObject& ntCountedObject::operator =( ntCountedObject const& arg )
{
	// don't copy the reference count, keep our own.
	return *this;
}

inline void ntCountedObject::add_reference() const
{
	if (this)
		m_reference_count++;
}

inline int ntCountedObject::get_reference_count() const
{
	return m_reference_count;
}

inline void ntCountedObject::remove_reference() const
{
	if (this && --m_reference_count == 0)
		delete this;
}


// -------------------------------------------------------------------------------------------
// abstract reference counted pointer.
// -------------------------------------------------------------------------------------------
class  ntCountedPtrBase
{
public:
	ntCountedPtrBase();
	ntCountedPtrBase( ntCountedObject const * object );
	ntCountedPtrBase( ntCountedPtrBase const& pointer );
	~ntCountedPtrBase();

	bool operator==( ntCountedObject const* arg ) const;
	bool operator!=( ntCountedObject const* arg ) const;
	bool operator==( ntCountedPtrBase const& arg ) const;
	bool operator!=( ntCountedPtrBase const& arg ) const;
	bool operator!() const;

	bool operator<(ntCountedPtrBase const& arg ) const; 
	bool operator<=(ntCountedPtrBase const& arg ) const; 
	bool operator>(ntCountedPtrBase const& arg ) const; 
	bool operator>=(ntCountedPtrBase const& arg ) const; 

	ntCountedPtrBase& operator=( ntCountedObject const * object );
	ntCountedPtrBase& operator=( ntCountedPtrBase const& arg );
protected:
	ntCountedObject const * m_object;
};

// -------------------------------------------------------------------------------------------
// inlines for abstract reference counted pointer.
// -------------------------------------------------------------------------------------------
inline ntCountedPtrBase::ntCountedPtrBase()
{
	m_object = 0;
}

inline ntCountedPtrBase::ntCountedPtrBase( ntCountedObject const * object )
{
	m_object = object;
	object->add_reference();
}

inline ntCountedPtrBase::ntCountedPtrBase( ntCountedPtrBase const& pointer )
{
	m_object = pointer.m_object;
	m_object->add_reference();
}

inline ntCountedPtrBase::~ntCountedPtrBase()
{
	m_object->remove_reference();
}


inline bool ntCountedPtrBase::operator==( ntCountedObject const* arg ) const
{ 
	return m_object == arg; 
}

inline bool ntCountedPtrBase::operator!=( ntCountedObject const* arg ) const
{
	return m_object != arg; 
}

inline bool ntCountedPtrBase::operator==( ntCountedPtrBase const& arg ) const
{
	return m_object == arg.m_object;
}

inline bool ntCountedPtrBase::operator!=( ntCountedPtrBase const& arg ) const
{
	return m_object != arg.m_object;
}

inline bool ntCountedPtrBase::operator!() const
{
	return m_object == 0;
}

inline ntCountedPtrBase& ntCountedPtrBase::operator=( ntCountedObject const * object )
{
	object->add_reference();
	m_object->remove_reference();
	m_object = object;
	return *this;
}

inline ntCountedPtrBase& ntCountedPtrBase::operator=( ntCountedPtrBase const& arg )
{
	ntCountedObject* object = const_cast<ntCountedObject*>(arg.m_object);

	object->add_reference();
	m_object->remove_reference();
	m_object = object;
	return *this;
}

inline
bool ntCountedPtrBase::operator<(ntCountedPtrBase const& arg ) const
{
	return m_object < arg.m_object;
}

inline
bool ntCountedPtrBase::operator<=(ntCountedPtrBase const& arg ) const
{
	return m_object <= arg.m_object;
}

inline
bool ntCountedPtrBase::operator>(ntCountedPtrBase const& arg ) const
{
	return m_object > arg.m_object;
}

inline
bool ntCountedPtrBase::operator>=(ntCountedPtrBase const& arg ) const
{
	return m_object >= arg.m_object;
}


// -------------------------------------------------------------------------------------------
// template for type-safe usage of reference counted pointers
// -------------------------------------------------------------------------------------------
template<class type> 
class  ntCountedPtr : public ntCountedPtrBase
{
public:
	typedef type element_type;

	ntCountedPtr();
	ntCountedPtr( type* object );

	template <class other_type>
	  ntCountedPtr( ntCountedPtr<other_type> const& pointer )
		: ntCountedPtrBase( implicit_cast<type*>( pointer.get() ) )
	{
	}

	ntCountedPtr( ntCountedPtr const& pointer );

	type*       get() const;

	ntCountedPtr& operator=( type* arg );

	template < class t_other_type >
	ntCountedPtr& operator=( ntCountedPtr< t_other_type > const& arg )
	{
		return static_cast<ntCountedPtr<type>&>(ntCountedPtrBase::operator=( 
												 implicit_cast<element_type *>( arg.get() )));
	}

	ntCountedPtr& operator=( ntCountedPtr const& arg );
	 		       operator type* () const;
	type*          operator->() const;
	type&          operator*() const;
};

// -------------------------------------------------------------------------------------------
// inlines for counted pointer template
// -------------------------------------------------------------------------------------------

template <class type>
inline ntCountedPtr<type>::ntCountedPtr() 
	: ntCountedPtrBase() 
{
}

template <class type>
inline ntCountedPtr<type>::ntCountedPtr( type* object ) 
	: ntCountedPtrBase( object ) 
{
}

template <class type>
inline ntCountedPtr<type>::ntCountedPtr( const ntCountedPtr& pointer ) 
                          : ntCountedPtrBase( pointer ) 
{
}

template <class type>
inline ntCountedPtr<type>& ntCountedPtr<type>::operator=( type* arg )
{
	return static_cast<ntCountedPtr<type>&>(ntCountedPtrBase::operator=(arg)); 
}

template <class type>
inline type* ntCountedPtr<type>::get() const
{ 
	return const_cast<type*>( static_cast< type const * >(m_object) ); 
}

template <class type>
inline ntCountedPtr<type>::operator type* () const
{
	return const_cast<type*>( static_cast< type const * >(m_object) ); 
}

template <class type>
inline type* ntCountedPtr<type>::operator->() const
{
	return const_cast<type*>( static_cast< type const * >(m_object) ); 
}

template <class type>
inline type& ntCountedPtr<type>::operator*() const
{ 
	return const_cast<type&>( static_cast< type const & >( *m_object) ); 
}

template <class type>
inline ntCountedPtr<type>& ntCountedPtr<type>::operator=( const ntCountedPtr<type>& arg )
{ 
	return static_cast<ntCountedPtr<type>&>(ntCountedPtrBase::operator=(arg)); 
}

// -------------------------------------------------------------------------------------------
// template for type-safe usage of reference counted pointers
// For pointers to const objects
// -------------------------------------------------------------------------------------------
template<class t_type> 
class  ntCountedConstPtr : public ntCountedPtrBase
{
public:
	typedef t_type const t_element_type;

	ntCountedConstPtr();
	ntCountedConstPtr( t_element_type * object );

	template <class other_type>
	explicit ntCountedConstPtr( ntCountedConstPtr<other_type> const& pointer )
		: ntCountedPtrBase( implicit_cast<t_element_type*>( pointer.get() ) )
	{
	}

	ntCountedConstPtr( ntCountedConstPtr const& pointer );
	ntCountedConstPtr( ntCountedPtr<t_type> const& pointer );

	t_element_type* get() const;

	ntCountedConstPtr& operator=( t_element_type* arg );
	
	template < class t_other_type >
	  ntCountedConstPtr& operator=( ntCountedConstPtr< t_other_type > const& arg )
	{
		return static_cast<ntCountedConstPtr<t_type>&>(ntCountedPtrBase::operator=(implicit_cast<t_element_type *>( arg.get() ) ) ); 
	}

	template < class t_other_type >
	  ntCountedConstPtr& operator=( ntCountedPtr< t_other_type > const& arg )
	{
		return static_cast<ntCountedConstPtr<t_type>&>(ntCountedPtrBase::operator=(implicit_cast<t_element_type *>( arg.get() ) ) ); 
	}

	ntCountedConstPtr& operator=( ntCountedConstPtr const& arg );
	ntCountedConstPtr& operator=( ntCountedPtr<t_type> const& arg );
	 		             operator t_element_type* () const;
	t_element_type*      operator->() const;
	t_element_type&      operator*() const;
};

// -------------------------------------------------------------------------------------------
// inlines for counted pointer template
// -------------------------------------------------------------------------------------------

template <class t_type>
inline ntCountedConstPtr<t_type>::ntCountedConstPtr() : ntCountedPtrBase() 
{
}

template <class t_type>
inline ntCountedConstPtr<t_type>::ntCountedConstPtr( t_element_type* object ) 
	: ntCountedPtrBase( object ) 
{
}

template <class t_type>
inline ntCountedConstPtr<t_type>::ntCountedConstPtr( ntCountedConstPtr const& pointer ) 
                          : ntCountedPtrBase( pointer ) 
{
}

template <class t_type>
inline ntCountedConstPtr<t_type>::ntCountedConstPtr( ntCountedPtr<t_type> const& pointer )
								  : ntCountedPtrBase( pointer )
{
}

template <class t_type>
inline ntCountedConstPtr<t_type>& ntCountedConstPtr<t_type>::operator=( t_element_type * arg )
{
	return static_cast<ntCountedConstPtr<t_type>&>(ntCountedPtrBase::operator=(arg)); 
}

template <class t_type>
inline t_type const * ntCountedConstPtr<t_type>::get() const
{ 
	return static_cast<t_element_type*>(m_object); 
}

template <class t_type>
inline ntCountedConstPtr<t_type>::operator t_type const *() const
{
	return static_cast<t_element_type*>(m_object); 
}

template <class t_type>
inline t_type const * ntCountedConstPtr<t_type>::operator->() const
{
	return static_cast<t_element_type*>(m_object);
}

template <class t_type>
inline t_type const & ntCountedConstPtr<t_type>::operator*() const
{ 
	return static_cast<t_element_type&>(*m_object);
}

template < class t_type >
ntCountedConstPtr<t_type>& ntCountedConstPtr<t_type>::operator=( ntCountedConstPtr<t_type> const& arg )
{
	return static_cast<ntCountedConstPtr<t_type>&>( ntCountedPtrBase::operator=( arg )); 
}

template < class t_type >
ntCountedConstPtr<t_type>& ntCountedConstPtr<t_type>::operator=( ntCountedPtr<t_type> const& arg )
{
	return static_cast<ntCountedConstPtr<t_type>&>(ntCountedPtrBase::operator=( arg ) ); 
}

