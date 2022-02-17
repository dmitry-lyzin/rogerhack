#pragma once
#include <iostream>
#include <cassert>

#define CE constexpr

// битовое поле  -> bitfield = { mask }
// битовый набор -> bitset   = { mask, val }
// ссылка на битовое поле -> bitfieldref? = { mask, val& }

// Загрузка строковой константы из потока.
// При успехе строковая константа из потока извлекается.
/*inline std::istream& operator>>( std::istream& is, const char *str )
{
    assert( str);
    for( unsigned int i = 0; *str; ++i)
    {
        int c = is.get();
        if( c != *str++)
        {
            if( c <= 0)
                is.clear();
            else
                is.putback( c);
            --str;
            for( ; i > 0; --i)
                is.putback( *--str);

            // При неудаче устанавливам failbit у потока, в остальном его состояние не меняется.
            is.setstate( std::ios_base::failbit );
            return is;
        }
    }
    return is;
}
*/
static bool get_from( std::istream& input, const char *token )
{
    assert( token);
    const char *p = token;
    while( *p )
    {
        if( input.peek() != *p++ )
        {
            --p;
            for( unsigned int i = p - token; i > 0; --i )
                input.unget();
            return false;
        }
        input.ignore();
    }
    return true;
}

CE static bool matches( const char *s1, const char *s2, unsigned int i = 0)
{
        return s1[i]    ? s1[i] == s2[i] && matches( s1, s2, i + 1)
                        : !s2[i];
}

template< typename Val>
struct Field
{
    Val val;
    Val mask;
    const char *name;

CE  Field( const char *name_, const Val& val_                    ): val( val_), mask( val_ ), name( name_) {}
CE  Field( const char *name_, const Val& val_, const Val& mask_  ): val( val_), mask( mask_), name( name_) {}
};

typedef Field< uint8_t>  Field8;
typedef Field< uint16_t> Field16;
typedef Field< uint32_t> Field32;
typedef Field< uint64_t> Field64;

template< typename Val >
struct BitSet0
{
    Val val;

    struct Bit
    {
        Val val;
        Val mask;

    CE  Bit( const Val &val_, const Val &mask_  ): val( val_    ), mask( mask_  ) {}
    CE  Bit( const Bit          &r              ): val( r.val   ), mask( r.mask ) {}
    CE  Bit( const Field< Val>  &r              ): val( r.val   ), mask( r.mask ) {}

    CE  Bit  operator~(             ) const { return Bit(~val        , mask         ); }
    CE  Bit  operator|( const Bit &r) const { return Bit( val | r.val, mask | r.mask); }
    CE  Bit  operator&( const Bit &r) const { return Bit( val & r.val, mask & r.mask); }
    };

CE  BitSet0(             ): val(                ) {}
CE  BitSet0( const Val &r): val( r              ) {}
CE  BitSet0( const Bit &r): val( r.val & r.mask ) {}
    void print( std::ostream &os, const Field< Val> *map, unsigned int maplen ) const
    {
        for( ; maplen > 0; )
        {
            --maplen;
            if( map->val == (val & map->mask) )
            {
                //os << ' ' << map->name;
                os << map->name;
                return;
            }
            map++;
        }
    }
    void read(  std::istream &is, const Field< Val> *map, unsigned int maplen )
    {
        if( !is)
            return;

        const Field< Val> *const map0 = map;
        const unsigned int maplen0 = maplen;

        for( ; maplen > 0; )
        {
            --maplen;
            if( get_from( is, map->name) )
            {
                val &= ~map->mask;
                val |=  map->val ;
                return;
            }
            //is.clear();
            map++;
        }

        std::cerr << "Wrong read! Wait words: ";
        const char* t = "\"";
        map = map0;
        for( maplen = maplen0; maplen > 0; )
        {
            --maplen;
            std::cerr << t << map->name << '"';
            t = " or \"";
            map++;
        }
        is.setstate( std::ios_base::failbit );
    }
};

template< typename Val, const Field< Val> map[], const unsigned int maplen>
class BitSet: BitSet0< Val>
{
    typedef BitSet0< Val> Base;

CE  static Field< Val> recurfind( const char *s, unsigned int i )
    {
        return matches( s, map[ i].name)  ? map[ i]
                                          : recurfind( s, i - 1);
                                          //: i > 0 ? recurfind( s, i - 1)
                                          //        : Field();
    }
CE  static Field< Val> value( const char *bitname )
    {
        return recurfind( bitname, maplen - 1);
    }

CE  BitSet( const Val &r): Base( r ) {}

public:
    struct Bit: Base::Bit
    {
    CE  Bit( const char *name               ): Base::Bit( value( name)  ) {}
    CE  Bit( const typename Base::Bit &r    ): Base::Bit( r             ) {}
    };
    struct Mask
    {
        Val mask;

    CE  Mask( const Bit          &r ): mask( r.mask             ) {}
    CE  Mask( const Field< Val>  &r ): mask( r.mask             ) {}
    CE  Mask( const char *name      ): mask( value( name).mask  ) {}
    };

CE  BitSet  getfield( const Mask &mask  ) const
    {
        return BitSet( Base::val & mask.mask );
    }
CE  bool    getvalue( const Bit  &bit   ) const
    {
        return (Base::val & bit.mask) == bit.val;
    }

CE  BitSet(                             ): Base(   ) {}
CE  BitSet( const typename Base::Bit &r ): Base( r ) {}

//  BitSet &operator = ( const Bit &r   )       { (Base::val &= ~r.mask) |= r.val; return *this; }
    BitSet &operator |=( const BitSet &r)       { Base::val |= r.val; return *this; }
    BitSet &operator &=( const BitSet &r)       { Base::val &= r.val; return *this; }
CE  BitSet  operator ~ (                ) const { return BitSet(~Base::val        );}
CE  BitSet  operator | ( const BitSet &r) const { return BitSet( Base::val | r.val);}
CE  BitSet  operator & ( const BitSet &r) const { return BitSet( Base::val & r.val);}

//CE  operator bool() const { return val; } // если раскоменьтит эту строку -> у BitSet появится operator[] !!!
    void print( std::ostream &os) const { Base::print( os, map, maplen);}
    void read(  std::istream &is)       { Base::read(  is, map, maplen);}
    friend std::ostream& operator<<( std::ostream &os, const BitSet &obj ) { obj.print( os); return os;}
    friend std::istream& operator>>( std::istream &is,       BitSet &obj ) { obj.read( is); return is; }
};

#define BITSET(x) BitSet< decltype((x)->val), (x), (sizeof(x)/sizeof(*(x))) >
