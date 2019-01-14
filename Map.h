#pragma once

#include <cassert>

template<typename TKey, typename TVal>
class Map
{
	unsigned buffLen;
	unsigned dataLen;
	TKey* pKey;
	TVal* pVal;
public:
	Map() : buffLen( 0 ), dataLen( 0 ), pKey( nullptr ), pVal( nullptr ) {};

	Map( const Map<TKey, TVal>& other )
		//: Map()
		: dataLen( other.dataLen )
	{
		if( pKey )//&& pVal
		{
			Allocate( other.dataLen );
			for( unsigned i = 0; i < dataLen; ++i )
			{
				new ( &pKey[ i ] ) TKey( other.pKey[ i ] );
				new ( &pVal[ i ] ) TVal( other.pVal[ i ] );
			}
		}
	}

	Map( Map<TKey, TVal>&& other )
		: Map()
	{
		Swap( other );
	}

	~Map()
	{
		Clear();
	}

	void Clear( bool bClearBuff = true )
	{
		if( pKey )//&& pVal
		{
			for( unsigned i = 0; i < dataLen; ++i )
			{
				( ( TKey* ) &pKey[ i ] )->~TKey();
				( ( TVal* ) &pVal[ i ] )->~TVal();
			}

			dataLen = 0;

			if( bClearBuff )
			{
				std::free( pKey );
				pKey = nullptr;
				std::free( pVal );
				pVal = nullptr;

				buffLen = 0;
			}
		}
	}

	bool Count( const TKey& key )
	{
		for( unsigned i = 0; i < dataLen; ++i )
		{
			if( pKey[ i ] == key )
				return true;
		}
		return false;
	}

	void Emplace( const TKey& key, const TVal& val )
	{
		unsigned ind = 0;
		while( ind < dataLen && key > pKey[ ind ] )
		{
			++ind;
		}

		if( !pKey || key != pKey[ ind ] )
		{
			InsertItem( ind, key, val );
		}
	}

	//?EqualRange

	//Find

	//Erase
	//Insert

	unsigned Size() { return dataLen; }
	bool Empty() { return !dataLen; }

	TVal& operator[ ]( const TKey& key )
	{
		unsigned ind = 0;
		while( ind < dataLen && key > pKey[ ind ] )
		{
			++ind;
		}

		if( !pKey || key != pKey[ ind ] )
		{
			InsertItem( ind, key );
			return pVal[ ind ];
		}

		return pVal[ ind ];
	}

	Map<TKey, TVal>& operator=( const Map<TKey, TVal>& other )
	{
		Clear( false );	//buff not clear
		if( other.dataLen > buffLen )
		{
			Allocate( other.dataLen );
		}

		dataLen = other.dataLen;
		for( unsigned i = 0; i < dataLen; ++i )
		{
			new ( &pKey[ i ] ) TKey( other.pKey[ i ] );
			new ( &pVal[ i ] ) TVal( other.pVal[ i ] );
		}

		return *this;
	}
	Map<TKey, TVal>& operator=( Map<TKey, TVal>&& other )
	{
		Swap( other );
		return *this;
	}

	void Print()
	{
		std::cout << "BuffLen = " << buffLen << std::endl;
		std::cout << "DataLen = " << dataLen << std::endl;
		for( unsigned i = 0; i < dataLen; ++i )
		{
			std::cout << "{ " << pKey[ i ] << " : " << pVal[ i ] << " }" << std::endl;
		}
	}

private:
	void InsertItem( unsigned ind, const TKey& key/*, TVal&& val = TVal() */ )
	{
		Allocate( dataLen + 1 );

		if( ind == dataLen )
		{
			new ( &pKey[ dataLen ] ) TKey( key );
			new ( &pVal[ dataLen ] ) TVal();
			++dataLen;
			return;
		}

		std::memmove( ( TKey* ) &pKey[ ind + 1 ], ( TKey* ) &pKey[ ind ], ( dataLen - ind ) * sizeof( TKey ) );
		new ( &pKey[ ind ] ) TKey( key );
		std::memmove( ( TVal* ) &pVal[ ind + 1 ], ( TVal* ) &pVal[ ind ], ( dataLen - ind ) * sizeof( TVal ) );
		new ( &pVal[ ind ] ) TVal();
		++dataLen;
	}

	void InsertItem( unsigned ind, const TKey& key, const TVal& val )
	{
		Allocate( dataLen + 1 );

		if( ind == dataLen )
		{
			new ( &pKey[ dataLen ] ) TKey( key );
			new ( &pVal[ dataLen ] ) TVal(val);
			++dataLen;
			return;
		}

		std::memmove( ( TKey* ) &pKey[ ind + 1 ], ( TKey* ) &pKey[ ind ], ( dataLen - ind ) * sizeof( TKey ) );
		new ( &pKey[ ind ] ) TKey( key );
		std::memmove( ( TVal* ) &pVal[ ind + 1 ], ( TVal* ) &pVal[ ind ], ( dataLen - ind ) * sizeof( TVal ) );
		new ( &pVal[ ind ] ) TVal(val);
		++dataLen;
	}

	void Allocate( unsigned n )
	{
		if( !pKey )//&& !pVal
		{
			//buffLen = 128 / sizeof( Type );

			//if( buffLen < n )
			buffLen = n;

			pKey = ( TKey* ) std::malloc( buffLen * sizeof( TKey ) );
			pVal = ( TVal* ) std::malloc( buffLen * sizeof( TVal ) );
			assert( pKey && pVal );
		}
		else //if( n > buffLen )
		{
			//buffLen = buffLen * 6 / 5;
			//if( n > buffLen )
			buffLen = n;

			TKey* newKey = ( TKey* ) std::malloc( buffLen * sizeof( TKey ) );
			TVal* newVal = ( TVal* ) std::malloc( buffLen * sizeof( TVal ) );
			assert( newKey && newVal );
			std::memcpy( newKey, pKey, dataLen * sizeof( TKey ) );
			std::memcpy( newVal, pVal, dataLen * sizeof( TVal ) );
			std::free( pKey );
			pKey = newKey;
			std::free( pVal );
			pVal = newVal;
		}
	}

	void Swap(Map<TKey, TVal>& tmp)
	{
		std::swap( buffLen, tmp.buffLen );
		std::swap( dataLen, tmp.dataLen );
		std::swap( pKey, tmp.pKey );
		std::swap( pVal, tmp.pVal );
	}
};
