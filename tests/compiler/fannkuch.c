/*
Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.

    * Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.

    * Neither the name of "The Computer Language Benchmarks Game" nor the
    name of "The Computer Language Shootout Benchmarks" nor the names of
    its contributors may be used to endorse or promote products derived
    from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
*/

/*
 * The Computer Language Shootout
 * http://shootout.alioth.debian.org/
 * Contributed by Heiner Marxen
 *
 * "fannkuch"	for C gcc
 *
 * $Id: fannkuch.1.gcc.code,v 1.15 2009-04-28 15:39:31 igouy-guest Exp $
 */
#include <stdio.h>
#include <stdlib.h>

#define Int	int
#define Aint	int

static int
fannkuch( int n )
{
    Aint*	perm;
    Aint*	perm1;
    Aint*	count;
    int	flips;
    int	flipsMax;
    Int		r;
    Int		i;
    Int		k;
    Int		didpr;
    const Int	n1	= n - 1;

    if( n < 1 ) return 0;

    perm  = (Aint*) calloc(n, sizeof(*perm ));
    perm1 = (Aint*) calloc(n, sizeof(*perm1));
    count = (Aint*) calloc(n, sizeof(*count));

    for( i=0 ; i<n ; ++i ) perm1[i] = i;	/* initial (trivial) permu */

    r = n; didpr = 0; flipsMax = 0;
    for(;;) {
	if( didpr < 30 ) {
	    for( i=0 ; i<n ; ++i ) 
	    {
			printf("%c", (char)('0' + (1+perm1[i])));
	    }
	    
		printf("\n");
	    ++didpr;
	}
	for( ; r!=1 ; --r ) {
	    count[r-1] = r;
	}

#define XCH(x,y)	{ Aint t_mp; t_mp=(x); (x)=(y); (y)=t_mp; }

	if( ! (perm1[0]==0 || perm1[n1]==n1) ) {
	    flips = 0;
	    for( i=1 ; i<n ; ++i ) {	/* perm = perm1 */
		perm[i] = perm1[i];
	    }
	    k = perm1[0];		/* cache perm[0] in k */
	    do {			/* k!=0 ==> k>0 */
		Int	j;
		for( i=1, j=k-1 ; i<j ; ++i, --j ) {
		    XCH(perm[i], perm[j])
		}
		++flips;
		/*
		 * Now exchange k (caching perm[0]) and perm[k]... with care!
		 * XCH(k, perm[k]) does NOT work!
		 */
		j=perm[k]; perm[k]=k ; k=j;
	    }while( k );
	    if( flipsMax < flips ) {
		flipsMax = flips;
	    }
	}

	for(;;) {
	    if( r == n ) {
		return flipsMax;
	    }
	    /* rotate down perm[0..r] by one */
	    {
		Int	perm0 = perm1[0];
		i = 0;
		while( i < r ) {
		    k = i+1;
		    perm1[i] = perm1[k];
		    i = k;
		}
		perm1[r] = perm0;
	    }
	    if( (count[r] -= 1) > 0 ) {
		break;
	    }
	    ++r;
	}
    }
}

int
main( int argc, char* argv[] )
{
	int n = 6;

	printf("0x%08x\n", fannkuch(n));
    return 0;
}

// CHECK: 123456
// CHECK: 213456
// CHECK: 231456
// CHECK: 321456
// CHECK: 312456
// CHECK: 132456
// CHECK: 234156
// CHECK: 324156
// CHECK: 342156
// CHECK: 432156
// CHECK: 423156
// CHECK: 243156
// CHECK: 341256
// CHECK: 431256
// CHECK: 413256
// CHECK: 143256
// CHECK: 134256
// CHECK: 314256
// CHECK: 412356
// CHECK: 142356
// CHECK: 124356
// CHECK: 214356
// CHECK: 241356
// CHECK: 421356
// CHECK: 234516
// CHECK: 324516
// CHECK: 342516
// CHECK: 432516
// CHECK: 423516
// CHECK: 243516
// CHECK: 0x0000000a

