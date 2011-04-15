/*
* $OFPREPARED$
*
* $Revision: 2.0 $
*
* $Id: otherutil.i,v 2.0 2006-01-01 20:08:32 timmy Exp $
*/

template<class T>
void
printbin( T x )
{
    T lastbit = (T)((T)1 << (T)(sizeof(T)*8-1));
    for ( char z = 0; z < sizeof(T)*8; z++ )
    {
        if ( x & lastbit )
            cout << "1";
        else
            cout << "0";
        x = x << 1;
    }
}

template<class T>
T
reversebits( T in )
{
    T ret = 0;
    T lastbit = (T)((T)1 << (T)(sizeof(T)*8-1));
    for ( int x = 0; x < sizeof(T)*8; x++ )
    {
        // Shift result (and everything else) down 1
        ret = ret >> 1;

        // Get the end bit and stick it in at the very top
        ret |= ( in & lastbit );

        // Pop off the top of the input
        in = in << 1;
    }
    return ret;
}
